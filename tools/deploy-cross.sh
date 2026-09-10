#!/usr/bin/env bash
# Assemble a standalone Windows folder from a cross-compiled build.
#
#   tools/deploy-cross.sh <build-dir> <dist-dir> <mingw-sysroot>
#
# windeployqt is itself a Windows binary and cannot run on the build host, so
# the Qt DLLs, plugins and translations are gathered by hand and the dependency
# closure is resolved with objdump.
set -euo pipefail

build=${1:?usage: deploy-cross.sh <build-dir> <dist-dir> <mingw-sysroot>}
dist=${2:?}
sysroot=${3:?}
objdump=${OBJDUMP:-x86_64-w64-mingw32-objdump}

root="$(cd "$(dirname "$0")/.." && pwd)"
bin="$sysroot/bin"

rm -rf "$dist"
mkdir -p "$dist"
cp "$build/Win32DiskImager.exe" "$dist/"
cp "$root"/Changelog.txt "$root"/README.md "$root"/License.txt "$root"/GPL-2 "$root"/LGPL-2.1 "$dist/"

# Qt plugins. Only the ones a widgets app on Windows actually loads.
qtplugins="$sysroot/lib/qt6/plugins"
[ -d "$qtplugins" ] || qtplugins="$sysroot/share/qt6/plugins"
for group in platforms styles imageformats generic; do
    if [ -d "$qtplugins/$group" ]; then
        mkdir -p "$dist/$group"
        cp "$qtplugins/$group"/*.dll "$dist/$group/" 2>/dev/null || true
    fi
done
# Debug variants would double the size for nothing.
find "$dist" -name '*d.dll' -delete 2>/dev/null || true
# qminimal/qoffscreen are headless platform plugins; useless in a shipped GUI.
rm -f "$dist/platforms/qminimal.dll" "$dist/platforms/qoffscreen.dll"

# Qt's own translations, trimmed to the languages the app ships.
LANGUAGES="es it pl nl de fr zh_CN zh_TW ta_IN ko ja"
qttr="$sysroot/share/qt6/translations"
if [ ! -d "$qttr" ]; then
    echo "error: no Qt translations at $qttr (is mingw64-qt6-qttranslations installed?)" >&2
    exit 1
fi
mkdir -p "$dist/translations"
for l in $LANGUAGES; do
    cp "$qttr/qt_$l.qm" "$dist/translations/" 2>/dev/null || true
    cp "$qttr/qtbase_$l.qm" "$dist/translations/" 2>/dev/null || true
done
if [ -z "$(ls -A "$dist/translations")" ]; then
    echo "error: $qttr contained none of the expected qt_*.qm files" >&2
    exit 1
fi

# Resolve the DLL closure: scan every binary already in dist, copy in anything
# it imports that exists in the sysroot, and repeat until nothing new appears.
while :; do
    before=$(find "$dist" -name '*.dll' | wc -l)
    while read -r f; do
        "$objdump" -p "$f" 2>/dev/null | awk '/DLL Name:/ {print $3}'
    done < <(find "$dist" -name '*.exe' -o -name '*.dll') | sort -u | while read -r dll; do
        if [ ! -f "$dist/$dll" ] && [ -f "$bin/$dll" ]; then
            cp "$bin/$dll" "$dist/"
        fi
    done
    [ "$(find "$dist" -name '*.dll' | wc -l)" -eq "$before" ] && break
done

# Fedora ships its MinGW DLLs unstripped; libstdc++ alone is ~26 MB of debug
# symbols that do nothing in a shipped build.
find "$dist" \( -name '*.dll' -o -name '*.exe' \) \
    -exec "${STRIP:-x86_64-w64-mingw32-strip}" --strip-unneeded {} + 2>/dev/null || true

echo "dist: $(find "$dist" -type f | wc -l) files, $(du -sh "$dist" | cut -f1)"
