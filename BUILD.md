# Build

Everything is done by a script in `tools/`. You run all of them the same way —
from the repo root, on your own machine. What differs is where the work then
happens.

Every build lands in `build/`, whichever route it took.

## What runs what

**On Windows**, in the MSYS2 UCRT64 shell:

- **`tools/build.sh`** → `build/`
  - cmake and ninja against the native Qt
- **`tools/deploy.sh`** → `dist/`
  - `windeployqt6`, then `ntldd` for what it misses
- **`tools/gpttest/`** → pass or fail
  - links the real `src/disk.cpp` and exercises the GPT repair

**On Linux, with the cross toolchain installed:**

- **`tools/build-cross.sh`** → `build/`
  - cmake and ninja against the MinGW Qt
- **`tools/deploy-cross.sh`** → `dist/`
  - `objdump`, and the Qt plugins gathered by hand

**On Linux, without it** — the same build, one step further out:

- **`tools/build-container.sh`** → `build/`
  - podman, which runs **`tools/build-cross.sh`** inside the image

  You run this on the host; it is the *build* that happens in a container. The
  script is a wrapper and nothing else.

**On Windows or Linux:**

- **`tools/lupdate.sh`** → `src/lang/*.ts`
  - whichever Qt 6 `lupdate` the host has — MSYS2's `lupdate`, Fedora's
    `lupdate-qt6` — else podman running *itself* in the image

**On Linux:**

- **`tools/make-test-images.sh`** → test images
  - `sfdisk`, `mkfs.vfat`, gzip and xz
- **`tools/verify-flashed.sh`** → pass or fail
  - `cmp`, an image against a device

Everything that cross-builds reads **`tools/build-env.sh`** for the packages,
toolchain paths, cmake flags and podman plumbing — including
`tools/Containerfile.build` when it builds the image, and
[.github/workflows/build.yml](.github/workflows/build.yml), which runs
`tools/build-cross.sh` the same as anyone else.

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
tools/build.sh test
```

It lands in `build/` like any other build, replacing whatever was there, so
switching back is just `tools/build.sh` again.

It **cannot open a device**, so use it for layout, tooltips, translations and
dialog text, and a normal build for anything that touches a card. Both
deploy scripts refuse to package one, since the two are indistinguishable once
the exe sits in a folder of its own. To tell them apart by hand:

```
grep -ac 'level="asInvoker"' build/Win32DiskImager.exe    # 1 = test build
```

## Windows, cross-compiled from Linux

It has to be Fedora: Debian and Ubuntu ship no MinGW Qt6 packages, so there is
nothing to link against there. That is why CI runs `ubuntu-latest` but inside a
`fedora:44` container.

`tools/build-cross.sh` *is* the cross build. Run it on a Fedora host and it
builds; `tools/build-container.sh` runs that same script inside the container;
[.github/workflows/build.yml](.github/workflows/build.yml) runs it too. However
this is built, it is built by that one script, with the toolchain and flags from
`tools/build-env.sh`.

**On a Fedora host.** Install the toolchain once:

```
sudo bash tools/build-env.sh install
```

then:

```
tools/build-cross.sh
tools/deploy-cross.sh build dist
```

**Anywhere podman runs**, including a Fedora host that would rather not install
the toolchain. The image builds itself on first use:

```
tools/build-container.sh
tools/deploy-cross.sh build dist
```

That script is a wrapper: it starts the container and runs `build-cross.sh`
inside it, where the toolchain is already installed. Both take the same
arguments — `clean` to start over, `test` for a no-elevation build — and both
fail if what comes out is not a win64 PE binary, which is what a host compiler
picked up by mistake would produce.

Either one before a push catches a broken cross build without waiting on the
workflow. Everything they write is gitignored, so the build directory persists
and ninja stays incremental: a no-op rebuild is well under a second, a one-file
change around twenty.

They share `build/`, and switching between them costs one reconfigure. A cmake
cache is tied to the path it was generated for, and the container sees this tree
as `/src`, so a cache from the other route is dropped and rebuilt -- the script
says so when it happens. `BUILD_DIR=...` overrides the directory.

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
tools/lupdate.sh              # every language
tools/lupdate.sh de fr        # only those
```

This works anywhere: it uses whichever Qt 6 `lupdate` it can find — MSYS2's
`lupdate` on Windows, Fedora's `lupdate-qt6` — and falls back to running itself
in the container when the host has neither. Only version 6 is accepted, since a
Qt 5 `lupdate` writes `.ts` files the Qt 6 build then has to interpret.

Unlike the build scripts it rewrites tracked files, so review the diff:

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
`lrelease-qt6` and `lupdate-qt6`, the cmake flags, the MSYS2 package list, and
the podman plumbing every container run goes through — lives in
[tools/build-env.sh](tools/build-env.sh). The container image, CI and every cross
script read it, so a local container build and the CI job cannot end up on
different toolchains. Each path can be pointed elsewhere for a host that lays
them out differently:

```
CROSS_LUPDATE=/usr/bin/lupdate-qt6 tools/lupdate.sh
```

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
