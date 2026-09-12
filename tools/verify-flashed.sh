#!/usr/bin/env bash
#
# Compares a flashed device against the image it was written from, byte for
# byte, for raw / gzip / xz images alike.
#
#   sudo tools/verify-flashed.sh test-64m.img.gz /dev/sdX
#
# A compressed image is checked against its uncompressed twin when one is
# sitting next to it -- test-64m.img.gz against test-64m.img -- so nothing has
# to be decompressed.  Where there is no twin (test-multimember.img.gz,
# test-multistream.img.xz, test-over4g.img.gz) the stream is decompressed
# instead, or you can name a reference image yourself with -r.
#
# Only the first N bytes of the device are read, where N is the size of the
# uncompressed image, so it does not matter that the card is larger than the
# image.  A short final sector is handled the way the writer handles it: the
# image is compared as-is, and the rest of that sector on the device must be
# zero.  This only applies to an image that is not a whole number of sectors;
# when the size divides evenly there is nothing to pad and the check is skipped.
#
# Copyright (C) 2026 peacepenguin, GPL-2.0-or-later.

set -euo pipefail

REFERENCE=""

usage() {
    cat <<EOF
Usage: sudo ${0##*/} [-r REFERENCE] IMAGE DEVICE

  -r REFERENCE  raw image to compare against, instead of looking for one next
                to IMAGE or decompressing IMAGE

  sudo ${0##*/} test-64m.img.gz /dev/sdb
  sudo ${0##*/} -r test-64m.img test-multimember.img.gz /dev/sdb
EOF
}

while getopts "r:h" opt; do
    case "$opt" in
        r) REFERENCE=$OPTARG ;;
        h) usage; exit 0 ;;
        *) usage >&2; exit 2 ;;
    esac
done
shift $((OPTIND - 1))

if [ $# -ne 2 ]; then
    usage >&2
    exit 2
fi

IMAGE=$1
DEVICE=$2

[ -f "$IMAGE" ] || { echo "no such image: $IMAGE" >&2; exit 1; }
[ -b "$DEVICE" ] || { echo "not a block device: $DEVICE" >&2; exit 1; }
[ -r "$DEVICE" ] || { echo "cannot read $DEVICE -- run with sudo" >&2; exit 1; }

# Decide by content, not by name: the test set deliberately includes raw images
# under a .gz or .xz name.
magic=$(head -c 6 "$IMAGE" | od -An -tx1 | tr -d ' \n')
case "$magic" in
    1f8b*)         kind=gzip ;;
    fd377a585a00*) kind=xz ;;
    *)             kind=raw ;;
esac

if [ -z "$REFERENCE" ] && [ "$kind" != raw ]; then
    # test-64m.img.gz -> test-64m.img, when that file is actually there.
    sibling="${IMAGE%.*}"
    if [ -f "$sibling" ]; then
        REFERENCE=$sibling
    fi
fi

if [ -n "$REFERENCE" ]; then
    [ -f "$REFERENCE" ] || { echo "no such reference: $REFERENCE" >&2; exit 1; }
    echo "image:     $IMAGE ($kind)"
    echo "reference: $REFERENCE (uncompressed, read directly)"
    size=$(stat -c%s "$REFERENCE")
    source=(cat "$REFERENCE")
else
    echo "image:     $IMAGE ($kind)"
    case "$kind" in
        gzip) source=(gzip -dc "$IMAGE") ;;
        xz)   source=(xz -dc "$IMAGE") ;;
        *)    source=(cat "$IMAGE") ;;
    esac
    if [ "$kind" = raw ]; then
        size=$(stat -c%s "$IMAGE")
    else
        echo "no uncompressed twin next to it, so sizing the stream" \
             "(pass -r to skip this)..."
        size=$("${source[@]}" | wc -c)
    fi
fi
echo "size:      $size bytes"

devsize=$(( $(blockdev --getsz "$DEVICE") * 512 ))
if [ "$size" -gt "$devsize" ]; then
    echo "image is larger than the device ($devsize bytes) -- it cannot have" \
         "been written in full" >&2
    exit 1
fi

echo "comparing against $DEVICE..."
status=0
cmp -n "$size" <("${source[@]}") "$DEVICE" || status=$?
if [ "$status" -ne 0 ]; then
    echo "MISMATCH: the device does not match the image" >&2
    exit 1
fi
echo "the first $size bytes of $DEVICE match the image exactly"

# A short final sector: the writer zero-pads it, so check the padding too.
tail_bytes=$(( size % 512 ))
if [ "$tail_bytes" -ne 0 ]; then
    pad=$(( 512 - tail_bytes ))
    nonzero=$(dd if="$DEVICE" bs=1 skip="$size" count="$pad" status=none |
              tr -d '\0' | wc -c)
    if [ "$nonzero" -ne 0 ]; then
        echo "MISMATCH: the $pad bytes padding the short final sector are not" \
             "all zero" >&2
        exit 1
    fi
    echo "the $pad bytes padding the short final sector are zero, as expected"
fi

echo "OK"
