#!/usr/bin/env bash
# Package build/Win32DiskImager.exe into a self-contained dist/ folder.
# Run from the repo root in the MSYS2 UCRT64 shell.
set -euo pipefail

root="$(cd "$(dirname "$0")" && pwd)"
cd "$root"

rm -rf dist
mkdir -p dist
cp build/Win32DiskImager.exe dist/
cp Changelog.txt README.md License.txt GPL-2 LGPL-2.1 dist/

# Languages the app itself ships translations for; Qt's own translations are
# trimmed to match instead of shipping all ~40 of them.
LANGUAGES="es it pl nl de fr zh_CN zh_TW ta_IN ko ja"

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
