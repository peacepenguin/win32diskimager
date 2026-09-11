# Reproducing the Windows GPT corruption on demand

Windows corrupts the primary GPT of an image whose `FirstUsableLBA` is not 34 —
which is most ARM board images — as soon as the disk is rescanned. This builds
a 48 MB image that triggers it, writes it to a 104 MB VHDX, and shows the
damage. About a minute per cycle, no SD card and no imaging tool involved.

Details and the exact defect are at the [bottom](#what-is-actually-happening).

You need a Linux VM with `sfdisk`, `parted` and `python3`, on a Hyper-V host.
A second Windows VM is optional — the host itself can mount the VHDX instead.

## 1. Build the 48 MB test image

On the Linux VM. `first-lba: 2048` is the field under test; `sfdisk` can set it
and `sgdisk` cannot. No filesystems — only the table matters.

```
# fedora44 used in this example:
sudo dnf install python3 parted util-linux

cd ~
git clone https://github.com/peacepenguin/win32diskimager

dd if=/dev/zero of=example.img bs=1000000 count=48

sfdisk example.img <<'EOF'
label: gpt
first-lba: 2048
start=32768, size=46875, type=0FC63DAF-8483-4772-8E79-3D69D8477DE4, name="TESTPART1"
start=79872, size=13812, type=0FC63DAF-8483-4772-8E79-3D69D8477DE4, name="TESTPART2"
EOF
```

Confirm the image is sound before blaming anything else later:

```
sudo parted example.img unit s print
sudo python3 win32diskimager/tools/gptdump.py example.img
```

Expect `FirstUsableLBA 2048`, `PartEntryLBA 2`, and every CRC `OK`. Keep
`example.img`; later cycles just re-write it.

## 2. Create the 104 MB VHDX and attach it to the Linux VM

Elevated PowerShell on the Hyper-V host.

```powershell
New-VHD -Path C:\vms\gpttest.vhdx -SizeBytes 104000000 -Dynamic   # 104 MB
Add-VMHardDiskDrive -VMName 'fedora44-builder' -ControllerType SCSI -Path C:\vms\gpttest.vhdx
```

SCSI so it can be attached and detached while the VM runs. A disk can only be
attached to one VM at a time.

## 3. Write the image to the disk

Pick the disk by exact size rather than assuming `/dev/sdb`.

```
TARGET=$(lsblk -bdno PATH,SIZE,TYPE | awk '$3=="disk" && $2==104000000 {print $1}')
[ "$(echo "$TARGET" | grep -c .)" -eq 1 ] || { echo "expected 1 disk, got: ${TARGET:-none}"; }
echo "target: $TARGET"
lsblk "$TARGET"
```

That should be an empty, unnamed disk. Then:

```
sudo dd if=example.img of="$TARGET" bs=1000000 conv=fsync
sudo partprobe "$TARGET"
```

`$TARGET` is used for the rest of the test; re-run the selection in a new shell.

## 4. Record the good state

```
sudo parted -l
sudo sgdisk -v "$TARGET"
sudo python3 win32diskimager/tools/gptdump.py "$TARGET" | tee before.txt
```

All three complain, and all three complaints are the healthy state — see
[the warnings](#the-warnings-that-are-not-the-bug). Answer `Ignore` to parted.

Copy the untouched disk on the host, for diffing later:

```powershell
Copy-Item C:\vms\gpttest.vhdx C:\vms\gpttest-before.vhdx
```

## 5. Show the disk to Windows

Detach from Linux, attach to the Windows VM:

```powershell
Remove-VMHardDiskDrive -VMName 'fedora44-builder' -ControllerType SCSI -ControllerNumber 0 -ControllerLocation 1
Add-VMHardDiskDrive -VMName 'win11-test' -ControllerType SCSI -Path C:\vms\gpttest.vhdx
```

(`Get-VMHardDiskDrive -VMName 'fedora44-builder'` gives the controller number
and location to remove.)

Or skip the VM and mount it on the host:

```powershell
Mount-DiskImage -ImagePath C:\vms\gpttest.vhdx
```

Then rescan — this is the trigger. Disk Management → Action → Rescan Disks,
`diskpart` → `rescan`, or:

```powershell
Update-HostStorageCache
```

Removable drives are rescanned automatically on insertion; SCSI disks like a
VHDX need this manual rescan. If an "Initialize disk" prompt appears, dismiss
it — accepting it wipes the disk.

Detach again:

```powershell
Remove-VMHardDiskDrive -VMName 'win11-test' -ControllerType SCSI -ControllerNumber 0 -ControllerLocation 1
Add-VMHardDiskDrive -VMName 'fedora44-builder' -ControllerType SCSI -Path C:\vms\gpttest.vhdx
```

```powershell
# if using the host:
Dismount-DiskImage -ImagePath C:\vms\gpttest.vhdx
```

## 6. Confirm the damage from Linux

```
sudo parted -l
sudo sgdisk -v "$TARGET"
sudo python3 win32diskimager/tools/gptdump.py "$TARGET" | tee after.txt
diff before.txt after.txt
```

`parted` now says `Partition Table: unknown`, and `gptdump.py` names the defect
outright:

```
PartEntryLBA     2016   count=128 size=128   (array spans 32 sectors, 2016..2047)
entries CRC over array: 0xab54d286 MISMATCH
** the array matching EntriesCRC is at LBA 2, not 2016 -- the header's PartEntryLBA is wrong by +2014 sectors
```

## 7. Repeat

Re-`dd` the image (step 3) and you are back to a known state. The VHDX and the
image do not need rebuilding.

---

# What is actually happening

On rescan, Windows relocates the backup GPT to the end of the device. That part
is correct and is what `sgdisk -e` does: `AlternateLBA` and `LastUsableLBA` are
updated, the backup header and its entry array are written at the device end,
and both verify.

It also rewrites the **primary** header's `PartitionEntryLBA` to
`FirstUsableLBA - 32`, instead of leaving it pointing at the entry array, which
has not moved from LBA 2. `PartitionEntryArrayCRC32` is then computed over the
wrong sectors and the primary table is rejected.

## Why `first-lba: 2048`

On an ordinary image `FirstUsableLBA` is 34, and `34 - 32 = 2` — the wrong
formula returns the right answer and nothing breaks. That is why this went
unnoticed for so long.

An image reserving space ahead of its first partition sets a higher value:
2048 on rk3588 boards, which keep idbloader and u-boot down there. `2048 - 32 =
2016`, which points at empty space, and the board stops booting because U-Boot
looks its partitions up by GPT name.

Rebuild the image with `first-lba: 34` and the identical test leaves the table
intact. That is the control.

## Why the damage is invisible from Windows

Windows validates the backup header — the half it wrote correctly. Disk
Management shows a healthy disk, and so does Rufus. `parted` reports `The
primary GPT table is corrupt, but the backup appears OK, so that will be used`.
Anything that reads the primary first, U-Boot included, rejects the table.

No data sector is touched. Only the table is broken, which is why the symptom
is "the board stopped booting" rather than "the card is blank".

## The warnings that are not the bug

In step 4, before anything is damaged:

* `parted`: `Not all of the space available ... appears to be used`. The image
  is smaller than the disk. Answer `Ignore`.
* `sgdisk`: `Problem: The secondary header's self-pointer indicates that it
  doesn't reside at the end of the disk`, plus a gap warning, ending
  `Identified 1 problems!`. Same observation, stated more alarmingly.
* `sgdisk`: `doesn't end on a 2048-sector boundary` — unrelated noise from the
  partition sizes.

The stranded backup GPT is the condition under test. Do not run `sgdisk -e` to
"fix" it: that is exactly what this fork's **Fix GPT after write** option does,
and it removes the trigger.

`before.txt` should show three sections — primary at `PartEntryLBA 2` with
`AlternateLBA 93749` flagged against a device last LBA of 203124; the backup
where the primary points, at LBA 93749 with its array at 93717, all CRCs `OK`;
and no signature at the device end, since the image never wrote out there.

## The sizes

PowerShell's `100MB` literal means 104857600 bytes, and Hyper-V's UI follows
the same convention, so a "100 MB" disk arrives 4.86% larger than its name and
Linux reports it as `105 MB`. Passing an exact byte count avoids the confusion.

104 rather than 100 because a VHDX must be a whole number of 512-byte sectors
and 100,000,000 ÷ 512 = 195312.5. Decimal sizes land on a sector boundary only
when the MB count is a multiple of 8.

| | bytes | 512-byte sectors | last LBA |
|---|---|---|---|
| disk  | 104000000 | 203125 | 203124 |
| image |  48000000 |  93750 |  93749 |

The image's backup GPT lands at LBA 93749, less than halfway into a device
whose last LBA is 203124. That gap is the setup.

## Notes for a bug report

* **The defect:** on rescan, Windows rewrites the primary GPT header's
  `PartitionEntryLBA` to `FirstUsableLBA - 32` instead of leaving it pointing
  at the partition entry array, invalidating `PartitionEntryArrayCRC32` on any
  disk whose `FirstUsableLBA` is not 34. The accompanying relocation of the
  backup GPT is correct and is not part of the complaint.
* Not specific to SD cards or removable media: this reproduces on a fixed VHDX
  on a synthetic SCSI controller.
* No third-party imaging tool is involved. The same repair fires after a Rufus
  write, so it belongs to Windows.
* Keep `before.txt`, `after.txt` and both VHDX copies. The two dumps side by
  side are the clearest artifact: same DiskGUID, same entries on disk, valid
  header CRC, and a `PartEntryLBA` pointing where the entries are not.
