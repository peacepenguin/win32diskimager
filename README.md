# Personal Fork of Image Writer (win32diskimager) for Microsoft Windows

This fork fixes an issue where images written to SD cards from Windows get
their GPT structure corrupted, leaving the card unbootable — see
[The Windows GPT problem](#the-windows-gpt-problem).

This utility reads and writes raw image files to SD and USB memory devices.
Run it as Administrator, point it at your raw image, and select the removable
device to write to. It cannot write CD-ROMs, and USB floppy is not supported.

This program has no warranty. The authors take no responsibility for lost or
damaged data.

## The Windows GPT problem

Writing an image smaller than the target card leaves the backup GPT where the
*image* ends, not where the *device* ends. That is normal, and Linux leaves it
alone — `parted` just notes the unused space.

Windows treats it as damage and silently repairs it. The repair moves the
backup GPT to the end of the device and updates `AlternateLBA` and
`LastUsableLBA` to match, but it also rewrites `PartitionEntryLBA` to point
somewhere the entry array does not live. The result passes Windows' own checks,
so Disk Management and Rufus still show a sensible partition list, while Linux
computes `PartitionEntryArrayCRC32` over the wrong sectors and rejects the
table outright:

    Partition Table: unknown

An rk3588 board (Rock 5B — Armbian, Batocera) then fails to boot, because
U-Boot looks its partitions up by GPT name. Rufus triggers the same repair, so
this is a Windows behaviour rather than a bug in any one imaging tool.

The repair fires whenever Windows re-reads the partition table, which it does
as soon as the device is rescanned after a write.

### What this fork does about it

Use the **Fix GPT after write** checkbox:

**Checked (default) — fix the table.** After writing, the backup GPT is moved
to the true last LBA of the device and the header updated to match, the way
`sgdisk -e` does. Windows finds a consistent table and has nothing to repair.
This removes the trigger rather than racing it, and it is what you want on the
card anyway. The device can be handled normally afterwards.

**Unchecked — preserve the image byte for byte.** The disk is taken offline and
ejected before the volume locks are released, so nothing can rescan it, and a
dialog tells you to physically remove the card without re-inserting it into
Windows. The card ends up identical to a Linux `dd`, backup GPT still where the
image put it.

Both paths are verified on real hardware against a Rock 5B.

Every write also zeroes the first and last 34 sectors of the device first,
clearing any partition table left by a previous, larger image. This is hygiene,
not a fix — testing shows it does **not** prevent Windows' repair, which is
driven by the new image's own backup GPT sitting mid-device. One of the two
options above is what protects the card.

Writes lock and dismount *every* volume on the target disk (not just the one
holding the selected drive letter), open the disk without sharing writes, and
flush and close it before unlocking — all of which previously left windows
where Windows or another process could modify the disk mid-write.

`tools/gptdump.py` decodes and checksum-verifies both GPT headers of an image
file or a raw device, if you want to inspect a card yourself.

| | |
|---|---|
| Building | [BUILD.md](BUILD.md) |
| Changes | [Changelog.txt](Changelog.txt) |
| Planned work | [TODO.txt](TODO.txt) |
| Translating | [readme-translations.txt](readme-translations.txt) |

## Fork changes:
 * Removed installer
 * Updated to QT6
 * Added build notes and scripts
 * Fixed the Windows GPT corruption described above
 * Added the "Fix GPT after write" option
 * Lock every volume on the target disk while writing; offline and eject when done
 * Zero stale partition tables at both ends of the device before writing
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
