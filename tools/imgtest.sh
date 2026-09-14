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
# is checked in and neither gzip nor xz has to be on the path. harness_run in
# tools/build-env.sh does the rest.
#
# Copyright (C) 2026 peacepenguin, GPL-2.0-or-later.
set -euo pipefail

REPO=$(cd "$(dirname "$0")/.." && pwd)
. "$REPO/tools/build-env.sh"

harness_run "$REPO" imgtest "$@"
