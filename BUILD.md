# Build

Binary-only build (no installer). Toolchain: MSYS2 / UCRT64 / Qt 6.

## 1. Environment (once)

```
winget install --id MSYS2.MSYS2 -e
```

Then in the **MSYS2 UCRT64** shell:

```
pacman -S --needed mingw-w64-ucrt-x86_64-gcc mingw-w64-ucrt-x86_64-cmake mingw-w64-ucrt-x86_64-ninja mingw-w64-ucrt-x86_64-qt6-base mingw-w64-ucrt-x86_64-qt6-tools mingw-w64-ucrt-x86_64-qt6-translations mingw-w64-ucrt-x86_64-zlib mingw-w64-ucrt-x86_64-xz
```

## 2. Compile

From the repo root, in the MSYS2 UCRT64 shell:

```
cmake -S src -B build -G Ninja -DCMAKE_BUILD_TYPE=Release
cmake --build build
```

Produces `build/Win32DiskImager.exe`. It only runs inside the MSYS2 shell,
because its Qt DLLs are not next to it.

## 3. Package a standalone folder

```
bash deploy.sh
```

This creates `dist/`: the exe, the Qt DLLs/plugins/translations, and the MinGW
runtime plus Qt's third-party DLLs. `windeployqt6` does not copy that last
group on MSYS2, and the exe will not start without them.

Copy `dist/` anywhere and run `dist/Win32DiskImager.exe`. The app must be run
as Administrator to access raw devices.

## Working on the interface: a build without the UAC prompt

The shipped executable embeds a manifest asking for `requireAdministrator`, so
every launch raises a UAC prompt — tedious when the change under test is a
tooltip or the wording of a dialog. `-DTEST_NO_ADMIN=ON` swaps that manifest for
`asInvoker`:

```
cmake -S src -B build-test -G Ninja -DTEST_NO_ADMIN=ON
cmake --build build-test
```

The resulting exe starts with no prompt, which is the entire point of it. It
also **cannot open a device**, so anything past selecting a file is untestable:
use it for layout, tooltips, translations and dialog text, and a normal elevated
build for everything else.

CMake prints a warning at configure time, the option defaults to `OFF`, and both
deploy scripts refuse to package such a build — the two binaries are otherwise
indistinguishable once the exe sits in a folder of its own:

```
error: build/Win32DiskImager.exe was built with TEST_NO_ADMIN=ON and
       cannot write to a device. Reconfigure without it before packaging.
```

The option generates an `asInvoker` copy of `DiskImager.manifest` into the build
directory and points `DiskImager.rc` at it through `#ifdef TEST_NO_ADMIN`; the
tracked manifest is never modified. Use a separate build directory, as above, so
the option does not linger in the cache of the build you ship from. To check
which kind an executable is:

```
grep -ac 'level="asInvoker"' build/Win32DiskImager.exe    # 1 = test build, 0 = normal
```

## Cross-compiling for win64 from Linux

This is what [.github/workflows/build.yml](.github/workflows/build.yml) does on
every push; the steps below reproduce it locally.

Use Fedora. Debian and Ubuntu ship no MinGW Qt6 packages, so there is nothing
to link against there — the CI job runs `ubuntu-latest` but inside a
`fedora:44` container for exactly this reason.

```
dnf -y install cmake ninja-build mingw64-gcc-c++ \
  mingw64-qt6-qtbase mingw64-qt6-qttools mingw64-qt6-qttranslations \
  mingw64-zlib mingw64-xz \
  qt6-linguist
```

```
cmake -S src -B build -G Ninja \
  -DCMAKE_TOOLCHAIN_FILE=/usr/share/mingw/toolchain-mingw64.cmake \
  -DCMAKE_BUILD_TYPE=Release \
  -DLRELEASE_EXECUTABLE=/usr/bin/lrelease-qt6
cmake --build build
```

