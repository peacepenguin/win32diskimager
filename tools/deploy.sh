#!/usr/bin/env bash
# Package build/Win32DiskImager.exe into a self-contained dist/ folder.
# Run from the repo root in the MSYS2 UCRT64 shell.
set -euo pipefail

root="$(cd "$(dirname "$0")/.." && pwd)"
cd "$root"

[ -f build/Win32DiskImager.exe ] || {
    echo "error: no build/Win32DiskImager.exe -- build it first" >&2
    exit 1
}

# A build made with -DTEST_NO_ADMIN=ON asks for no elevation and cannot open a
# device for writing. It is for looking at the GUI, never for shipping, and the
# difference is invisible once the exe is in a folder of its own.
if grep -aq 'level="asInvoker"' build/Win32DiskImager.exe; then
    echo "error: build/Win32DiskImager.exe was built with TEST_NO_ADMIN=ON and" >&2
    echo "       cannot write to a device. Reconfigure without it before packaging." >&2
    exit 1
fi

# Empty dist rather than deleting it: on Windows an open Explorer window or a
# shell sitting in the directory locks the directory node itself, while its
# contents still delete fine.
mkdir -p dist
rm -rf dist/* dist/.[!.]* 2>/dev/null || true
cp build/Win32DiskImager.exe dist/
cp Changelog.txt README.md License.txt THIRD-PARTY-NOTICES.txt GPL-2 LGPL-2.1 dist/

# Languages the app itself ships translations for; Qt's own translations are
# trimmed to match instead of shipping all ~40 of them. Read from CMakeLists so
# this list cannot drift from the one the build compiles.
LANGUAGES=$(sed -n 's/^set(LANGUAGES \(.*\))$/\1/p' src/CMakeLists.txt)
[ -n "$LANGUAGES" ] || { echo "error: no LANGUAGES in src/CMakeLists.txt" >&2; exit 1; }

# Qt DLLs, plugins and translations. The app is offline and 2D-only, so skip
# the networking and software-OpenGL payloads windeployqt adds by default.
(cd dist && windeployqt6 --release \
    --no-opengl-sw \
    --no-system-d3d-compiler \
    Win32DiskImager.exe)

# Nothing in the app links Qt6Network; windeployqt ships it speculatively.
rm -rf dist/Qt6Network.dll dist/tls dist/networkinformation

for f in dist/translations/*.qm; do
    keep=""
    for l in $LANGUAGES; do
        case "$(basename "$f")" in *_"$l".qm) keep=1;; esac
    done
    [ -n "$keep" ] || rm -f "$f"
done

# windeployqt does not pull in the MinGW runtime or Qt's third-party
# dependencies on MSYS2, so resolve them ourselves. Plugins in the
# subdirectories have dependencies of their own, so scan everything and
# repeat until no new DLLs appear.
while :; do
    before=$(find dist -name '*.dll' | wc -l)
    for f in $(find dist -name '*.exe' -o -name '*.dll'); do
        ntldd -R "$f" 2>/dev/null | awk '/ucrt64/ {print $3}'
    done | sort -u | while read -r dll; do
        [ -f "dist/$(basename "$dll")" ] || cp "$dll" dist/
    done
    [ "$(find dist -name '*.dll' | wc -l)" -eq "$before" ] && break
done

echo "dist/ is ready ($(du -sh dist | cut -f1))"
