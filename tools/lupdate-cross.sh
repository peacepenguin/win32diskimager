#!/usr/bin/env bash
# Refresh src/lang/*.ts from the current sources.
#
#   tools/lupdate-cross.sh           # update every .ts in src/lang
#   tools/lupdate-cross.sh de fr     # only those languages
#
# Runs lupdate directly when the toolchain is installed on this host, and in the
# Fedora container when it is not — the same pair of routes as building, and the
# same result either way.
#
# lupdate is a *native* tool, from qt6-linguist, the same package that provides
# the lrelease the cross build uses. It is not part of the MinGW Qt: Qt6::lupdate
# from mingw64-qt6-qttools is a Windows .exe and cannot run on the build host.
#
# Unlike the build, this rewrites files that are tracked in git. New strings
# arrive untranslated, and strings that no longer appear in the source are
# marked "vanished" rather than deleted, so translators keep their history.
# Review the diff before committing.
#
# Copyright (C) 2026 peacepenguin, GPL-2.0-or-later.
set -euo pipefail

REPO=$(cd "$(dirname "$0")/.." && pwd)
. "$REPO/tools/build-env.sh"

# Named languages, or all of them. Checked here so a typo fails immediately
# rather than after a container has started.
for lang in "$@"; do
    [ -f "$REPO/src/lang/diskimager_$lang.ts" ] \
        || { echo "error: no src/lang/diskimager_$lang.ts" >&2; exit 1; }
done

if [ ! -x "$CROSS_LUPDATE" ]; then
    # Inside the container this means the image is broken; going round again
    # would only loop.
    if [ -n "${W32DI_IN_CONTAINER:-}" ]; then
        echo "error: $CROSS_LUPDATE missing inside the container image." >&2
        echo "       Rebuild it: podman build -t $CROSS_IMAGE -f tools/Containerfile.build ." >&2
        exit 1
    fi
    container_run "$REPO" /src/tools/lupdate-cross.sh "$@"
    exit $?
fi

TSFILES=()
if [ "$#" -gt 0 ]; then
    for lang in "$@"; do
        TSFILES+=("lang/diskimager_$lang.ts")
    done
else
    for ts in "$REPO"/src/lang/*.ts; do
        TSFILES+=("lang/$(basename "$ts")")
    done
fi

# Run from src/ so the <location> paths lupdate writes stay relative to the .ts
# files the way the existing ones are ("../mainwindow.ui").
cd "$REPO/src"
"$CROSS_LUPDATE" -locations relative *.cpp *.h *.ui -ts "${TSFILES[@]}"
