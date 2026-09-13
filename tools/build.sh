#!/usr/bin/env bash
# Build the app natively on Windows, in the MSYS2 UCRT64 shell.
#
#   tools/build.sh              # configure (if needed) and build into build/
#   tools/build.sh clean        # drop the build dir first
#   tools/build.sh test         # into build/ as usual, but a binary that asks
#                               # for no elevation, for working on the interface
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

build_parse_args "$@" || { echo "usage: ${0##*/} [test] [clean]" >&2; exit 2; }
build=${BUILD_DIR:-$REPO/build}

for tool in cmake ninja g++ windeployqt6; do
    command -v "$tool" >/dev/null 2>&1 || {
        echo "error: $tool not found. In the MSYS2 UCRT64 shell, install the" >&2
        echo "       toolchain with:" >&2
        echo "         pacman -S --needed \$(bash tools/build-env.sh packages-msys2)" >&2
        exit 1
    }
done

build_prepare "$build"
build_run "$REPO/src" "$build" native_configure
build_report "$build" "tools/deploy.sh"
