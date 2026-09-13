#!/usr/bin/env bash
# Cross-compile a win64 binary on a Fedora host, using the toolchain installed
# there directly. No container.
#
#   tools/build-cross.sh              # configure (if needed) and build
#   tools/build-cross.sh clean        # drop the build dir first
#   tools/build-cross.sh test         # a binary that asks for no elevation
#   tools/build-cross.sh test clean
#
# Install the toolchain once with:
#   sudo bash tools/build-env.sh install
#
# tools/build-container.sh does the same build inside the Fedora container,
# which is what CI uses and what a non-Fedora host needs. Both are driven by
# tools/build-env.sh, so they compile with the same toolchain and flags.
#
# They build into different directories on purpose. A cmake cache records the
# absolute path it was generated for, and the container sees this tree as /src,
# so sharing one directory would make every switch between them fail.
# Override with BUILD_DIR=... if you want somewhere else.
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
    # Asks for no elevation, so it starts without a UAC prompt -- and cannot
    # open a device either. For working on the interface, never for shipping.
    extra=(-DTEST_NO_ADMIN=ON)
else
    extra=(-DTEST_NO_ADMIN=OFF)
fi

command -v cmake >/dev/null 2>&1 || { echo "error: cmake not found" >&2; exit 1; }
if ! cross_check; then
    echo "error: the MinGW cross toolchain is not installed, or not where the" >&2
    echo "       build expects it. On Fedora:" >&2
    echo "         sudo bash tools/build-env.sh install" >&2
    echo "       On any other distribution there are no MinGW Qt6 packages," >&2
    echo "       so use the container instead: tools/build-container.sh" >&2
    exit 1
fi

[ "$clean" = 1 ] && rm -rf "$build"
# build/ is shared with the native build and the container, which sees this tree
# as /src. A cache from either of those is no use here.
drop_foreign_cache "$build" "$CROSS_TOOLCHAIN"

# Configuring costs far more than an incremental build, so only do it when there
# is no cache yet; ninja re-runs cmake itself when CMakeLists.txt changes. The
# -D above is passed either way, so a mode switch reconfigures on its own.
if [ ! -f "$build/CMakeCache.txt" ]; then
    cross_configure "$REPO/src" "$build" "${extra[@]}"
else
    cmake -S "$REPO/src" -B "$build" "${extra[@]}" >/dev/null
fi
cmake --build "$build"

# A host compiler picked up by mistake produces an ELF binary that looks like a
# successful build until someone tries to run it. Check here, so every route
# into this script is covered rather than only CI.
if ! file "$build/Win32DiskImager.exe" | grep -q 'PE32+'; then
    echo "error: $build/Win32DiskImager.exe is not a win64 PE binary:" >&2
    file "$build/Win32DiskImager.exe" >&2
    exit 1
fi
file "$build/Win32DiskImager.exe"

echo
echo "built $build/Win32DiskImager.exe"
if [ "$mode" = test ]; then
    echo "this build asks for no elevation and CANNOT write to a device"
else
    echo "package it with: tools/deploy-cross.sh ${build#"$REPO"/} dist"
fi
