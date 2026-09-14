#!/usr/bin/env bash
# Build and run the GPT repair tests.
#
#   tools/gpttest.sh              # build (if needed) and run
#   tools/gpttest.sh clean        # drop the build dir first
#
# Exits non-zero if any check fails, so it can gate a commit or a release.
#
# The sources are in tools/gpttest/. They compile the real src/disk.cpp into a
# harness that drives relocateBackupGPT() and repairPrimaryGpt() against a file
# standing in for a device. harness_run in tools/build-env.sh does the rest.
#
# Copyright (C) 2026 peacepenguin, GPL-2.0-or-later.
set -euo pipefail

REPO=$(cd "$(dirname "$0")/.." && pwd)
. "$REPO/tools/build-env.sh"

harness_run "$REPO" gpttest "$@"
