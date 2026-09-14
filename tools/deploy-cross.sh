#!/usr/bin/env bash
# Assemble a standalone Windows folder from a cross-compiled build.
#
#   tools/deploy-cross.sh                          # build/ -> dist/
#   tools/deploy-cross.sh <build-dir> <dist-dir> [mingw-sysroot]
#
# windeployqt is itself a Windows binary and cannot run on the build host, so
# the Qt DLLs, plugins and translations are gathered by hand and the dependency
# closure is resolved with objdump.
set -euo pipefail

root="$(cd "$(dirname "$0")/.." && pwd)"
# Toolchain paths shared with the build scripts, CI and the container image.
. "$root/tools/build-env.sh"

# The same two directories every build script uses, so the common case needs no
# arguments. They are still overridable, positionally, for a build kept aside.
build=${1:-$root/build}
dist=${2:-$root/dist}
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
cp "$root"/Changelog.txt "$root"/README.md "$root"/License.txt "$root"/THIRD-PARTY-NOTICES.txt "$root"/GPL-2 "$root"/LGPL-2.1 "$dist/"

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

# Checked rather than assumed, for the reason the strip check below gives: a
# missing lister is silent. The closure would find nothing, no runtime DLL
# would be copied, and the folder would be reported ready around an executable
# that cannot start.
command -v "$objdump" >/dev/null 2>&1 || {
    echo "error: $objdump not found, so the DLLs the build depends on could not" >&2
    echo "       be resolved. It comes with binutils; see tools/build-env.sh." >&2
    exit 1
}

# Resolve the DLL closure: scan every binary already in dist, copy in anything
# it imports that exists in the sysroot, and repeat until nothing new appears.
# Shared with the native build; see deploy_resolve_closure in tools/build-env.sh.
deploy_resolve_closure "$objdump" "$bin" "$dist"

# Fedora ships its MinGW DLLs unstripped; libstdc++ alone is ~26 MB of debug
# symbols that do nothing in a shipped build.
#
# The tool is checked for rather than assumed: hiding a missing strip behind
# 2>/dev/null shipped the symbols anyway while this comment claimed they were
# gone. Errors from individual files stay visible now, for the same reason.
strip_tool=${STRIP:-x86_64-w64-mingw32-strip}
command -v "$strip_tool" >/dev/null 2>&1 || {
    echo "error: $strip_tool not found, so the package would ship its debug" >&2
    echo "       symbols. It comes with binutils; see tools/build-env.sh." >&2
    exit 1
}
find "$dist" \( -name '*.dll' -o -name '*.exe' \) \
    -exec "$strip_tool" --strip-unneeded {} +

echo "dist: $(find "$dist" -type f | wc -l) files, $(du -sh "$dist" | cut -f1)"
