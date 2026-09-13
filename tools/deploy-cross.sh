#!/usr/bin/env bash
# Assemble a standalone Windows folder from a cross-compiled build.
#
#   tools/deploy-cross.sh <build-dir> <dist-dir> <mingw-sysroot>
#
# windeployqt is itself a Windows binary and cannot run on the build host, so
# the Qt DLLs, plugins and translations are gathered by hand and the dependency
# closure is resolved with objdump.
set -euo pipefail

build=${1:?usage: deploy-cross.sh <build-dir> <dist-dir> [mingw-sysroot]}
dist=${2:?}

root="$(cd "$(dirname "$0")/.." && pwd)"
# Toolchain paths shared with the build scripts, CI and the container image.
. "$root/tools/build-env.sh"

sysroot=${3:-$CROSS_SYSROOT}
objdump=${OBJDUMP:-x86_64-w64-mingw32-objdump}
bin="$sysroot/bin"

[ -f "$build/Win32DiskImager.exe" ] || {
    echo "error: no $build/Win32DiskImager.exe -- build it first" >&2
    exit 1
}

# A build made with -DTEST_NO_ADMIN=ON asks for no elevation and cannot open a
# device for writing. It is for looking at the GUI, never for shipping, and the
# difference is invisible once the exe is in a folder of its own.
if grep -aq 'level="asInvoker"' "$build/Win32DiskImager.exe"; then
    echo "error: $build/Win32DiskImager.exe was built with TEST_NO_ADMIN=ON and" >&2
    echo "       cannot write to a device. Reconfigure without it before packaging." >&2
    exit 1
fi

rm -rf "$dist"
mkdir -p "$dist"
cp "$build/Win32DiskImager.exe" "$dist/"
cp "$root"/Changelog.txt "$root"/README.md "$root"/License.txt "$root"/GPL-2 "$root"/LGPL-2.1 "$dist/"

# Qt plugins. Only the ones a widgets app on Windows actually loads.
qtplugins="$sysroot/lib/qt6/plugins"
[ -d "$qtplugins" ] || qtplugins="$sysroot/share/qt6/plugins"
for group in platforms styles imageformats iconengines generic; do
    if [ -d "$qtplugins/$group" ]; then
        mkdir -p "$dist/$group"
        cp "$qtplugins/$group"/*.dll "$dist/$group/" 2>/dev/null || true
    fi
done
# Debug variants of the plugins would double the size for nothing. Scoped to
# the plugin directories: at the top level "*d.dll" would also match innocent
# names such as libzstd.dll.
for group in platforms styles imageformats iconengines generic; do
    [ -d "$dist/$group" ] && find "$dist/$group" -name '*d.dll' -delete 2>/dev/null
done
true
# qminimal/qoffscreen are headless platform plugins; useless in a shipped GUI.
rm -f "$dist/platforms/qminimal.dll" "$dist/platforms/qoffscreen.dll"

# Qt's own translations, trimmed to the languages the app ships. Read from
# CMakeLists so this list cannot drift from the one the build compiles.
LANGUAGES=$(sed -n 's/^set(LANGUAGES \(.*\))$/\1/p' "$root/src/CMakeLists.txt")
[ -n "$LANGUAGES" ] || { echo "error: no LANGUAGES in src/CMakeLists.txt" >&2; exit 1; }
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
