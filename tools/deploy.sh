#!/usr/bin/env bash
# Package build/Win32DiskImager.exe into a self-contained dist/ folder.
# Run from the repo root in the MSYS2 UCRT64 shell.
set -euo pipefail

root="$(cd "$(dirname "$0")/.." && pwd)"
cd "$root"
# The same file the cross build reads. This script is the one that did not, and
# so was the one with no check that its tools were installed.
. "$root/tools/build-env.sh"

# windeployqt6 brings the Qt payload; objdump resolves everything it leaves out.
# Neither was checked for before, and a missing one is silent: the closure finds
# nothing, no runtime DLLs are copied, and the folder is reported ready with an
# executable in it that cannot start.
need_msys2_tools windeployqt6 objdump

# Qt's DLLs and the MinGW runtime sit beside the tools, so the prefix is asked
# for rather than written down.
MSYS2_BIN=$(dirname "$(command -v objdump)")

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

# Qt6Network is not left out, because it cannot be. generic/qtuiotouchplugin.dll
# links it, and windeployqt says as much while deploying: "Adding Qt6Network for
# qtuiotouchplugin.dll from plugin type: generic". Deleting the DLL here achieved
# nothing -- the dependency scan at the end of this script reads the same import
# table and copies it straight back in -- so it is left where the tooling put it.
# The cross build ends up with it for the same reason.
#
# Its plugins are a different matter. Nothing imports them: they are loaded by
# name at runtime, which is why the scan below never asks for them and these
# deletions do stick. No code path in the app opens a socket.
rm -rf dist/tls dist/networkinformation

# ${f##*/} rather than basename: identical answer, no process. basename sat in
# the inner loop, so trimming 32 catalogues against 11 languages spawned it 352
# times -- 4.6 seconds of the run, measured, for a string operation the shell
# does for nothing.
for f in dist/translations/*.qm; do
    keep=""
    name=${f##*/}
    for l in $LANGUAGES; do
        case "$name" in *_"$l".qm) keep=1;; esac
    done
    [ -n "$keep" ] || rm -f "$f"
done

# The same post-condition the cross script keeps on its own copy of this step.
# windeployqt decides by itself what to write here and under what names -- it
# merges Qt's per-module catalogues into one qt_<lang>.qm -- so a change on its
# side, or to the trim above, could empty this directory while everything else
# in this script still succeeded. The package would look right, start fine, and
# have every Qt-supplied string in it silently in English.
#
# Emptiness is the test, not one file per language: Qt ships no translation at
# all for some of the languages the app itself covers (ta_IN among them), and
# those gaps are normal.
if [ -z "$(ls -A dist/translations 2>/dev/null)" ]; then
    echo "error: no Qt translations in dist/translations." >&2
    echo "       windeployqt writes them and the loop above trims them to" >&2
    echo "       LANGUAGES; see its output above for what it actually did." >&2
    exit 1
fi

# windeployqt does not pull in the MinGW runtime or Qt's third-party
# dependencies on MSYS2, so resolve them ourselves. Plugins in the
# subdirectories have dependencies of their own, so everything is scanned and
# the pass repeated until no new DLLs appear. Shared with the cross build; see
# deploy_resolve_closure in tools/build-env.sh.
deploy_resolve_closure objdump "$MSYS2_BIN" dist

echo "dist/ is ready ($(du -sh dist | cut -f1))"
