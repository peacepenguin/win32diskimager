# Build

Binary-only build (no installer). Toolchain: MSYS2 / UCRT64 / Qt 6.

## 1. Environment (once)

```
winget install --id MSYS2.MSYS2 -e
```

Then in the **MSYS2 UCRT64** shell:

```
pacman -S --needed mingw-w64-ucrt-x86_64-gcc mingw-w64-ucrt-x86_64-cmake mingw-w64-ucrt-x86_64-ninja mingw-w64-ucrt-x86_64-qt6-base mingw-w64-ucrt-x86_64-qt6-tools mingw-w64-ucrt-x86_64-qt6-translations
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

## Cross-compiling for win64 from Linux

This is what [.github/workflows/build.yml](.github/workflows/build.yml) does on
every push; the steps below reproduce it locally.

Use Fedora. Debian and Ubuntu ship no MinGW Qt6 packages, so there is nothing
to link against there — the CI job runs `ubuntu-latest` but inside a
`fedora:44` container for exactly this reason.

```
dnf -y install cmake ninja-build mingw64-gcc-c++ \
  mingw64-qt6-qtbase mingw64-qt6-qttools mingw64-qt6-qttranslations \
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
Windows build.

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
