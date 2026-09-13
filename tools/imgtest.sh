#!/usr/bin/env bash
# Build and run the image-decoder tests.
#
#   tools/imgtest.sh              # build (if needed) and run
#   tools/imgtest.sh clean        # drop the build dir first
#
# Exits non-zero if any check fails, so it can gate a commit or a release.
#
# The sources are in tools/imgtest/. They compile the real src/imagesource.cpp
# into a harness that reads images back and compares them with what went in --
# raw, gzip, xz, multi-member gzip, multi-stream xz, stream padding, a size that
# is not a whole number of sectors, and files that stop in the middle. The
# fixtures are built by the harness with zlib and liblzma, so nothing compressed
# is checked in and neither gzip nor xz has to be on the path.
#
# Its own build directory, not build/, because it is a different cmake project
# and would otherwise fight the application's cache.
#
# Copyright (C) 2026 peacepenguin, GPL-2.0-or-later.
set -euo pipefail

REPO=$(cd "$(dirname "$0")/.." && pwd)
build="$REPO/build-imgtest"

case "${1:-}" in
    clean) rm -rf "$build" ;;
    "")    ;;
    *)     echo "usage: ${0##*/} [clean]" >&2; exit 2 ;;
esac

for tool in cmake ninja g++; do
    command -v "$tool" >/dev/null 2>&1 || {
        echo "error: $tool not found. In the MSYS2 UCRT64 shell, install the" >&2
        echo "       toolchain with:" >&2
        echo "         pacman -S --needed \$(bash tools/build-env.sh packages-msys2)" >&2
        exit 1
    }
done

if [ ! -f "$build/CMakeCache.txt" ]; then
    cmake -S "$REPO/tools/imgtest" -B "$build" -G Ninja
fi
cmake --build "$build"

# From the build directory: the harness writes its fixtures into the working
# directory, and those belong next to the binary rather than in the repo root.
cd "$build"
./imgtest.exe
