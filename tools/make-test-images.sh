#!/usr/bin/env bash
#
# Generates test images for the raw / gzip / xz image source.
#
# Most of them are a DOS-partitioned disk with a single FAT volume holding
# README.TXT and PATTERN.BIN (64 KiB of self-describing sectors), plus an
# "ENDOFIMAGE" marker in the very last sector of the image. After flashing, the
# volume should mount, both files should read back intact, and the last sector
# of the device should carry the marker, which together show the whole image was
# written and not just the front of it.
#
# The exception is the GPT pair -- test-gpt-affected.img and test-gpt-safe.img --
# which carry a partition table and nothing else. They differ only in
# FirstUsableLBA, which is the field the Windows GPT rewrite bug turns on. See
# TESTING-GPT-BUG.md.
#
# Run on Fedora as a normal user (no root, no loop devices needed):
#
#   sudo dnf install util-linux dosfstools mtools xz gzip
#   tools/make-test-images.sh -o ~/diskimager-test-images
#
# Copyright (C) 2026 peacepenguin, GPL-2.0-or-later.

set -euo pipefail

OUTDIR="./test-images"
SIZES="16M 64M 512M"
VARSIZE="64M"          # base size the edge-case variants are built from
WITH_HUGE=0            # also build the >4 GiB pair (slow, needs a big target)
KEEP_RAW=1

usage() {
    cat <<EOF
Usage: ${0##*/} [options]

  -o DIR      output directory (default: $OUTDIR)
  -s "A B C"  sizes to build raw/gz/xz sets for (default: "$SIZES")
  -v SIZE     size the edge-case variants are built from (default: $VARSIZE)
  -H          also build the >4 GiB gz/xz pair (slow; needs a >4.1 GiB target)
  -n          do not keep the uncompressed .img files (compressed only)
  -h          this help

Sizes are anything truncate(1) accepts: 16M, 512M, 2G, ...
EOF
}

while getopts "o:s:v:Hnh" opt; do
    case "$opt" in
        o) OUTDIR=$OPTARG ;;
        s) SIZES=$OPTARG ;;
        v) VARSIZE=$OPTARG ;;
        H) WITH_HUGE=1 ;;
        n) KEEP_RAW=0 ;;
        h) usage; exit 0 ;;
        *) usage >&2; exit 2 ;;
    esac
done

need() {
    command -v "$1" >/dev/null 2>&1 || {
        echo "missing required tool: $1 ($2)" >&2
        exit 1
    }
}
need sfdisk    "util-linux"
need mkfs.vfat "dosfstools"
need mcopy     "mtools"
need gzip      "gzip"
need xz        "xz"
need python3   "python3"
need sha256sum "coreutils"
need truncate  "coreutils"

REPO=$(cd "$(dirname "$0")/.." && pwd)

mkdir -p "$OUTDIR"
OUTDIR=$(cd "$OUTDIR" && pwd)
WORK=$(mktemp -d)
trap 'rm -rf "$WORK"' EXIT

echo "output: $OUTDIR"

# ---------------------------------------------------------------- payload ---

cat > "$WORK/README.TXT" <<'EOF'
win32diskimager compressed-image test volume

If you can read this file from the flashed device, the image reached the
device intact.

PATTERN.BIN is 64 KiB of self-describing 512-byte sectors: each one starts
with the ASCII tag "SECT" followed by its own index as a little-endian
32-bit integer, and the rest of the sector is filled with a byte derived
from that index.  A sector written to the wrong offset shows up as a
mismatch between the tag and where the sector actually landed.

The final sector of the *image* (past the end of this filesystem) carries
the ASCII marker ENDOFIMAGE-win32diskimager-test-last-sector.  Read the
last sector of the flashed device to confirm the image was written all the
way to the end.
EOF

python3 - "$WORK/PATTERN.BIN" <<'EOF'
import struct, sys
sec, n = 512, 128           # 64 KiB
with open(sys.argv[1], "wb") as f:
    for i in range(n):
        f.write(b"SECT" + struct.pack("<I", i)
                + bytes([(i * 7 + 0x20) & 0xFF]) * (sec - 8))
EOF

# ------------------------------------------------------------- base image ---

# make_base SIZE PATH -- partitioned disk image with a populated FAT volume
make_base() {
    local size=$1 path=$2
    local part="$WORK/part.img"

    rm -f "$path" "$part"
    truncate -s "$size" "$path"

    local total_sectors part_start part_sectors ptype
    total_sectors=$(( $(stat -c%s "$path") / 512 ))
    part_start=2048
    part_sectors=$(( total_sectors - part_start ))
    if [ "$part_sectors" -lt 8192 ]; then
        echo "size $size is too small to hold a filesystem" >&2
        exit 1
    fi
    # 0x0e = FAT16 LBA, 0x0c = FAT32 LBA
    if [ "$part_sectors" -lt 1048576 ]; then ptype=e; else ptype=c; fi

    sfdisk --quiet --wipe always "$path" >/dev/null <<EOF
label: dos
start=$part_start, size=$part_sectors, type=$ptype, bootable
EOF

    truncate -s $(( part_sectors * 512 )) "$part"
    mkfs.vfat -n TESTIMG "$part" >/dev/null
    MTOOLS_SKIP_CHECK=1 mcopy -i "$part" "$WORK/README.TXT" "$WORK/PATTERN.BIN" ::
    dd if="$part" of="$path" bs=1M seek=1 conv=notrunc status=none

    # ENDOFIMAGE marker in the last sector of the image
    printf 'ENDOFIMAGE-win32diskimager-test-last-sector' |
        dd of="$path" bs=512 seek=$(( total_sectors - 1 )) conv=notrunc status=none
}

