#!/usr/bin/env bash
# Build the app natively on Windows, in the MSYS2 UCRT64 shell.
#
#   tools/build.sh              # configure (if needed) and build into build/
#   tools/build.sh clean        # drop the build dir first
#   tools/build.sh test         # build-test/, a binary that asks for no
#                               # elevation, for working on the interface
#   tools/build.sh test clean
#
# Package what this produces with tools/deploy.sh. To build for Windows from
# Linux instead: tools/build-cross.sh on a Fedora host, or
# tools/build-container.sh anywhere podman runs.
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

build=${BUILD_DIR:-$REPO/build}
# Always stated, never left to whatever the cache happens to hold, so switching
# between a normal and a test build is just a matter of the argument.
if [ "$mode" = test ]; then
    # A build that asks for no elevation: no UAC prompt on every launch, and no
    # ability to open a device either.
    extra=(-DTEST_NO_ADMIN=ON)
else
    extra=(-DTEST_NO_ADMIN=OFF)
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
# build/ is shared with the cross build and the container; a cache from either
# of those is no use here. A native build uses no toolchain file.
drop_foreign_cache "$build" ""

# Configuring costs far more than an incremental build, so only do it when there
# is no cache yet; ninja re-runs cmake itself when CMakeLists.txt changes. The
# -D above is passed either way, so a mode switch reconfigures on its own.
if [ ! -f "$build/CMakeCache.txt" ]; then
    cmake -S "$REPO/src" -B "$build" -G Ninja -DCMAKE_BUILD_TYPE=Release "${extra[@]}"
else
    cmake -S "$REPO/src" -B "$build" "${extra[@]}" >/dev/null
fi
cmake --build "$build"

echo
echo "built $build/Win32DiskImager.exe"
if [ "$mode" = test ]; then
    echo "this build asks for no elevation and CANNOT write to a device"
else
    echo "package it with: tools/deploy.sh"
fi
