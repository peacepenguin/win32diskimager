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

if [ "$mode" = test ]; then
    # Asks for no elevation, so it starts without a UAC prompt -- and cannot
    # open a device either. For working on the interface, never for shipping.
    build=${BUILD_DIR:-$REPO/build-cross-test}
    extra=(-DTEST_NO_ADMIN=ON)
else
    build=${BUILD_DIR:-$REPO/build-cross}
    extra=()
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

# Configuring costs far more than an incremental build, so only do it when there
# is no cache yet; ninja re-runs cmake itself when CMakeLists.txt changes.
if [ ! -f "$build/CMakeCache.txt" ]; then
    cross_configure "$REPO/src" "$build" "${extra[@]}"
fi
cmake --build "$build"

file "$build/Win32DiskImager.exe"
echo
echo "built $build/Win32DiskImager.exe"
if [ "$mode" = test ]; then
    echo "this build asks for no elevation and CANNOT write to a device"
else
    echo "package it with: tools/deploy-cross.sh ${build#"$REPO"/} dist"
fi