# ------------------------------------------------------ raw/gz/xz per size ---

for size in $SIZES; do
    tag=$(echo "$size" | tr 'A-Z' 'a-z')
    echo "building ${tag} set"
    make_base "$size" "$OUTDIR/test-${tag}.img"
    gzip -9 -c "$OUTDIR/test-${tag}.img" > "$OUTDIR/test-${tag}.img.gz"
    xz -9 -T0 -c "$OUTDIR/test-${tag}.img" > "$OUTDIR/test-${tag}.img.xz"
done

# ------------------------------------------------------------- variants -----

vtag=$(echo "$VARSIZE" | tr 'A-Z' 'a-z')
BASE="$OUTDIR/test-${vtag}.img"
if [ ! -f "$BASE" ]; then
    echo "building ${vtag} base for the variants"
    make_base "$VARSIZE" "$BASE"
    gzip -9 -c "$BASE" > "$OUTDIR/test-${vtag}.img.gz"
    xz -9 -T0 -c "$BASE" > "$OUTDIR/test-${vtag}.img.xz"
fi
echo "building variants from test-${vtag}.img"

halfmb=$(( $(stat -c%s "$BASE") / 2 / 1048576 ))

# multi-member gzip: two members back to back, must decode as one image
dd if="$BASE" bs=1M count="$halfmb" status=none | gzip -9 -c > "$WORK/m1.gz"
dd if="$BASE" bs=1M skip="$halfmb" status=none | gzip -9 -c > "$WORK/m2.gz"
cat "$WORK/m1.gz" "$WORK/m2.gz" > "$OUTDIR/test-multimember.img.gz"

# gzip followed by zero padding: must stop at the end of the member, not error
cat "$OUTDIR/test-${vtag}.img.gz" > "$OUTDIR/test-gz-trailing-pad.img.gz"
dd if=/dev/zero bs=4096 count=1 status=none >> "$OUTDIR/test-gz-trailing-pad.img.gz"

# two xz streams back to back
dd if="$BASE" bs=1M count=1 status=none | xz -9 -c > "$WORK/s2.xz"
cat "$OUTDIR/test-${vtag}.img.xz" "$WORK/s2.xz" > "$OUTDIR/test-multistream.img.xz"

# xz plus legal 4-byte-aligned stream padding
cat "$OUTDIR/test-${vtag}.img.xz" > "$OUTDIR/test-xz-stream-pad.img.xz"
dd if=/dev/zero bs=4 count=2 status=none >> "$OUTDIR/test-xz-stream-pad.img.xz"

# unaligned image: 511 bytes past a sector boundary, final sector is short
cp "$BASE" "$OUTDIR/test-unaligned.img"
printf 'TAIL-511-BYTES-NOT-A-FULL-SECTOR' >> "$OUTDIR/test-unaligned.img"
dd if=/dev/zero bs=1 count=479 status=none >> "$OUTDIR/test-unaligned.img"
gzip -9 -c "$OUTDIR/test-unaligned.img" > "$OUTDIR/test-unaligned.img.gz"
xz -9 -T0 -c "$OUTDIR/test-unaligned.img" > "$OUTDIR/test-unaligned.img.xz"

# A gzip file is only trusted to know its own size when it is too small to have
# wrapped the 32-bit size field: filesize * 1032 < 4 GiB, so anything over about
# 4.06 MB compressed takes the unknown-size path and must be written until the
# stream ends.  The images above are mostly zeros and compress far below that,
# so pad one with incompressible data to push it over the line cheaply -- this
# is the same path a real multi-gigabyte OS image takes, without the wait.
cp "$BASE" "$WORK/unknown.img"
dd if=/dev/urandom of="$WORK/unknown.img" bs=1M seek=8 count=8 conv=notrunc status=none
gzip -6 -c "$WORK/unknown.img" > "$OUTDIR/test-gz-size-unknown.img.gz"
gzsize=$(stat -c%s "$OUTDIR/test-gz-size-unknown.img.gz")
if [ "$gzsize" -le 4161790 ]; then
    echo "warning: test-gz-size-unknown.img.gz is only $gzsize bytes," \
         "under the ~4.06 MB threshold; it will NOT exercise the unknown-size path" >&2
