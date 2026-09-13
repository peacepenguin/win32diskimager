#!/usr/bin/env bash
# Build and run the GPT repair tests.
#
#   tools/gpttest.sh              # build (if needed) and run
#   tools/gpttest.sh clean        # drop the build dir first
#
# Exits non-zero if any check fails, so it can gate a commit or a release.
#
# The sources are in tools/gpttest/. They compile the real src/disk.cpp into a
# harness that drives relocateBackupGPT() against a file standing in for a
# device -- no card, no VM, no UAC prompt. MSYS2 UCRT64 only: it is Win32 code.
#
# Its own build directory, not build/, because it is a different cmake project
# and would otherwise fight the application's cache.
#
# Copyright (C) 2026 peacepenguin, GPL-2.0-or-later.
set -euo pipefail

REPO=$(cd "$(dirname "$0")/.." && pwd)
build="$REPO/build-gpttest"

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
    cmake -S "$REPO/tools/gpttest" -B "$build" -G Ninja
fi
cmake --build "$build"

# From the build directory: the harness writes a scratch image into the working
# directory, and that belongs next to the binary rather than in the repo root.
cd "$build"
./gpttest.exe
