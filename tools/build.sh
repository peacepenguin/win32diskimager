#!/usr/bin/env bash
# Build the app natively on Windows, in the MSYS2 UCRT64 shell.
#
#   tools/build.sh              # configure (if needed) and build into build/
#   tools/build.sh clean        # drop the build dir first
#   tools/build.sh test         # build-test/, a binary that asks for no
#                               # elevation, for working on the interface
#   tools/build.sh test clean
#
# The cross-compiled counterpart is tools/build-cross.sh. Package what this
# produces with tools/deploy.sh.
#
# Copyright (C) 2026 peacepenguin, GPL-2.0-or-later.
set -euo pipefail

REPO=$(cd "$(dirname "$0")/.." && pwd)
. "$REPO/tools/build-env.sh"

mode=""
clean=0
for arg in "$@"; do
    case "$arg" in
        test)  mode=test ;;
        clean) clean=1 ;;
        *) echo "usage: ${0##*/} [test] [clean]" >&2; exit 2 ;;
    esac
done

if [ "$mode" = test ]; then
    # A build that asks for no elevation: no UAC prompt on every launch, and no
    # ability to open a device either. Kept in its own directory so the option
    # cannot linger in the cache of the build that gets shipped.
    build="$REPO/build-test"
    extra=(-DTEST_NO_ADMIN=ON)
else
    build="$REPO/build"
    extra=()
fi

for tool in cmake ninja g++ windeployqt6; do
    command -v "$tool" >/dev/null 2>&1 || {
        echo "error: $tool not found. In the MSYS2 UCRT64 shell, install the" >&2
        echo "       toolchain with:" >&2
        echo "         pacman -S --needed \$(bash tools/build-env.sh packages-msys2)" >&2
        exit 1
    }
done

[ "$clean" = 1 ] && rm -rf "$build"

# Configuring costs far more than an incremental build, so only do it when there
# is no cache yet; ninja re-runs cmake itself when CMakeLists.txt changes.
if [ ! -f "$build/CMakeCache.txt" ]; then
    cmake -S "$REPO/src" -B "$build" -G Ninja -DCMAKE_BUILD_TYPE=Release "${extra[@]}"
fi
cmake --build "$build"

echo
echo "built $build/Win32DiskImager.exe"
if [ "$mode" = test ]; then
    echo "this build asks for no elevation and CANNOT write to a device"
else
    echo "package it with: tools/deploy.sh"
fi
