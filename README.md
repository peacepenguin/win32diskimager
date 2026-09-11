# Personal Fork of Image Writer (win32diskimager) for Microsoft Windows

This utility reads and writes raw image files to SD and USB memory devices. Run
it as Administrator, point it at your raw image, and select the device to write
to. It cannot write CD-ROMs, and USB floppy is not supported.

This fork fixes an issue where images written from Windows get their GPT
corrupted, leaving the card unbootable — see
[The Windows GPT problem](#the-windows-gpt-problem).

This program has no warranty. The authors take no responsibility for lost or
damaged data.

| | |
|---|---|
| Building | [BUILD.md](BUILD.md) |
| Reproducing the bug | [TESTING-GPT-BUG.md](TESTING-GPT-BUG.md) |
| Changes | [Changelog.txt](Changelog.txt) |
| Planned work | [TODO.txt](TODO.txt) |
| Translating | [readme-translations.txt](readme-translations.txt) |

## The Windows GPT problem

Writing an image smaller than the card leaves the backup GPT where the *image*
ends rather than where the *device* ends. That is normal, and Linux leaves it
alone. Windows treats it as damage and repairs it on the next rescan — which it
does as soon as the device is re-enumerated after a write.

Most of that repair is correct: the backup GPT is moved to the end of the
device and `AlternateLBA` and `LastUsableLBA` are updated to match. But Windows
also rewrites the primary header's `PartitionEntryLBA` to `FirstUsableLBA - 32`
instead of leaving it pointing at the entry array, which has not moved.

On an ordinary image `FirstUsableLBA` is 34, so `34 - 32 = 2` is accidentally
correct and nothing breaks. ARM board images reserve space ahead of the first
partition — rk3588 keeps idbloader and u-boot below LBA 2048 — and there
`2048 - 32 = 2016` points at empty space. `PartitionEntryArrayCRC32` is then
computed over the wrong sectors:

    Partition Table: unknown

Windows validates the backup header, the half it wrote correctly, so Disk
Management and Rufus still show a sensible partition list. Anything reading the
primary first rejects the table, and a Rock 5B (Armbian, Batocera) fails to
boot because U-Boot looks its partitions up by GPT name. Rufus triggers the same
repair, so this is Windows behaviour rather than a bug in any one imaging tool.

The full analysis, and a 48 MB reproducer, are in
[TESTING-GPT-BUG.md](TESTING-GPT-BUG.md).

### What this fork does about it

The **Fix GPT after write** checkbox:

**Checked (default) — fix the table.** After writing, the backup GPT is moved
to the true last LBA and the header updated to match, the way `sgdisk -e` does.
Windows finds a consistent table and has nothing to repair, which removes the
trigger rather than racing it. It is what you want on the card anyway, and the
device can be handled normally afterwards.

**Unchecked — preserve the image byte for byte.** The disk is taken offline and
ejected before the volume locks are released, so nothing can rescan it, and a
dialog tells you to remove the card without re-inserting it. The card ends up
identical to a Linux `dd`.

Both paths are verified on real hardware against a Rock 5B.

Two further changes to the write path: every write first zeroes the first and
last 34 sectors, clearing any table left by a previous larger image (hygiene —
it does **not** prevent the repair on its own); and writes lock and dismount
*every* volume on the target disk, open it without sharing writes, and flush
and close before unlocking, closing the windows where another process could
modify the disk mid-write.

`tools/gptdump.py` decodes and checksum-verifies both GPT headers of an image
or a raw device, and reports where an entry array actually lives when a header
points elsewhere.

## The device list

Devices are enumerated as physical disks (`\\.\PhysicalDriveN`), not as drive
letters. Upstream scanned letters, so a card only appeared once Windows had
mounted a filesystem and assigned one — which a card holding a Linux image
never gets, making the card it had just written invisible. Each entry shows its
drive letters if any, its size, and the model it reports.

Removable and USB/SD/MMC devices are always listed. **Show all devices** adds
fixed disks, for internal PCIe card readers that present the card as
non-removable. The disk Windows is running from is never listed. The list also
refreshes on a timer, since a card going into a reader that presents no volume
produces no device-arrival broadcast at all.

## Fork changes:
 * Removed installer
 * Updated to QT6
 * Added build notes and scripts
 * Fixed the Windows GPT corruption described above
 * Added the "Fix GPT after write" option
 * Lock every volume on the target disk while writing; offline and eject when done
 * Zero stale partition tables at both ends of the device before writing
 * Enumerate physical disks instead of drive letters, so devices with no
   letter still appear
 * Added the "Show all devices" option for card readers that present as fixed
 * Added a GitHub Actions workflow that cross-compiles for win64 from Linux

## Legal

Image Writer for Windows is licensed under the General Public License v2, with
full text in [GPL-2](GPL-2).

This project uses and includes binaries of the MinGW runtime library
(http://www.mingw.org) and of the Qt library (http://www.qt-project.org/),
the latter licensed under the "Library General Public License", with full text
in [LGPL-2.1](LGPL-2.1).

Original version developed by Justin Davis <tuxdavis@gmail.com>.
Maintained by the ImageWriter developers
(http://sourceforge.net/projects/win32diskimager).
