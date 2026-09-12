#!/usr/bin/env bash
# Cross-compile a win64 binary in the Fedora container, the way CI does.
#
#   tools/build-cross.sh          # configure (if needed) and build
#   tools/build-cross.sh clean    # drop the build dir first
#
# The image is built once by:
#   podman build -t w32di-build -f tools/Containerfile.build .
#
# build/ and src/lang/*.qm are gitignored, so building straight into the work
# tree is safe and keeps ninja incremental between runs.
set -euo pipefail

IMAGE=${IMAGE:-w32di-build}
REPO=$(cd "$(dirname "$0")/.." && pwd)

if ! podman image exists "$IMAGE"; then
    echo "building $IMAGE (one time)..."
    podman build -t "$IMAGE" -f "$REPO/tools/Containerfile.build" "$REPO"
fi

if [ "${1:-}" = "clean" ]; then
    rm -rf "$REPO/build"
fi

podman run --rm -v "$REPO:/src" "$IMAGE" bash -c '
set -euo pipefail
# The toolchain paths and the cmake invocation live in one place, shared with
# CI and the container image.
. /src/tools/build-env.sh
# Configuring takes far longer than an incremental build, so only do it when
# there is no cache yet. ninja re-runs cmake itself if CMakeLists.txt changes.
if [ ! -f /src/build/CMakeCache.txt ]; then
    cross_configure /src/src /src/build
fi
cmake --build /src/build
file /src/build/Win32DiskImager.exe
'