`LRELEASE_EXECUTABLE` matters: `Qt6::lrelease` from a MinGW Qt is a Windows
`.exe` and cannot run on the build host, so the *native* `lrelease-qt6` from
`qt6-linguist` compiles the translations instead. Leave it unset for a normal
Windows build. The same package provides `lupdate-qt6`, for the other half of
the translation workflow — see [Updating the translations](#updating-the-translations).

Then package:

```
bash tools/deploy-cross.sh build dist /usr/x86_64-w64-mingw32/sys-root/mingw
```

`windeployqt` is itself a Windows binary and cannot run here, so this script
copies the Qt plugins and translations by hand and resolves the DLL closure
with `x86_64-w64-mingw32-objdump -p`. It also strips the result, since Fedora
ships its MinGW DLLs unstripped (`libstdc++-6.dll` alone is ~26 MB otherwise).

Note the `.rc` file must reference the icon with a forward slash. Windows
`windres` accepts a backslash there; the cross build does not.

### In a container, without installing Fedora

If the host is not Fedora, run the same toolchain in a container. Build the
image once:

```
podman build -t w32di-build -f tools/Containerfile.build .
```

Then build any time with:

```
tools/build-cross.sh
```

which mounts the repo, configures on first use, and runs `cmake --build`. Pass
`clean` to drop `build/` and configure again. The script builds the image
itself if it is missing, so the `podman build` above is optional.

Everything it writes is gitignored — `build/` and the `lang/*.qm` files that
`lrelease` generates *into the source tree* (`translations.qrc` references them
relative to that directory). So the build directory persists between runs and
ninja stays incremental: a no-op rebuild is well under a second, a one-file
change around twenty seconds, a full build about thirty.

Configuring costs more than an incremental build, so the script only configures
when `build/CMakeCache.txt` is absent; ninja re-runs cmake on its own when
`CMakeLists.txt` changes.

This is worth running before a push. It is the same Fedora image, toolchain and
cmake invocation as CI, so it catches a broken cross build without waiting on
the workflow.

## Testing the GPT repair

`relocateBackupGPT()` rewrites partition tables and zeroes the stale backup left
mid-device, so a mistake there destroys data rather than merely misbehaving.
`tools/gpttest/` exercises it against a file standing in for a device — no SD
card, no VM, no UAC prompt, about a second per run:

```
cmake -S tools/gpttest -B build-gpttest -G Ninja
cmake --build build-gpttest
./build-gpttest/gpttest.exe
```

It exits non-zero if any check fails. The real `src/disk.cpp` is compiled into
the harness, so the code under test is the shipped code:
`rawSeekRead`/`rawSeekWrite` go through `ReadFile`/`WriteFile`, which behave the
same on a file handle as on a raw device. MSYS2 UCRT64 only — it is Win32 code.

Seven cases, 60 checks. Three relocate a backup stranded mid-device (the
ordinary `FirstUsableLBA` 34 layout, the reserved-space layout that the Windows
bug corrupts, and an image nearly filling the device) and check the CRCs, that
`FirstUsableLBA` and `PartitionEntryLBA` survive untouched, that the protective
MBR is extended, and that the stale copy is gone. The other four are the cases
where **nothing may be written**: a stale copy covered by a partition, a device
with no GPT, a backup already at the last LBA, and a corrupt primary header —
the last three check the device comes back byte-identical.

Those negative cases are the point. When changing the repair, confirm the
harness still fails when it should: break a guard on purpose, run it, see the
failure, then put it back. Removing the partition-overlap test in the stale
cleanup, for one, turns "data inside the partition was NOT zeroed" red.

## The scripts

Five of them, all `#!/usr/bin/env bash` with `set -euo pipefail`, all executable
in git, all resolving their own paths so they can be run from anywhere:

| script | where it runs | what it does |
| --- | --- | --- |
| `deploy.sh` | MSYS2 UCRT64 | packages `build/` into `dist/` |
| `tools/build-cross.sh` | Linux + podman | cross-builds in the Fedora container |
| `tools/deploy-cross.sh` | Fedora / the container | packages a cross build into a dist dir |
| `tools/lupdate-cross.sh` | Linux + podman | refreshes `src/lang/*.ts` |
| `tools/make-test-images.sh` | Linux | builds raw/gz/xz test images |
| `tools/verify-flashed.sh` | Linux | compares a flashed device against its image |

The two deploy scripts are deliberately parallel: same files copied into the
dist folder, same Qt plugin groups, same trimming of Qt's translations, same
iterate-until-stable DLL closure, and the same refusal to package a
`TEST_NO_ADMIN` build. They differ only where the platform forces it —
`windeployqt6` and `ntldd` on Windows against a hand-rolled copy and
`objdump -p` on Linux, and `deploy.sh` empties `dist/` in place rather than
deleting it because Explorer locks the directory node on Windows.

The list of shipped languages lives in `src/CMakeLists.txt` and both deploy
scripts read it from there, so trimming Qt's translations cannot drift from the
set the build actually compiles. Add a language in one place:

```
set(LANGUAGES es it pl nl de fr zh_CN zh_TW ta_IN ko ja)
```

## Updating the translations

`src/lang/*.ts` hold the translations; the build compiles them to `.qm` with
`lrelease` and embeds those through `translations.qrc`. Adding or changing a
`tr()` string in the source does not reach the `.ts` files on its own — until
they are refreshed, the new string simply falls back to English at runtime, and
a *reworded* string silently loses the translation it used to have.

Refresh them with:

```
tools/lupdate-cross.sh              # every language
tools/lupdate-cross.sh de fr        # only those
```

This runs the native `lupdate-qt6` from the same container the cross build
uses, so it needs no Qt on the host. Unlike `tools/build-cross.sh`, it rewrites
files that are tracked in git, so review the diff:

- **New strings** arrive as `<translation type="unfinished"></translation>`.
  `lrelease` skips them and the app shows English until someone fills them in.
- **Reworded strings** turn up as a new unfinished entry, while the old
  translation is kept next to it as `type="vanished"` rather than deleted — so
  a translator can see what the wording was and adapt it. Nothing is lost.
- Most of the diff is `<location>` line numbers moving with the source. That
  churn is expected and harmless.

Run it before a release, or after any commit that touches user-visible text.
