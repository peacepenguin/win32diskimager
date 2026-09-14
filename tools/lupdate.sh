#!/usr/bin/env bash
# Refresh src/lang/*.ts from the current sources.
#
#   tools/lupdate.sh           # update every .ts in src/lang
#   tools/lupdate.sh de fr     # only those languages
#
# Runs anywhere: it uses whichever Qt 6 lupdate the host has -- plain "lupdate"
# in MSYS2 UCRT64, "lupdate-qt6" from Fedora's qt6-linguist -- and falls back to
# running itself in the Fedora container when the host has neither. Only version
# 6 is accepted; a Qt 5 lupdate writes .ts files the Qt 6 build then has to
# interpret.
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

if ! LUPDATE=$(lupdate_path); then
    # Inside the container this means the image is broken; going round again
    # would only loop.
    if [ -n "${W32DI_IN_CONTAINER:-}" ]; then
        echo "error: no Qt 6 lupdate inside the container image." >&2
        echo "       Rebuild it: podman build -t $CROSS_IMAGE -f tools/Containerfile.build ." >&2
        exit 1
    fi
    # Nothing local, so try the container. On Windows there is none, and the
    # tool is one package away.
    if ! command -v podman >/dev/null 2>&1; then
        echo "error: no Qt 6 lupdate on this host." >&2
        echo "       MSYS2 UCRT64:  pacman -S --needed mingw-w64-ucrt-x86_64-qt6-tools" >&2
        echo "       Fedora:        sudo bash tools/build-env.sh install" >&2
        echo "       Anywhere else: install podman and this will use a container." >&2
        exit 1
    fi
    container_run "$REPO" /src/tools/lupdate.sh "$@"
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

# Run from src/ so the source files are found by the plain names below.
cd "$REPO/src"

# -locations none: the .ts files record no line numbers.
#
# They are only ever read by a person in Linguist, to see where a string comes
# from. Nothing in the build wants them: lrelease ignores them, and a .ts
# stripped of every <location> compiles to a byte-identical .qm. What they cost
# is the history -- adding one string moved the line numbers under every string
# after it, so a one-line change arrived as a hundred-line diff in each of
# twelve files and the real change had to be hunted for.
#
# Matching is by context and source text, not by position, so existing
# translations survive the change and later runs still find them.
#
# Anyone who does want the source references can have them for a look without
# committing them:  lupdate -locations relative *.cpp *.h *.ui -ts lang/foo.ts
"$LUPDATE" -locations none *.cpp *.h *.ui -ts "${TSFILES[@]}"
