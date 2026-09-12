#!/usr/bin/env bash
# The cross-build environment, in one place: which Fedora packages the toolchain
# needs, where that toolchain lives, and how cmake is invoked against it.
#
# Everything that cross-builds reads this file, so none of it is written down
# twice: tools/Containerfile.build, tools/build-cross.sh, tools/build-container.sh,
# tools/lupdate-cross.sh, tools/deploy-cross.sh and .github/workflows/build.yml.
# BUILD.md points here rather than repeating the values.
#
# Use it either way. As a library:
#
#   . tools/build-env.sh
#   cross_configure /src/src /src/build
#
# or as a command:
#
#   tools/build-env.sh install            # dnf install the toolchain
#   tools/build-env.sh check              # assert the layout is as expected
#   tools/build-env.sh configure SRC BUILD [extra cmake args...]
#   tools/build-env.sh packages [ci]      # print the package list
#   tools/build-env.sh packages-msys2     # print the MSYS2 list, for a native build
#   tools/build-env.sh print NAME         # print one value (SYSROOT, TOOLCHAIN, ...)
#
# Copyright (C) 2026 peacepenguin, GPL-2.0-or-later.

# ---------------------------------------------------------------- toolchain ---

# Fedora is not a preference: Debian and Ubuntu ship no MinGW Qt6 packages, so
# there is nothing to link against there.
CROSS_BASE_IMAGE="fedora:44"

# qt6-linguist is the *native* Linguist build, and carries both halves of the
# translation workflow: lrelease-qt6, which the build runs to compile lang/*.ts
# into the .qm files translations.qrc embeds, and lupdate-qt6, which
# tools/lupdate-cross.sh runs to refresh those .ts files from the sources.
# Neither can come from mingw64-qt6-qttools: those are Windows .exe files.
CROSS_PACKAGES="cmake ninja-build file findutils binutils
                mingw64-gcc-c++ mingw64-qt6-qtbase mingw64-qt6-qttools
                mingw64-qt6-qttranslations
                mingw64-zlib mingw64-xz
                qt6-linguist"

# Only CI needs these: zip and gh to publish a release, git for the checkout.
CROSS_CI_PACKAGES="git zip gh"

# The MSYS2 UCRT64 packages for a native Windows build. Nothing here installs
# them -- that is done by hand, once -- but the list belongs with the others.
MSYS2_PACKAGES="mingw-w64-ucrt-x86_64-gcc
                mingw-w64-ucrt-x86_64-cmake
                mingw-w64-ucrt-x86_64-ninja
                mingw-w64-ucrt-x86_64-qt6-base
                mingw-w64-ucrt-x86_64-qt6-tools
                mingw-w64-ucrt-x86_64-qt6-translations
                mingw-w64-ucrt-x86_64-zlib
                mingw-w64-ucrt-x86_64-xz"

# Where Fedora's mingw64 packages put things. Overridable for a host that lays
# them out differently, and so a single piece can be pointed elsewhere without
# editing this file.
CROSS_TOOLCHAIN="${CROSS_TOOLCHAIN:-/usr/share/mingw/toolchain-mingw64.cmake}"
CROSS_LRELEASE="${CROSS_LRELEASE:-/usr/bin/lrelease-qt6}"
CROSS_LUPDATE="${CROSS_LUPDATE:-/usr/bin/lupdate-qt6}"
CROSS_SYSROOT="${CROSS_SYSROOT:-/usr/x86_64-w64-mingw32/sys-root/mingw}"

# The image tools/Containerfile.build produces. Override with IMAGE=...
CROSS_IMAGE="${IMAGE:-w32di-build}"

# Extra "podman run" arguments a caller wants, as an array.
CONTAINER_ENV=()

# ---------------------------------------------------------------- functions ---

cross_packages()
{
    # shellcheck disable=SC2086   # deliberate word splitting: one per line
    if [ "${1:-}" = "ci" ]; then
        echo $CROSS_PACKAGES $CROSS_CI_PACKAGES
    else
        echo $CROSS_PACKAGES
    fi
}

