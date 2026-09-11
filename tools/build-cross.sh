#!/bin/bash
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
set -e

IMAGE=${IMAGE:-w32di-build}
REPO=$(cd "$(dirname "$0")/.." && pwd)

if ! podman image exists "$IMAGE"; then
    echo "building $IMAGE (one time)..."
    podman build -t "$IMAGE" -f "$REPO/tools/Containerfile.build" "$REPO"
fi

if [ "$1" = "clean" ]; then
    rm -rf "$REPO/build"
fi

podman run --rm -v "$REPO:/src" "$IMAGE" bash -c '
set -e
# Configuring takes far longer than an incremental build, so only do it when
# there is no cache yet. ninja re-runs cmake itself if CMakeLists.txt changes.
if [ ! -f /src/build/CMakeCache.txt ]; then
    cmake -S /src/src -B /src/build -G Ninja \
      -DCMAKE_TOOLCHAIN_FILE=/usr/share/mingw/toolchain-mingw64.cmake \
      -DCMAKE_BUILD_TYPE=Release \
      -DLRELEASE_EXECUTABLE=/usr/bin/lrelease-qt6
fi
cmake --build /src/build
file /src/build/Win32DiskImager.exe
'