fi
# The same image in xz, where the size stays exact however big the file gets.
xz -6 -T0 -c "$WORK/unknown.img" > "$OUTDIR/test-gz-size-unknown.img.xz"

# compressed name, raw content: open() decides by content, not by name
cp "$BASE" "$OUTDIR/test-raw-named-gz.img.gz"
cp "$BASE" "$OUTDIR/test-raw-named-xz.img.xz"

# truncated streams: must fail cleanly partway through
head -c 30000 "$OUTDIR/test-${vtag}.img.gz" > "$OUTDIR/test-truncated.img.gz"
head -c 5000 "$OUTDIR/test-${vtag}.img.xz" > "$OUTDIR/test-truncated.img.xz"

# corrupt payload: must fail, not write garbage.  Some byte patterns happen to
# decode unchanged, so keep moving until the reference tool actually rejects it.
corrupt() {
    local src=$1 dst=$2 test_cmd=$3 size pct off
    size=$(stat -c%s "$src")
    # Offsets are a fraction of the file so they land inside the payload of a
    # 10 KB xz and a 500 KB gzip alike, clear of the header and the footer.
    for pct in 50 35 65 45 55 40 60; do
        off=$(( size * pct / 100 ))
        [ "$off" -gt 64 ] && [ "$off" -lt $(( size - 16 )) ] || continue
        cp "$src" "$dst"
        printf '\x5A\xA5\x3C\xC3\x17\x71' |
            dd of="$dst" bs=1 seek="$off" conv=notrunc status=none
        if ! $test_cmd "$dst" >/dev/null 2>&1; then return 0; fi
    done
    echo "warning: could not produce a corrupt ${dst##*/}" >&2
    rm -f "$dst"
}
corrupt "$OUTDIR/test-${vtag}.img.gz" "$OUTDIR/test-corrupt.img.gz" "gzip -t"
corrupt "$OUTDIR/test-${vtag}.img.xz" "$OUTDIR/test-corrupt.img.xz" "xz -t"

# ---------------------------------------------------------- GPT test pair ---

# Two images whose only difference is FirstUsableLBA, for the Windows GPT
# rewrite bug. On a rescan Windows recomputes the primary header's
# PartitionEntryLBA as FirstUsableLBA minus the length of the entry array:
#
#   34   - 32 = 2     the real entry array. Harmless, and why this went unseen.
#   2048 - 32 = 2016  empty space. The primary table is then rejected by Linux
#                     and the board will not boot.
#
# 2048 is what rk3588 and similar boards use, keeping idbloader and u-boot below
# the first partition. No filesystems: only the table is under test.
#
# TESTING-GPT-BUG.md explains the defect and how to reproduce it on a VHDX.
make_gpt_image()
{
    local firstlba=$1 path=$2
    rm -f "$path"
    truncate -s 48000000 "$path"          # 48 MB, as in TESTING-GPT-BUG.md
    sfdisk --quiet --wipe always "$path" >/dev/null <<EOF
label: gpt
first-lba: $firstlba
start=32768, size=46875, type=0FC63DAF-8483-4772-8E79-3D69D8477DE4, name="TESTPART1"
start=79872, size=13812, type=0FC63DAF-8483-4772-8E79-3D69D8477DE4, name="TESTPART2"
EOF
    # Confirm the field under test really is what was asked for. sfdisk aligns
    # things quietly, and an image that claims to be the affected one while
    # holding 34 would make the whole test meaningless. Read it back out of the
    # header rather than out of sfdisk's own dump, which omits first-lba when it
    # is the default.
    local got
    got=$(python3 "$REPO/tools/gptdump.py" "$path" \
          | sed -n 's/^ *FirstUsableLBA *//p' | head -1)
    if [ "$got" != "$firstlba" ]; then
        echo "error: ${path##*/} wanted FirstUsableLBA $firstlba, got '${got:-none}'" >&2
        exit 1
    fi
}

echo "building the GPT pair"
make_gpt_image 2048 "$OUTDIR/test-gpt-affected.img"
make_gpt_image 34   "$OUTDIR/test-gpt-safe.img"

# -------------------------------------------------- "Shrink image on Read" ---
#
# Every image below puts a recognizable ASCII tag in each region that matters,
# rather than leaving it zero: a region "Shrink image on Read" must drop and
# one it must instead preserve would otherwise both read back the same way if
# the shrink got it wrong. Comparing the shrunk output against these tags
# (not just its size) is what actually catches a gap left in, or -- worse --
# real data left out.
#
# None of these are also built as .gz/.xz: they are the *device* content for a
# Write, then a Read back with the box checked, not an image opened straight
# by ImageSource the way the raw/gz/xz sets above are.

