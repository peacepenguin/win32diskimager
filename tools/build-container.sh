#!/usr/bin/env bash
# Cross-compile a win64 binary in the Fedora container, the way CI does.
#
#   tools/build-container.sh              # configure (if needed) and build
#   tools/build-container.sh clean        # drop the build dir first
#   tools/build-container.sh test         # a binary that asks for no elevation
#
# All this does is run tools/build-cross.sh inside the image, where the toolchain
# is already installed: the build itself, and every argument it takes, is that
# script. Use this when the host is not Fedora, or is but would rather not
# install the toolchain. The image builds itself on first use.
#
# The build lands in build/, while a build-cross.sh run on the host lands in
# build-cross/. That is deliberate: a cmake cache records the absolute path it
# was generated for, and the container sees this tree as /src, so one shared
# directory would make every switch between them fail.
#
# Copyright (C) 2026 peacepenguin, GPL-2.0-or-later.
set -euo pipefail

REPO=$(cd "$(dirname "$0")/.." && pwd)
. "$REPO/tools/build-env.sh"

container_run "$REPO" env BUILD_DIR=/src/build /src/tools/build-cross.sh "$@"