cross_install()
{
    # shellcheck disable=SC2046
    dnf -y install $(cross_packages "${1:-}")
    cross_check
}

# Fail early and loudly if the layout is not what the build and deploy expect.
cross_check()
{
    local bad=0
    [ -f "$CROSS_TOOLCHAIN" ] || { echo "missing $CROSS_TOOLCHAIN" >&2; bad=1; }
    [ -x "$CROSS_LRELEASE" ]  || { echo "missing $CROSS_LRELEASE" >&2; bad=1; }
    [ -x "$CROSS_LUPDATE" ]   || { echo "missing $CROSS_LUPDATE" >&2; bad=1; }
    [ -d "$CROSS_SYSROOT" ]   || { echo "missing $CROSS_SYSROOT" >&2; bad=1; }
    return $bad
}

# cross_configure SRCDIR BUILDDIR [extra cmake args...]
#
# LRELEASE_EXECUTABLE matters: Qt6::lrelease from a MinGW Qt is a Windows .exe
# and cannot run on the build host, so the native lrelease-qt6 compiles the
# translations instead.
cross_configure()
{
    local src=${1:?usage: cross_configure SRCDIR BUILDDIR [cmake args...]}
    local build=${2:?}
    shift 2
    cmake -S "$src" -B "$build" -G Ninja \
        -DCMAKE_TOOLCHAIN_FILE="$CROSS_TOOLCHAIN" \
        -DCMAKE_BUILD_TYPE=Release \
        -DLRELEASE_EXECUTABLE="$CROSS_LRELEASE" \
        "$@"
}

# container_run REPO COMMAND...
#
# Runs COMMAND in the Fedora image with REPO mounted at /src, building the image
# first if it is not there yet. The one copy of the podman plumbing: everything
# that needs a container goes through here.
#
# W32DI_IN_CONTAINER lets the script on the inside tell where it is, so a script
# that falls back to the container cannot end up calling itself forever when the
# image is missing something.
container_run()
{
    local repo=${1:?usage: container_run REPO COMMAND...}
    shift

    command -v podman >/dev/null 2>&1 || {
        echo "error: podman not found, and this needs a container." >&2
        echo "       On Fedora you can install the toolchain instead:" >&2
        echo "         sudo bash tools/build-env.sh install" >&2
        return 1
    }
    if ! podman image exists "$CROSS_IMAGE"; then
        echo "building $CROSS_IMAGE (one time)..." >&2
        podman build -t "$CROSS_IMAGE" -f "$repo/tools/Containerfile.build" "$repo"
    fi
    podman run --rm -v "$repo:/src" -w /src \
        -e W32DI_IN_CONTAINER=1 \
        "${CONTAINER_ENV[@]}" \
        "$CROSS_IMAGE" "$@"
}

# ------------------------------------------------------------------ command ---

# Only when run, not when sourced.
if [ "${BASH_SOURCE[0]}" = "$0" ]; then
    set -euo pipefail
    cmd=${1:-}
    shift || true
    case "$cmd" in
        install)         cross_install "${1:-}" ;;
        check)           cross_check ;;
        configure)       cross_configure "$@" ;;
        packages)        cross_packages "${1:-}" ;;
        packages-msys2)  echo $MSYS2_PACKAGES ;;
        print)
            case "${1:-}" in
                IMAGE)     echo "$CROSS_BASE_IMAGE" ;;
                TOOLCHAIN) echo "$CROSS_TOOLCHAIN" ;;
                LRELEASE)  echo "$CROSS_LRELEASE" ;;
                LUPDATE)   echo "$CROSS_LUPDATE" ;;
                SYSROOT)   echo "$CROSS_SYSROOT" ;;
                *) echo "print: unknown name '${1:-}'" >&2; exit 2 ;;
            esac
            ;;
        *)
            sed -n '2,25p' "$0" | sed 's/^# \{0,1\}//'
            [ -n "$cmd" ] && exit 2
            exit 0
            ;;
    esac
fi
