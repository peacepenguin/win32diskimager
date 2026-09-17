# Win Disk Imager

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

**Write** an image to a device. A `.img.gz` or `.img.xz` is decompressed as it
goes, so there is never an expanded copy on disk and nothing to unpack first.

**Read** a device back into an image file, always as an uncompressed `.img`.

**Verify** a device against an image byte for byte, reading compressed images
the same way writing does.

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

## Partition tables

Writing an image smaller than the card leaves the backup GPT where the *image*
ends rather than where the *device* ends. Linux leaves that alone.

**Windows always rewrites it.** Any time it rescans a disk whose image was
smaller than the disk - which is every time such a card is plugged in - it
rewrites the partition table to match the device. That happens on every card,
every time, and most of what it does is right and worth having: the backup GPT
is moved to the end of the device and `AlternateLBA` and `LastUsableLBA` are
updated to match, which is what `sgdisk -e` would do and what the card wanted
anyway.

**Sometimes it corrupts the table doing it.** While rewriting, Windows also
recomputes the primary header's `PartitionEntryLBA` as `FirstUsableLBA - 32`,
rather than leaving it pointing at the entry array, which has not moved.
Whether that ruins the table depends entirely on the image:

- **`FirstUsableLBA` is 34** - the ordinary case, and most desktop and
  Raspberry Pi images. `34 - 32 = 2` is exactly where the entry array already
  is, so the wrong formula arrives at the right answer and nothing breaks.
  This is why the defect went unnoticed for so long.
- **`FirstUsableLBA` is higher** - ARM board images reserve space ahead of the
  first partition; rk3588 keeps idbloader and u-boot below LBA 2048. There
  `2048 - 32 = 2016` points at empty space, `PartitionEntryArrayCRC32` no
  longer describes what the header points at, and the primary table is
  corrupt. `parted` says `Partition Table: unknown`, and a Rock 5B fails to
  boot because U-Boot looks its partitions up by GPT name.

No data sector is touched either way, and Windows validates the backup header -
the half it wrote correctly - so Disk Management and Rufus still show a healthy
disk even when the primary table is ruined. Rufus triggers the same rewrite, so
this is Windows behaviour rather than a bug in any one imaging tool. The full
analysis, and a 48 MB reproducer that shows the damage in about a minute, are
in [TESTING-GPT-BUG.md](TESTING-GPT-BUG.md).

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

This is a personal fork of Win32DiskImager (upstream: the ImageWriter project),
not affiliated with it or its maintainers. It carries its own name, Win Disk
Imager, so a build of this fork is never mistaken for one of upstream's;
upstream did not write the changes described here and cannot support them -
report anything you hit at
[github.com/peacepenguin/win32diskimager](https://github.com/peacepenguin/win32diskimager/issues),
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
