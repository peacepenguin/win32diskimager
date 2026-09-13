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
# All three build into build/. A cmake cache records the absolute path it was
# generated for, and the container sees this tree as /src, so a cache left by
# one of them is useless to the others -- build_prepare drops it rather than
# letting the build fail confusingly. Override with BUILD_DIR=... for a build
# you want kept aside.
#
# Copyright (C) 2026 peacepenguin, GPL-2.0-or-later.
set -euo pipefail

REPO=$(cd "$(dirname "$0")/.." && pwd)
. "$REPO/tools/build-env.sh"

build_parse_args "$@" || { echo "usage: ${0##*/} [test] [clean]" >&2; exit 2; }
build=${BUILD_DIR:-$REPO/build}

command -v cmake >/dev/null 2>&1 || { echo "error: cmake not found" >&2; exit 1; }
if ! cross_check; then
    echo "error: the MinGW cross toolchain is not installed, or not where the" >&2
    echo "       build expects it. On Fedora:" >&2
    echo "         sudo bash tools/build-env.sh install" >&2
    echo "       On any other distribution there are no MinGW Qt6 packages," >&2
    echo "       so use the container instead: tools/build-container.sh" >&2
    exit 1
fi

build_prepare "$build" "$CROSS_TOOLCHAIN"
build_run "$REPO/src" "$build" cross_configure

# A host compiler picked up by mistake produces an ELF binary that looks like a
# successful build until someone tries to run it. Check here, so every route
# into this script is covered rather than only CI.
if ! file "$build/Win32DiskImager.exe" | grep -q 'PE32+'; then
    echo "error: $build/Win32DiskImager.exe is not a win64 PE binary:" >&2
    file "$build/Win32DiskImager.exe" >&2
    exit 1
fi
file "$build/Win32DiskImager.exe"

if [ "$build" = "$REPO/build" ]; then
    build_report "$build" "tools/deploy-cross.sh"
else
    # BUILD_DIR was overridden, so the defaults would not find this build.
    build_report "$build" "tools/deploy-cross.sh $build dist"
fi
