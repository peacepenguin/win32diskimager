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