# fill_pattern PATH OFFSET_BYTES LEN_BYTES TAG
fill_pattern() {
    local path=$1 off=$2 len=$3 tag=$4
    python3 - "$path" "$off" "$len" "$tag" <<'EOF'
import sys
path, off, length, tag = sys.argv[1], int(sys.argv[2]), int(sys.argv[3]), sys.argv[4].encode()
buf = (tag * (length // len(tag) + 1))[:length]
with open(path, "r+b") as f:
    f.seek(off)
    f.write(buf)
EOF
}

# write_fat_part DISKPATH OFFSET_SECTORS SIZE_SECTORS LABEL -- formats a FAT
# volume of SIZE_SECTORS holding README.TXT and PATTERN.BIN, and drops it into
# DISKPATH at OFFSET_SECTORS, the same way make_base() does for the plain sets.
write_fat_part() {
    local disk=$1 off=$2 sectors=$3 label=$4
    local blob="$WORK/${label}.part"
    rm -f "$blob"
    truncate -s $(( sectors * 512 )) "$blob"
    mkfs.vfat -n "$label" "$blob" >/dev/null
    MTOOLS_SKIP_CHECK=1 mcopy -i "$blob" "$WORK/README.TXT" "$WORK/PATTERN.BIN" ::
    dd if="$blob" of="$disk" bs=512 seek="$off" conv=notrunc status=none
}

# check_firstusablelba PATH WANT -- same check make_gpt_image() does, read
# back out of the header rather than trusted from what was asked for.
check_firstusablelba() {
    local path=$1 want=$2 got
    got=$(python3 "$REPO/tools/gptdump.py" "$path" \
          | sed -n 's/^ *FirstUsableLBA *//p' | head -1)
    if [ "$got" != "$want" ]; then
        echo "error: ${path##*/} wanted FirstUsableLBA $want, got '${got:-none}'" >&2
        exit 1
    fi
}

# make_shrink_mbr_image PATH
#
# 200 MB MBR disk: 50 MB unpartitioned, a 100 MB FAT partition, then another
# 50 MB unpartitioned. An MBR has no FirstUsableLBA field the way GPT does,
# but the boot sector alone marks exactly the same amount of the device as
# reserved -- one sector -- so "Shrink image on Read" repacks an MBR device
# the same way it does a GPT one: both gaps here must be gone, and the
# partition moved right after the boot sector. The partition start (102400)
# is not a multiple of 8 sectors either, so this also exercises the 4Kn-
# alignment repacking, the same as test-shrink-gpt.img does for GPT.
make_shrink_mbr_image() {
    local path=$1
    local sector=512
    local gap_sectors=$(( 50 * 1048576 / sector ))    # 102400
    local part_sectors=$(( 100 * 1048576 / sector ))  # 204800
    local part_start=$gap_sectors
    local total_sectors=$(( part_start + part_sectors + gap_sectors ))

    rm -f "$path"
    truncate -s $(( total_sectors * sector )) "$path"
    sfdisk --quiet --wipe always "$path" >/dev/null <<EOF
label: dos
start=$part_start, size=$part_sectors, type=e, bootable
EOF
    write_fat_part "$path" "$part_start" "$part_sectors" SHRINKMBR

    # Sector 0 carries the MBR itself; the fillable front gap starts at 1.
    fill_pattern "$path" "$sector" $(( (part_start - 1) * sector )) \
        "FRONT-GAP-MUST-BE-DROPPED-BY-SHRINK "
    local back_off=$(( (part_start + part_sectors) * sector ))
    fill_pattern "$path" "$back_off" $(( gap_sectors * sector )) \
        "BACK-GAP-MUST-BE-DROPPED-BY-SHRINK "
}

# make_shrink_mbr_tight_image PATH
#
# One MBR partition already running from the sector right after the boot
# sector to the very last sector of the device: there is no gap anywhere for
# "Shrink image on Read" to remove. Checking the box against this image
# should read it back in full, byte for byte, exactly as if it had been left
# unchecked -- the negative case for test-shrink-mbr.img and
# test-shrink-mbr-multi.img.
make_shrink_mbr_tight_image() {
    local path=$1
    local sector=512
    local part_start=1
    local part_sectors=$(( 100 * 1048576 / sector - part_start ))
    local total_sectors=$(( part_start + part_sectors ))

    rm -f "$path"
    truncate -s $(( total_sectors * sector )) "$path"
    sfdisk --quiet --wipe always "$path" >/dev/null <<EOF
label: dos
start=$part_start, size=$part_sectors, type=e, bootable
EOF
    write_fat_part "$path" "$part_start" "$part_sectors" SHRTIGHT
}

# make_shrink_mbr_multi_image PATH
#
# Three MBR primary partitions rather than one, with a gap ahead of each of
# the first two and none after the last -- "between partitions", which
# test-shrink-mbr.img's single partition cannot exercise, and the MBR
# counterpart to test-shrink-gpt-multi.img. No start is a multiple of 8
# sectors. Geometry only, no filesystems: each partition gets its own stamp
# instead, the same as the GPT multi-partition image.
make_shrink_mbr_multi_image() {
    local path=$1
    local sector=512
    local gap1=100003 gap2=50007
    local p1=204801 p2=102403 p3=153607
    local p1_start=$(( 1 + gap1 ))
    local p2_start=$(( p1_start + p1 + gap2 ))
    local p3_start=$(( p2_start + p2 ))
    local total_sectors=$(( p3_start + p3 ))

    rm -f "$path"
    truncate -s $(( total_sectors * sector )) "$path"
    sfdisk --quiet --wipe always "$path" >/dev/null <<EOF
label: dos
start=$p1_start, size=$p1, type=e, bootable
start=$p2_start, size=$p2, type=e
start=$p3_start, size=$p3, type=e
EOF
    fill_pattern "$path" "$sector" $(( gap1 * sector )) \
        "GAP1-BEFORE-PART1-MUST-BE-DROPPED "
    fill_pattern "$path" $(( p1_start * sector )) $(( p1 * sector )) "PART1-DATA "
    fill_pattern "$path" $(( (p1_start + p1) * sector )) $(( gap2 * sector )) \
        "GAP2-BETWEEN-PART1-AND-PART2-MUST-BE-DROPPED "
    fill_pattern "$path" $(( p2_start * sector )) $(( p2 * sector )) "PART2-DATA "
    fill_pattern "$path" $(( p3_start * sector )) $(( p3 * sector )) "PART3-DATA "
}

# make_shrink_gpt_image PATH
#
# 200-ish MB GPT disk, default FirstUsableLBA (34): a 50 MB gap, a 100 MB FAT
# partition, then another 50 MB gap before the backup GPT. Both gaps must be
# gone, and the backup GPT relocated right after the partition, once "Shrink
# image on Read" has run. The partition start (102434) is not a multiple of
# 8 sectors, so this also exercises the 4Kn-alignment repacking, not just
# gap removal.
make_shrink_gpt_image() {
    local path=$1
    local sector=512
    local front_reserved=34        # protective MBR + primary header + entries
    local backup_reserved=33       # backup entries (32) + backup header (1)
    local gap_sectors=$(( 50 * 1048576 / sector ))    # 102400
    local part_sectors=$(( 100 * 1048576 / sector ))  # 204800
    local part_start=$(( front_reserved + gap_sectors ))
    local total_sectors=$(( part_start + part_sectors + gap_sectors + backup_reserved ))

    rm -f "$path"
    truncate -s $(( total_sectors * sector )) "$path"
    sfdisk --quiet --wipe always "$path" >/dev/null <<EOF
label: gpt
first-lba: $front_reserved
start=$part_start, size=$part_sectors, type=0FC63DAF-8483-4772-8E79-3D69D8477DE4, name="SHRINKGPT"
EOF
    write_fat_part "$path" "$part_start" "$part_sectors" SHRINKGPT

    fill_pattern "$path" $(( front_reserved * sector )) $(( gap_sectors * sector )) \
        "GAP-BEFORE-PARTITION-MUST-BE-DROPPED "
    local back_off=$(( (part_start + part_sectors) * sector ))
    fill_pattern "$path" "$back_off" $(( gap_sectors * sector )) \
        "GAP-AFTER-PARTITION-MUST-BE-DROPPED "

    check_firstusablelba "$path" "$front_reserved"
}

# make_shrink_gpt_reserved_image PATH
#
# Same idea, but FirstUsableLBA is raised to 65536 (32 MiB), the way rk3588
# and similar boards reserve room for idbloader/U-Boot ahead of the first
# partition (see the GPT pair above). That reserved span is stamped and must
# survive a shrink completely unchanged -- it is not a gap, and nothing here
# should treat it like one. A further 20 MB gap between the reserved span and
# the partition, and another 20 MB before the backup GPT, must both still be
# dropped: an elevated FirstUsableLBA does not make the ordinary gaps around
# it any less removable.
make_shrink_gpt_reserved_image() {
    local path=$1
    local sector=512
    local firstusable=65536        # 32 MiB reserved, e.g. for U-Boot
    local backup_reserved=33
    local gap_sectors=$(( 20 * 1048576 / sector ))    # 40960
    local part_sectors=$(( 100 * 1048576 / sector ))  # 204800
    local part_start=$(( firstusable + gap_sectors ))
    local total_sectors=$(( part_start + part_sectors + gap_sectors + backup_reserved ))

    rm -f "$path"
    truncate -s $(( total_sectors * sector )) "$path"
    sfdisk --quiet --wipe always "$path" >/dev/null <<EOF
label: gpt
first-lba: $firstusable
start=$part_start, size=$part_sectors, type=0FC63DAF-8483-4772-8E79-3D69D8477DE4, name="SHRINKRSV"
EOF
    write_fat_part "$path" "$part_start" "$part_sectors" SHRINKRSV

    # [34, firstusable): the reserved span itself -- must be copied verbatim.
    fill_pattern "$path" $(( 34 * sector )) $(( (firstusable - 34) * sector )) \
        "RESERVED-BOOTLOADER-DATA-MUST-SURVIVE "
    # [firstusable, part_start): an ordinary gap despite the elevated
    # FirstUsableLBA -- must still be dropped.
    fill_pattern "$path" $(( firstusable * sector )) $(( gap_sectors * sector )) \
        "GAP-AFTER-RESERVED-MUST-BE-DROPPED "
    local back_off=$(( (part_start + part_sectors) * sector ))
    fill_pattern "$path" "$back_off" $(( gap_sectors * sector )) \
        "TAIL-GAP-MUST-BE-DROPPED "

    check_firstusablelba "$path" "$firstusable"
}

# make_shrink_gpt_tight_image PATH
#
# Default FirstUsableLBA, one partition that already runs from FirstUsableLBA
# to the sector before the backup GPT: there is no gap anywhere left to
# remove. Checking "Shrink image on Read" against this image should read it
# in full, byte for byte, exactly as if the box had been left unchecked --
# the negative case for the two GPT images above.
make_shrink_gpt_tight_image() {
    local path=$1
    local sector=512
    local front_reserved=34
    local backup_reserved=33
    local part_sectors=$(( 100 * 1048576 / sector ))
    local part_start=$front_reserved
    local total_sectors=$(( part_start + part_sectors + backup_reserved ))

    rm -f "$path"
    truncate -s $(( total_sectors * sector )) "$path"
    sfdisk --quiet --wipe always "$path" >/dev/null <<EOF
label: gpt
first-lba: $front_reserved
start=$part_start, size=$part_sectors, type=0FC63DAF-8483-4772-8E79-3D69D8477DE4, name="SHRINKTIGHT"
EOF
    write_fat_part "$path" "$part_start" "$part_sectors" SHRTIGHT

    check_firstusablelba "$path" "$front_reserved"
}

# make_shrink_gpt_multi_image PATH
#
# Three partitions rather than one, with a gap ahead of each of the first two
# and none after the last (the trailing case is already covered above) --
# "between partitions" is what a single-partition image cannot exercise.
# Every start is deliberately not a multiple of 8 sectors (4 KiB), so a
# repack that failed to realign a partition would still move the right bytes
# to the right place while landing it on the wrong boundary. No filesystems:
# with three partitions and three gaps this is about the repacking math, so
# each region gets its own stamp instead.
make_shrink_gpt_multi_image() {
    local path=$1
    local sector=512
    local front_reserved=34
    local backup_reserved=33
    local gap1=100003 gap2=50007
    local p1=204801 p2=102403 p3=153607
    local p1_start=$(( front_reserved + gap1 ))
    local p2_start=$(( p1_start + p1 + gap2 ))
    local p3_start=$(( p2_start + p2 ))
    local total_sectors=$(( p3_start + p3 + backup_reserved ))

    rm -f "$path"
    truncate -s $(( total_sectors * sector )) "$path"
    sfdisk --quiet --wipe always "$path" >/dev/null <<EOF
label: gpt
first-lba: $front_reserved
start=$p1_start, size=$p1, type=0FC63DAF-8483-4772-8E79-3D69D8477DE4, name="PART1"
start=$p2_start, size=$p2, type=0FC63DAF-8483-4772-8E79-3D69D8477DE4, name="PART2"
start=$p3_start, size=$p3, type=0FC63DAF-8483-4772-8E79-3D69D8477DE4, name="PART3"
EOF
    fill_pattern "$path" $(( front_reserved * sector )) $(( gap1 * sector )) \
        "GAP1-BEFORE-PART1-MUST-BE-DROPPED "
    fill_pattern "$path" $(( p1_start * sector )) $(( p1 * sector )) "PART1-DATA "
    fill_pattern "$path" $(( (p1_start + p1) * sector )) $(( gap2 * sector )) \
        "GAP2-BETWEEN-PART1-AND-PART2-MUST-BE-DROPPED "
    fill_pattern "$path" $(( p2_start * sector )) $(( p2 * sector )) "PART2-DATA "
    fill_pattern "$path" $(( p3_start * sector )) $(( p3 * sector )) "PART3-DATA "

    check_firstusablelba "$path" "$front_reserved"
}

echo "building the shrink-on-read set"
make_shrink_mbr_image          "$OUTDIR/test-shrink-mbr.img"
make_shrink_mbr_tight_image    "$OUTDIR/test-shrink-mbr-tight.img"
make_shrink_mbr_multi_image    "$OUTDIR/test-shrink-mbr-multi.img"
make_shrink_gpt_image          "$OUTDIR/test-shrink-gpt.img"
make_shrink_gpt_reserved_image "$OUTDIR/test-shrink-gpt-reserved.img"
make_shrink_gpt_tight_image    "$OUTDIR/test-shrink-gpt-tight.img"
make_shrink_gpt_multi_image    "$OUTDIR/test-shrink-gpt-multi.img"

# ------------------------------------------------------------ >4 GiB pair ---

if [ "$WITH_HUGE" = 1 ]; then
    echo "building the >4 GiB pair (slow)"
    # gzip stores only the low 32 bits of the uncompressed size, so past 4 GiB
    # the size is not knowable up front and the image has to be written until
    # the stream ends.  xz carries an index, so its size stays exact -- the two
    # files are the same image and compare the two paths directly.
    { cat "$BASE"; dd if=/dev/zero bs=1M count=4096 status=none; } |
        tee >(gzip -6 -c > "$OUTDIR/test-over4g.img.gz") |
        xz -0 -T0 -c > "$OUTDIR/test-over4g.img.xz"
fi

# ------------------------------------------------------------- manifest -----

if [ "$KEEP_RAW" = 0 ]; then
    # Only the raw images that also exist compressed. The GPT pair is raw-only
    # -- there is no test-gpt-affected.img.gz -- so a blanket *.img would throw
    # away the two images the GPT repair is tested with.
    for img in "$OUTDIR"/*.img; do
        [ -e "$img" ] || continue
        if [ -e "$img.gz" ] || [ -e "$img.xz" ]; then
            rm -f "$img"
        fi
    done
fi

# nullglob, and a check that something matched: an unmatched *.img would reach
# sha256sum as a literal, fail, and take the script down with it before the
# manifest below was ever written.
(
    cd "$OUTDIR"
    shopt -s nullglob
    sums=( *.img *.img.gz *.img.xz )
    if [ ${#sums[@]} -gt 0 ]; then
        sha256sum -- "${sums[@]}" > SHA256SUMS
    else
        : > SHA256SUMS
    fi
)

cat > "$OUTDIR/MANIFEST.txt" <<EOF
win32diskimager raw / gzip / xz test images
generated $(date -Iseconds) by tools/make-test-images.sh

Each image below is a DOS-partitioned disk with one FAT volume (label TESTIMG)
holding README.TXT and PATTERN.BIN, and an ENDOFIMAGE marker in the last
sector of the image -- except the GPT pair, which is a partition table and
nothing else.

should write, verify, and mount cleanly
  test-<size>.img              raw, the baseline for each size
  test-<size>.img.gz           the same image, gzip
  test-<size>.img.xz           the same image, xz
  test-multimember.img.gz      two gzip members back to back -> one image
  test-gz-trailing-pad.img.gz  gzip plus zero padding; stops at the member end
  test-multistream.img.xz      two xz streams; decodes to the base image plus
                               its first 1 MiB, so it is LARGER than the base
  test-xz-stream-pad.img.xz    xz plus legal 4-byte stream padding
  test-unaligned.img[.gz|.xz]  511 bytes past a sector boundary; the final
                               sector is short and must be zero-padded
  test-raw-named-gz.img.gz     raw content under a .gz name -> treated as raw
  test-raw-named-xz.img.xz     raw content under a .xz name -> treated as raw
  test-gz-size-unknown.img.gz  padded with incompressible data so the gzip file
                               clears ~4.06 MB, the point past which the size
                               field could have wrapped and is no longer
                               trusted.  Writes until the stream ends, which is
                               what any real multi-gigabyte image does.
  test-gz-size-unknown.img.xz  the same image in xz, where the size IS known --
                               flash both and compare how progress behaves

the GPT pair, for the Windows GPT rewrite bug -- no filesystem, only a table
  test-gpt-affected.img        FirstUsableLBA 2048, as ARM board images have it.
                               With "Fix GPT after write" ON the write should end
                               with the GPT made to match the device. With it OFF
                               the warning should say this image IS affected.
  test-gpt-safe.img            the same table with FirstUsableLBA 34, the control.
                               With the option OFF the warning should say this
                               image is NOT affected. Nothing else differs.

for "Shrink image on Read" -- write one of these to a device (or attach it
directly), then Read it back with the box checked. Every gap and every
region that must survive is stamped with its own ASCII tag rather than left
zero, so diffing the shrunk output against the original catches a gap left
in, or real data left out, that comparing sizes alone would miss.
  test-shrink-mbr.img          MBR: 50 MB gap, 100 MB FAT partition, 50 MB gap.
                               An MBR has no FirstUsableLBA field, but the boot
                               sector reserves exactly the same one sector GPT
                               reserves at minimum, so both gaps here (tagged
                               ...-MUST-BE-DROPPED) should be gone and the
                               partition moved to right after the boot sector.
                               Its start is also not on a 4Kn boundary, so this
                               exercises the alignment repacking too. Shrunk
                               size: ~100 MB plus the boot sector.
  test-shrink-mbr-tight.img    MBR, one partition already spanning from the
                               sector right after the boot sector to the end
                               of the device: nothing to shrink anywhere.
                               Checking the box against this image should
                               read it back in full and byte for byte
                               identical, the same as leaving it unchecked --
                               the negative case for the two MBR images here.
  test-shrink-mbr-multi.img    MBR, three primary partitions with a gap ahead
                               of each of the first two (tags GAP1-.../GAP2-
                               ...) and none after the last -- "between
                               partitions", which test-shrink-mbr.img's single
                               partition cannot exercise. No start is on a 4Kn
                               boundary. Geometry only, no filesystems (each
                               partition is instead filled with its own
                               PART<n>-DATA tag), the same as its GPT
                               counterpart below.
  test-shrink-gpt.img          GPT, default FirstUsableLBA: 50 MB gap, 100 MB
                               FAT partition, 50 MB gap, backup GPT. Both gaps
                               (tagged ...-MUST-BE-DROPPED) should be gone and
                               the backup GPT relocated right after the
                               partition. The partition also does not start on
                               a 4Kn boundary, so this exercises the alignment
                               repacking as well as gap removal. Shrunk size:
                               ~100 MB plus one entry-array-and-header's worth
                               of sectors.
  test-shrink-gpt-reserved.img GPT, FirstUsableLBA raised to 65536 (32 MiB), as
                               rk3588 and similar boards reserve for U-Boot.
                               The reserved span itself (tagged
                               RESERVED-BOOTLOADER-DATA-MUST-SURVIVE) must
                               come through byte for byte; the 20 MB gap after
                               it and the 20 MB gap before the backup GPT
                               (both tagged ...-MUST-BE-DROPPED) must not.

                               DO NOT attempt volume creation at that reserved span in Disk
                               Management once this is on a device. GPT itself
                               has no concept for "reserved, not a partition,
                               do not touch" beyond FirstUsableLBA -- Windows
                               shows the span as ordinary unallocated space,
                               with nothing marking it any different from a
                               real gap. Disk Management correctly refuses to
                               create a volume there, but that attempt (even
                               though it fails) has been observed to leave the
                               disk stuck: every subsequent write gets Access
                               Denied, and neither restarting
                               win32diskimager nor toggling the disk
                               online/offline clears it -- only physically
                               removing and reinserting the device does. That
                               points at Virtual Disk Service (the thing
                               behind Disk Management) holding its own handle
                               on the drive after the rejected attempt, which
                               is a Windows/VDS matter, not something this
                               app can detect or release. This is further
                               evidence of buggy behavior in windows related
                               to handling firstusableLBA.
  test-shrink-gpt-tight.img    GPT, one partition already spanning from
                               FirstUsableLBA to the backup GPT: nothing to
                               shrink anywhere. Checking the box against this
                               image should read it back in full and byte for
                               byte identical, the same as leaving it
                               unchecked -- the negative case for the two GPT
                               images above.
  test-shrink-gpt-multi.img    GPT, three partitions with a gap ahead of each
                               of the first two (tags GAP1-.../GAP2-...) and
                               none after the last -- "between partitions",
                               which a single-partition image cannot exercise.
                               No start is on a 4Kn boundary. Geometry only,
                               no filesystems (each partition is instead
                               filled with its own PART<n>-DATA tag), so
                               checking this one is a matter of confirming
                               each tag ends up contiguous and in order, not
                               mounting anything.

should fail with a clear error, and not report a half-written device as good
  test-truncated.img.gz        gzip stream cut off partway
  test-truncated.img.xz        xz stream cut off partway
  test-corrupt.img.gz          gzip payload corrupted mid-stream
  test-corrupt.img.xz          xz payload corrupted mid-stream

should ALSO write, verify, and mount cleanly -- built only with -H, and needs a
target larger than 4.1 GiB
  test-over4g.img.gz           >4 GiB: the gzip size field wraps (to exactly
                               64 MiB here), so the size is NOT known and the
                               write must run until the stream ends.  A device
                               holding only the first 64 MiB is the failure this
                               catches, and it will look like a clean write.
  test-over4g.img.xz           the same image in xz, where the size IS known

After flashing, compare the device against the image byte for byte.  This
covers every case below, including the short final sector and the >4 GiB pair:
  tools/verify-flashed.sh test-64m.img.gz /dev/sdX

To check the tail by hand instead, seek by the size of the IMAGE, not of the
device -- the card is larger than every image here, so the last sector of the
card is old data, not the end of what was written:
  size=\$(xz -dc test-64m.img.xz | wc -c)      # or: stat -c%s test-64m.img
  sudo dd if=/dev/sdX bs=512 skip=\$(( (size + 511) / 512 - 1 )) count=1 |
      head -c 64
The marker there is ENDOFIMAGE-... for every image except test-unaligned.*,
whose final sector is the short one and reads TAIL-511-BYTES-NOT-A-FULL-SECTOR
followed by zero padding.

"Read to .img.gz" / "Read to .img.xz" apply to any Read, shrunk or not -- there
is no separate fixture for them. After a shrink run above (or a plain one),
repeat it with one of the compression boxes checked and confirm the result is
the correctly-named file (a "bob.img" target becomes bob.img.gz/.img.xz) and
opens back up as the same content, e.g. via test-shrink-gpt.img and:
  gzip -t bob.img.gz && xz -t bob.img.xz
EOF

echo
cat "$OUTDIR/MANIFEST.txt"
echo
ls -la "$OUTDIR"
