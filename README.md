# WinDiskImager

Reads and writes raw disk images to any Windows block storage device: USB flash
drive, SD card, SATA, NVMe, mounted VHDX, drive enclosure. Run it as
Administrator, point it at an image, and choose the device to write to.

This program has no warranty. The authors take no responsibility for lost or
damaged data.

| | |
|---|---|
| Building from source | [BUILD.md](BUILD.md) |
| What changed, per release | [Changelog.txt](Changelog.txt) |
| Translating | [readme-translations.txt](readme-translations.txt) |
| The GPT defect, in full | [TESTING-GPT-BUG.md](TESTING-GPT-BUG.md) |

## What it does

**Write** a raw image file `.img` to a device. `.img.gz` or `.img.xz` is decompressed as it is read, so there is never an expanded copy on disk.

**Read** a device back into an image file, as an uncompressed `.img` or with
compression to `.img.gz` or `.img.xz`. **Shrink image on Read** repacks the
partitions to remove unpartitioned gaps instead of copying the device byte for
byte -- see [Shrink image on Read](#shrink-image-on-read).

**Verify** a device against an image byte for byte, reading compressed images
the same way writing does. Verify will inform if the GPT is different, but the partition data is identical in case it's been expanded by this tool or by windows.

**Checksum** the image file, as MD5, SHA1 or SHA256. Choosing an image selects
SHA256, which is what publishers usually quote.

**Keep the partition table intact**, which is the one thing this fork exists
for; see [Partition tables](#partition-tables).

## The device list

Devices are enumerated as physical disks (`\.\PhysicalDriveN`), not as drive
letters. Upstream scanned letters, so a card only appeared once Windows had
mounted a filesystem and assigned one - which a card holding a Linux image
never gets, making the card it had just written invisible. Each entry shows its
drive letters if any, its size, and the model it reports.

Removable and USB/SD/MMC devices are always listed. **Show all devices** adds
fixed disks, for internal PCIe card readers that present the card as
non-removable. The disk Windows is running from is never listed. The list also
refreshes on a timer, since a card going into a reader that presents no volume
produces no device-arrival broadcast at all.

## Shrink image on Read

Reading a device normally copies it byte for byte, sector zero to the last
one, whether or not anything is actually there. **Shrink image on Read**
instead reads the device's MBR or GPT, works out where each partition
actually starts and ends, and repacks them back to back -- closing the gap
ahead of the first partition, any gap between partitions, and the gap after
the last one -- so the image comes out only as large as the data it holds.
Each partition is realigned to a 4K boundary as it moves, so the result
works on both 512-byte and 4Kn media regardless of how the original was
laid out. For a GPT device the backup table is rebuilt and relocated to the
new end; for MBR, which has no backup table, only the partition entries
themselves are updated.

A device with no partition table, or one already packed this tight, is read
in full instead -- silently, since neither is an error. Combine it with **Read
to .img.gz** / **Read to .img.xz** to shrink and compress in the same pass.

## Partition tables

Writing an image smaller than the card leaves the backup GPT where the *image*
ends rather than where the *device* ends. Linux leaves that alone; **Windows
always rewrites it** the first time the disk is rescanned, which is every time
such a card is plugged in.

Most of that rewrite is correct and welcome: the backup GPT is moved to the
end of the device the way `sgdisk -e` would. **But while doing it, Windows
also recomputes the primary header's `PartitionEntryLBA` as `FirstUsableLBA -
32`** instead of leaving it pointing at the entry array. On an ordinary image
`FirstUsableLBA` is 34, so the wrong formula happens to land on the right
answer and nothing breaks -- which is why this went unnoticed for so long. An
ARM board image that reserves space ahead of its first partition (rk3588 and
similar keep idbloader and u-boot below LBA 2048) sets `FirstUsableLBA`
higher, the formula then points at empty space, and the primary table is
corrupted -- invisibly, since Windows validates the backup header it wrote
correctly and shows a healthy disk regardless. No data sector is touched
either way; only the table breaks, which is why the symptom is "the board
stopped booting" rather than "the card is blank". Rufus triggers the same
rewrite, so this is Windows behaviour, not a bug in any one imaging tool.

The full mechanism, why it is invisible from Windows, and a 48 MB reproducer
that shows the damage in about a minute with no SD card involved, are all in
[TESTING-GPT-BUG.md](TESTING-GPT-BUG.md).

### Fix GPT after write

**Checked - fix the table.** After writing, the backup GPT is moved to the true
last LBA and the header updated to match, the way `sgdisk -e` does. Windows
finds a consistent table and has nothing to repair, which removes the trigger
rather than racing it. It is what you want on the card anyway, and the device
can be handled normally afterwards.

**Unchecked - preserve the image byte for byte.** The disk is taken offline and
ejected before the volume locks are released, so nothing can rescan it, and a
dialog tells you to remove the card without re-inserting it. The card ends up
identical to a Linux `dd`. Re-inserting it in Windows lets Windows rewrite the
table, which corrupts it if the image's `FirstUsableLBA` is not 34.

### Repairing a table Windows has already broken

Verify checks the table as well as the data. A card that Windows has already
rescanned still matches its image sector for sector - only the table is wrong -
so a plain comparison passes and tells you nothing.

When the device holds the image correctly but its primary GPT points at sectors
the partition entries are not in, Verify says so and offers to repair it. The
repair points `PartitionEntryLBA` back at the entry array and rebuilds the
header checksum. No data sector is touched.

### Other write-path changes

Every write first zeroes the first and last 34 sectors, clearing any table left
by a previous larger image. That is hygiene - it does **not** prevent the
repair on its own. Writes also lock and dismount *every* volume on the target
disk, open it without sharing writes, and flush and close before unlocking,
closing the windows where another process could modify the disk mid-write.

`tools/gptdump.py` decodes and checksum-verifies both GPT headers of an image
or a raw device, and reports where an entry array actually lives when a header
points elsewhere.

## About this fork

This is a fork of Win32DiskImager (upstream: the ImageWriter project),
not affiliated with it or its maintainers. It carries its own name,
WinDiskImager, so a build of this fork is never mistaken for one of upstream's;
upstream did not write the changes described here and cannot support them -
report anything you hit at
[github.com/peacepenguin/windiskimager](https://github.com/peacepenguin/windiskimager/issues),
not to SourceForge.

What changed in each release is in [Changelog.txt](Changelog.txt); the sections
above describe how the current version behaves.

## Legal

Image Writer for Windows is licensed under the General Public License v2, with
full text in [GPL-2](GPL-2). The modifications in this fork are released under
the same licence, Copyright (C) 2026 peacepenguin; the files they touch carry
notices saying what changed.

This project uses and includes binaries of the MinGW runtime library
(http://www.mingw.org) and of the Qt library (http://www.qt-project.org/),
the latter licensed under the "Library General Public License", with full text
in [LGPL-2.1](LGPL-2.1).

Original version developed by Justin Davis <tuxdavis@gmail.com>.
Maintained by the ImageWriter developers
(http://sourceforge.net/projects/win32diskimager).
