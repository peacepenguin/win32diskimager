#!/usr/bin/env bash
# Refresh src/lang/*.ts from the current sources, in the Fedora container.
#
#   tools/lupdate-cross.sh           # update every .ts in src/lang
#   tools/lupdate-cross.sh de fr     # only those languages
#
# lupdate is a *native* tool, from qt6-linguist, the same package that provides
# the lrelease the cross build uses. It is not part of the MinGW Qt: Qt6::lupdate
# from mingw64-qt6-qttools is a Windows .exe and cannot run on the build host.
#
# Unlike the build, this rewrites files that are tracked in git. New strings
# arrive untranslated, and strings that no longer appear in the source are
# marked "vanished" rather than deleted, so translators keep their history.
# Review the diff before committing.
set -euo pipefail

IMAGE=${IMAGE:-w32di-build}
REPO=$(cd "$(dirname "$0")/.." && pwd)

if ! podman image exists "$IMAGE"; then
    echo "building $IMAGE (one time)..."
    podman build -t "$IMAGE" -f "$REPO/tools/Containerfile.build" "$REPO"
fi

# Named languages, or all of them.
if [ "$#" -gt 0 ]; then
    for lang in "$@"; do
        test -f "$REPO/src/lang/diskimager_$lang.ts" \
            || { echo "error: no src/lang/diskimager_$lang.ts" >&2; exit 1; }
    done
    TSFILES=$(for lang in "$@"; do printf 'lang/diskimager_%s.ts ' "$lang"; done)
else
    TSFILES=""
fi

podman run --rm -v "$REPO:/src" -e TSFILES="$TSFILES" "$IMAGE" bash -c '
set -e
# Run from src/ so the <location> paths lupdate writes stay relative to the .ts
# files the way the existing ones are ("../mainwindow.ui").
cd /src/src
[ -n "$TSFILES" ] || TSFILES=$(ls lang/*.ts)
lupdate-qt6 -locations relative *.cpp *.h *.ui -ts $TSFILES
'
