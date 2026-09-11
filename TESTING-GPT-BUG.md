# Reproducing the Windows GPT corruption on demand

A repeatable bench version of [the bug](README.md#the-windows-gpt-problem),
using two Hyper-V VMs and a 104 MB virtual disk instead of a physical card. No
SD card, no reader, no imaging tool — the corruption is triggered by Windows
merely seeing the disk, so this reproduces it in about a minute per cycle and
produces artifacts you can attach to a bug report.

The shape of it: write a 48 MB image to a 104 MB disk so the backup GPT lands
mid-device, confirm Linux is happy with that, show the disk to Windows, then
look at it from Linux again.

You need a Linux VM (any distro with `sgdisk`, `parted` and `python3`) and a
Windows VM, both on the same Hyper-V host. Or you can skip the windows vm and just mount the vhdx directly on the host.

## 1. Build the 48 MB test image

On the Linux VM. The image is GPT with two ext4 partitions, each carrying a
marker file so you can tell afterwards whether the *data* survived — it does,
which is part of the point. Only the partition table is damaged.

```
# fedora44 used in this example:
sudo dnf install python3 parted sgdisk

cd ~

# get tools:
git clone https://github.com/peacepenguin/win32diskimager


# 48 MB exactly. bs=1M would be binary (1048576), so give dd a decimal block.
dd if=/dev/zero of=example.img bs=1000000 count=48

# GPT, two partitions: 24 MB and the rest of the image. sgdisk's M suffix is
# binary as well, so sizes go in 512-byte sectors: 46875 * 512 = 24,000,000.
sgdisk -o example.img
sgdisk -n 1:2048:+46875 -t 1:8300 -c 1:"TESTPART1" example.img
sgdisk -n 2:0:0         -t 2:8300 -c 2:"TESTPART2" example.img

#Format and fill the partitions through a loop device:
LOOP=$(sudo losetup -fP --show example.img)
echo "$LOOP"

sudo mkfs.ext4 -q -L TESTPART1 ${LOOP}p1
sudo mkfs.ext4 -q -L TESTPART2 ${LOOP}p2

sudo mkdir -p /mnt/p1 /mnt/p2
sudo mount ${LOOP}p1 /mnt/p1
sudo mount ${LOOP}p2 /mnt/p2
echo "THIS IS PARTITION ONE" | sudo tee /mnt/p1/MARKER-PARTITION-1.txt
echo "THIS IS PARTITION TWO" | sudo tee /mnt/p2/MARKER-PARTITION-2.txt
sudo umount /mnt/p1 /mnt/p2
sudo losetup -d $LOOP
```

Keep `example.img` — every later cycle just re-writes it, so the test is
repeatable without rebuilding anything.

### Check the image is good before you start

Worth doing once, so that nothing later can be blamed on a bad image. `parted`
reads a file as happily as a device — but *not* with `-l`, which means "list
every block device" and ignores any file you name:

```
sudo parted example.img unit s print
sudo python3 win32diskimager/tools/gptdump.py example.img
```

Both partitions should be listed, with their `TESTPART1` / `TESTPART2` names,
and `gptdump.py` should report `HeaderCRC ... OK` and `entries CRC over array:
... OK` for the primary and backup headers alike.

Note what `parted` does **not** say here: there is no "Not all of the space
available appears to be used" warning. The image's backup GPT sits at the end
of the 48 MB file, exactly where it belongs, so the image is internally
consistent and complete. That warning appears in step 4, only after the image
is written to the larger 104 MB disk, and it is a remark about the image being
smaller than the device — not a defect in either.

To see the image alongside real devices in `parted -l`, attach it as a loop
device first (`sudo losetup -fP --show example.img`) and it appears as
`/dev/loopN`.

## 2. Create the 104 MB VHDX and attach it to the Linux VM

On the Hyper-V host, in an elevated PowerShell:

```powershell
New-VHD -Path C:\vms\gpttest.vhdx -SizeBytes 104000000 -Dynamic   # 104 MB
Add-VMHardDiskDrive -VMName 'fedora44-builder' -ControllerType SCSI -Path C:\vms\gpttest.vhdx
```

A SCSI controller lets you attach and detach while the VM is running, which is
what makes the cycle quick — Generation 2 VMs have nothing else, and on a
Generation 1 VM use SCSI rather than IDE for the same reason. A disk can only
be attached to one VM at a time.

**Why the byte count, and why 104 rather than 100.** PowerShell's `100MB` is
not 100 MB: the literal means 104857600 bytes, and Hyper-V's UI follows the
same convention, so a disk you asked for in "MB" arrives 4.86% larger than the
name suggests and Linux honestly reports it as `105 MB`. Passing the exact byte
count removes the guesswork — the number in the command is the number of bytes
you get.

104 rather than 100 because a VHDX has to be a whole number of 512-byte
sectors, and 100,000,000 ÷ 512 = 195312.5. Decimal sizes land on a sector
boundary only when the MB count is a multiple of 8, so 104 MB is the nearest
honest round number to the 100 MB this test would otherwise use. Every size in
this document is a real decimal MB, with nothing rounded behind your back:

| | bytes | 512-byte sectors | last LBA |
|---|---|---|---|
| disk  | 104000000 | 203125 | 203124 |
| image |  48000000 |  93750 |  93749 |

So the image's own backup GPT lands at LBA 93749, less than halfway into a
device whose last LBA is 203124. That gap is the entire setup.

## 3. Write the image to the disk

On the Linux VM. Rather than assuming the new disk is `/dev/sdb`, pick it by
size: nothing else on a normal machine is exactly 104000000 bytes, so the match
is unambiguous — and if it somehow is not, this refuses to guess rather than
writing to the wrong disk.

```
# Whole disks only (TYPE=disk excludes partitions and loop devices), matched on
# an exact byte count.
TARGET=$(lsblk -bdno PATH,SIZE,TYPE | awk '$3=="disk" && $2==104000000 {print $1}')

# Exactly one match, or stop.
[ "$(echo "$TARGET" | grep -c .)" -eq 1 ] || { echo "expected 1 disk, got: ${TARGET:-none}"; }
echo "target: $TARGET"
lsblk "$TARGET"
```

Look at that `lsblk` output before continuing — it should be an empty, unnamed
disk. Then write the image:

```
sudo dd if=example.img of="$TARGET" bs=1000000 conv=fsync
sudo partprobe "$TARGET"
```

`$TARGET` is used for the rest of the test, so re-run the selection if you come
back in a fresh shell.

## 4. Record the good state

```
sudo parted -l
```

Expect a complaint, and expect it to be *benign*:

```
Warning: Not all of the space available to /dev/sdb appears to be used ...
Fix/Ignore?
```

Answer `Ignore`. This is the normal, healthy state of a card written with an
image smaller than the card. The partition table is valid; there is simply
unused space past the end of it.

Capture the baseline — this is the "before" half of a bug report:

```
sudo sgdisk -v "$TARGET"
sudo python3 win32diskimager/tools/gptdump.py "$TARGET" | tee before.txt
sudo mount "${TARGET}1" /mnt/p1 && cat /mnt/p1/MARKER-PARTITION-1.txt; sudo umount /mnt/p1
```

`before.txt` should show `PartEntryLBA 2` in both headers, both CRCs `OK`, and
`AlternateLBA 93749` flagged against a device last LBA of 203124.

Take a copy of the untouched disk on the host, so you can diff sector ranges
afterwards rather than arguing from memory:

```powershell
Copy-Item C:\vms\gpttest.vhdx C:\vms\gpttest-before.vhdx
```

## 5. Show the disk to Windows

Detach from Linux, attach to the Windows VM:

```powershell
Remove-VMHardDiskDrive -VMName 'fedora44-builder' -ControllerType SCSI -ControllerNumber 0 -ControllerLocation 1
Add-VMHardDiskDrive -VMName 'windows' -ControllerType SCSI -Path C:\vms\gpttest.vhdx
```

(`Get-VMHardDiskDrive -VMName 'fedora44-builder'` gives the controller number and location
to remove.)

That is the whole step. **Do nothing in the Windows VM.** Do not open Disk
Management, do not click anything, and if a "You need to format the disk" or
"Initialize disk" prompt appears, dismiss it — accepting it would wipe the disk
and prove nothing. Windows repairs the table as soon as it enumerates the disk.

Opening Disk Management afterwards is informative, though: it shows two healthy
unknown-filesystem partitions, because the table Windows wrote passes Windows'
own checks. Rufus shows them too. That is why this goes unnoticed.

Now detach it again:

```powershell
Remove-VMHardDiskDrive -VMName 'windows' -ControllerType SCSI -ControllerNumber 0 -ControllerLocation 1
Add-VMHardDiskDrive -VMName 'fedora44-builder' -ControllerType SCSI -Path C:\vms\gpttest.vhdx
```

## 6. Confirm the damage from Linux

```
sudo parted -l
```

Expect:

```
Error: /dev/sdb: unrecognised disk label
...
Partition Table: unknown
```

And the detail:

```
sudo sgdisk -v "$TARGET"
sudo python3 win32diskimager/tools/gptdump.py "$TARGET" | tee after.txt
diff before.txt after.txt
```

What the diff shows:

* `AlternateLBA` moved from 93749 to 203124, and `LastUsableLBA` updated to
  match — that part is a legitimate repair, and is what `sgdisk -e` does.
* `PartEntryLBA` **rewritten to a sector the entry array does not occupy**.
  This is the bug. The header CRC is recomputed and valid, so the header looks
  fine in isolation, but `PartitionEntryArrayCRC32` is now checked against the
  wrong sectors. `gptdump.py` reports `entries CRC over array: ... MISMATCH`,
  and Linux rejects the table outright.

The partition data is untouched — mount `/dev/sdb1` by offset, or re-run
`sgdisk -e` on a fresh copy, and the marker files are still there. Only the
table is broken, which is exactly why the symptom is "the board stopped
booting" rather than "the card is blank".

## 7. Repeat

The cycle is step 3 through step 6 — re-`dd` the image and you are back to a
known state. No need to recreate the VHDX or the image.

## Notes for a bug report

* No third-party imaging tool is involved anywhere in this reproduction. The
  same repair fires after a Rufus write, so it belongs to Windows, not to any
  one tool.
* The trigger is the partition table rescan, which happens whenever Windows
  enumerates the disk — attaching it, replugging a card, or any rescan after a
  write.
* Keep `before.txt`, `after.txt`, and both VHDX copies. The two `gptdump.py`
  outputs side by side are the clearest single artifact: same DiskGUID, same
  partition entries on disk, valid header CRC, and a `PartEntryLBA` pointing
  somewhere the entries are not.
* The two mitigations this fork offers, for context on what does and does not
  avoid the repair, are in [README.md](README.md#what-this-fork-does-about-it).
