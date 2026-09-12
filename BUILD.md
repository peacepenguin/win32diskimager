# Build

Everything is done by a script in `tools/`. This file says which one to run and
why; the details are in the scripts themselves.

| | |
| --- | --- |
| `tools/build.sh` | build on Windows, in the MSYS2 UCRT64 shell |
| `tools/deploy.sh` | package that build into a standalone `dist/` |
| `tools/build-cross.sh` | build for Windows from Linux, in a container |
| `tools/deploy-cross.sh` | package a cross build |
| `tools/lupdate-cross.sh` | refresh `src/lang/*.ts` after changing any text |
| `tools/gpttest/` | test the GPT repair without a card |
| `tools/build-env.sh` | the toolchain itself: packages, paths, cmake flags |

## Windows, natively

Once:

```
winget install --id MSYS2.MSYS2 -e
```

Then in the **MSYS2 UCRT64** shell:

```
pacman -S --needed $(bash tools/build-env.sh packages-msys2)
```

Then, any time:

```
tools/build.sh                 # into build/
tools/deploy.sh                # into dist/
```

`build/Win32DiskImager.exe` only runs inside the MSYS2 shell, because its Qt
DLLs are not next to it. `dist/` is the standalone copy: move it anywhere and run
`dist/Win32DiskImager.exe` **as Administrator**, which raw device access needs.

`tools/build.sh clean` drops the build directory first. Without it, ninja stays
incremental.

## Working on the interface

Every launch of the real binary raises a UAC prompt, which gets old when the
change under test is a tooltip. This builds one that does not:

```
tools/build.sh test            # into build-test/
```

It also **cannot open a device**, so use it for layout, tooltips, translations
and dialog text, and a normal build for anything that touches a card. Both
deploy scripts refuse to package one, since the two are indistinguishable once
the exe sits in a folder of its own. To tell them apart by hand:

```
grep -ac 'level="asInvoker"' build/Win32DiskImager.exe    # 1 = test build
```

## Windows, cross-compiled from Linux

This is what [.github/workflows/build.yml](.github/workflows/build.yml) does on
every push. Locally it needs podman; the image builds itself on first use:

```
tools/build-cross.sh           # into build/
tools/deploy-cross.sh build dist
```

`tools/build-cross.sh clean` starts over. Everything the container writes is
gitignored, so the build directory persists and ninja stays incremental between
runs: a no-op rebuild is well under a second, a one-file change around twenty.

Worth running before a push — same image, toolchain and cmake invocation as CI,
so it catches a broken cross build without waiting on the workflow.

It has to be Fedora: Debian and Ubuntu ship no MinGW Qt6 packages, so there is
nothing to link against there. That is why the CI job runs `ubuntu-latest` but
inside a `fedora:44` container. Without podman, install the toolchain on a Fedora
box directly and skip the container:

```
sudo bash tools/build-env.sh install
bash tools/build-env.sh configure src build
cmake --build build
```

## Testing the GPT repair

`relocateBackupGPT()` rewrites partition tables and zeroes the stale backup left
mid-device, so a mistake there destroys data rather than merely misbehaving.
This exercises it against a file standing in for a device — no card, no VM, no
UAC prompt, about a second per run:

```
cmake -S tools/gpttest -B build-gpttest -G Ninja
cmake --build build-gpttest
./build-gpttest/gpttest.exe
```

It exits non-zero if any check fails, and compiles the real `src/disk.cpp`, so
the code under test is the shipped code. MSYS2 UCRT64 only — it is Win32 code.

Seven cases, 60 checks. Three relocate a backup stranded mid-device; the other
four are cases where **nothing may be written** — a stale copy covered by a
partition, no GPT at all, a backup already at the last LBA, and a corrupt
header. Those are the point. When changing the repair, confirm the harness still
fails when it should: break a guard on purpose, watch it go red, put it back.

## Updating the translations

`src/lang/*.ts` hold the translations; the build compiles them to `.qm` and
embeds those through `translations.qrc`. Adding or changing a `tr()` string does
not reach the `.ts` files on its own — until they are refreshed, a new string
falls back to English and a *reworded* one silently loses the translation it had.

```
tools/lupdate-cross.sh              # every language
tools/lupdate-cross.sh de fr        # only those
```

This runs the native `lupdate-qt6` in the same container the cross build uses, so
it needs no Qt on the host. Unlike the build scripts it rewrites tracked files,
so review the diff:

- **New strings** arrive as `<translation type="unfinished"></translation>`.
  `lrelease` skips them and the app shows English until someone fills them in.
- **Reworded strings** appear as a new unfinished entry while the old translation
  is kept beside it as `type="vanished"`, so a translator can adapt it. Nothing
  is lost.
- Most of the diff is `<location>` line numbers moving with the source. Expected
  and harmless.

Run it before a release, or after any commit that touches user-visible text.

## Where things are defined

Two lists would otherwise be written down in several places, so each has one home
and everything else reads it from there.

**The toolchain** — Fedora packages, the MinGW sysroot, the paths to
`lrelease-qt6` and `lupdate-qt6`, the cmake flags, and the MSYS2 package list —
lives in [tools/build-env.sh](tools/build-env.sh). The container image, CI and
every cross script read it, so a local container build and the CI job cannot end
up on different toolchains.

```
tools/build-env.sh packages ci      # what CI installs
tools/build-env.sh print SYSROOT    # where it expects the MinGW tree
```

**The shipped languages** live in `src/CMakeLists.txt`, and both deploy scripts
read the list from there, so trimming Qt's translations cannot drift from the set
the build compiles. Add a language in one place:

```
set(LANGUAGES es it pl nl de fr zh_CN zh_TW ta_IN ko ja)
```

## Notes

The two deploy scripts are deliberately parallel — same files copied in, same Qt
plugin groups, same trimming of Qt's translations, same iterate-until-stable DLL
closure, same refusal to package a test build. They differ only where the
platform forces it: `windeployqt6` and `ntldd` on Windows against a hand-rolled
copy and `objdump -p` on Linux, and `deploy.sh` empties `dist/` in place rather
than deleting it, because Explorer locks the directory node on Windows.

`windeployqt` cannot run on Linux at all, being a Windows binary, which is why
the cross deploy gathers the Qt plugins and translations by hand. It also strips
the result: Fedora ships its MinGW DLLs unstripped, and `libstdc++-6.dll` alone
is ~26 MB otherwise.

The `.rc` file must reference the icon with a forward slash. Windows `windres`
accepts a backslash there; the cross build does not.
