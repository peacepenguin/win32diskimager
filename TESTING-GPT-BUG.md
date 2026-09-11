# Reproducing the Windows GPT corruption on demand

A repeatable bench version of [the bug](README.md#the-windows-gpt-problem),
using two Hyper-V VMs and a 104 MB virtual disk instead of a physical card. No
SD card, no reader, no imaging tool — the corruption is triggered by Windows
merely seeing the disk, so this reproduces it in about a minute per cycle and
produces artifacts you can attach to a bug report.

The shape of it: write a 48 MB image to a 104 MB disk so the backup GPT lands
mid-device, confirm Linux is happy with that, show the disk to Windows, then
look at it from Linux again.

**What Windows gets wrong.** On rescan it relocates the backup GPT to the end
of the device — correctly, and the backup header and its entry array both come
out valid. But it also rewrites the *primary* header's `PartitionEntryLBA` to
`FirstUsableLBA - 32`, rather than leaving it pointing at the entry array that
is still sitting at LBA 2. `PartitionEntryArrayCRC32` is then computed over the
wrong sectors and the primary table is rejected.

That formula is why the image below sets `first-lba: 2048`, and it is the whole
reason this bug goes unnoticed. On an ordinary image `FirstUsableLBA` is 34, so
`34 - 32 = 2` — the wrong formula lands on the right answer by coincidence and
nothing breaks. An image that reserves space before its first partition sets a
higher `FirstUsableLBA`: 2048 on the rk3588 boards, which keep idbloader and
u-boot down there. `2048 - 32 = 2016`, which points at nothing, and the card
stops booting. Build the same image with `first-lba: 34` and it survives the
identical test — that is the control, if you want one.

You need a Linux VM (any distro with `sgdisk`, `parted` and `python3`) and a
Windows VM, both on the same Hyper-V host. Or you can skip the windows vm and just mount the vhdx directly on the host.

## 1. Build the 48 MB test image

On the Linux VM. Only the partition table matters here, so the partitions stay
empty — no filesystems, nothing to mount. The bug damages the table and leaves
every data sector alone, which is why the symptom is "the board stopped
booting" rather than "the card is blank".

`sfdisk` rather than `sgdisk`, because `first-lba` is the field under test and
`sgdisk` gives no way to set it:

```
# fedora44 used in this example:
sudo dnf install python3 parted util-linux

cd ~

# get tools:
git clone https://github.com/peacepenguin/win32diskimager


# 48 MB exactly. bs=1M would be binary (1048576), so give dd a decimal block.
dd if=/dev/zero of=example.img bs=1000000 count=48

# GPT, two partitions, and first-lba 2048 to reserve room ahead of them the way
# an rk3588 image reserves space for idbloader and u-boot. Sizes are in
# 512-byte sectors: 46875 * 512 = 24,000,000.
sfdisk example.img <<'EOF'
label: gpt
first-lba: 2048
start=32768, size=46875, type=0FC63DAF-8483-4772-8E79-3D69D8477DE4, name="TESTPART1"
start=79872, size=13812, type=0FC63DAF-8483-4772-8E79-3D69D8477DE4, name="TESTPART2"
EOF
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
... OK` for the primary and backup headers alike. Two fields to note, since
they are what the test turns on:

```
FirstUsableLBA   2048
PartEntryLBA     2   count=128 size=128   (array spans 32 sectors, 2..33)
```

The entry array is at LBA 2, immediately after the header, where it belongs and
where it stays. `FirstUsableLBA` is 2048, well clear of it. Windows is about to
conclude that the first of those two numbers can be derived from the second.

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


```

Both tools complain here, and both complaints are the healthy state. `sgdisk`
is the more alarming of the two — it ends with `Identified 1 problems!` — but
read what it actually says:

```
Problem: The secondary header's self-pointer indicates that it doesn't reside
at the end of the disk.
...
Warning: There is a gap between the secondary partition table (ending at sector
93748) and the secondary metadata (sector 93749).
```

That is the same observation `parted` made: the backup GPT is where the *image*
ended, not where the *device* ends. Nothing is damaged, and `sgdisk -e` — the
"fix" it is hinting at — is exactly what this fork's **Fix GPT after write**
option does. Do not run it here; the stranded backup GPT is the condition under
test. The two `doesn't end on a 2048-sector boundary` cautions are unrelated
noise from the partition sizes and can be ignored throughout.

`before.txt` should show three sections, and this is what a *good* disk looks
like:

* **Primary GPT (LBA 1)** — `FirstUsableLBA 2048`, `PartEntryLBA 2`, `HeaderCRC
  ... OK`, `entries CRC over array: ... OK`, and `AlternateLBA 93749` flagged
  against a device last LBA of 203124. That flag is the mid-device backup GPT,
  the setup for the test.
* **Backup GPT where the primary points (LBA 93749)** — `PartEntryLBA 93717`,
  both CRCs `OK`, listing the same two partitions with the same names. The
  backup is intact and consistent with the primary; it is merely early.
* **Backup GPT at device end (LBA 203124)** — `no GPT signature`. Correct: the
  image never wrote anything out there.

Both headers agreeing, with valid CRCs, is the point of the baseline. After
step 5 the primary's `PartEntryLBA` reads 2016 while the array is still at 2,
and the entries CRC stops matching.

Take a copy of the untouched disk on the host, so you can diff sector ranges
afterwards rather than arguing from memory:

```powershell
Copy-Item C:\vms\gpttest.vhdx C:\vms\gpttest-before.vhdx
```

## 5. Show the disk to Windows

Detach from Linux, attach to the Windows VM:

```powershell
Remove-VMHardDiskDrive -VMName 'fedora44-builder' -ControllerType SCSI -ControllerNumber 0 -ControllerLocation 1
Add-VMHardDiskDrive -VMName 'win11-test' -ControllerType SCSI -Path C:\vms\gpttest.vhdx

# or use the host instead of a windows vm:
Mount-DiskImage -ImagePath C:\vms\gpttest.vhdx

# tell windows to 'rescan' (diskpart -> rescan) 
# disk management mmc -> action -> rescan disks
# or powershell:
Update-HostStorageCache

# the rescan initiates the GPT 'fix'. the rescan occurs automatically
# on removeable drives, but scsi drives like vhdx need manual rescan.


```

(`Get-VMHardDiskDrive -VMName 'fedora44-builder'` gives the controller number and location
to remove.)

Now detach it again:

```powershell
Remove-VMHardDiskDrive -VMName 'win11-test' -ControllerType SCSI -ControllerNumber 0 -ControllerLocation 1

# or if using the windows host:
Dismount-DiskImage -ImagePath C:\vms\gpttest.vhdx


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
* The **backup** GPT is entirely correct: relocated to LBA 203124, its entry
  array written at `PartEntryLBA 203092` (last - 32), both CRCs `OK`, both
  partitions listed. Windows did this half properly.
* The **primary** header's `PartEntryLBA` is **2016**, which is
  `FirstUsableLBA - 32`. The entry array is still at LBA 2, untouched.
  `gptdump.py` says so directly:

```
PartEntryLBA     2016   count=128 size=128   (array spans 32 sectors, 2016..2047)
entries CRC over array: 0xab54d286 MISMATCH
** the array matching EntriesCRC is at LBA 2, not 2016 -- the header's PartEntryLBA is wrong by +2014 sectors
```

That is the bug in one line. The header CRC is recomputed and valid, so the
header passes inspection on its own, while `PartitionEntryArrayCRC32` is now
checked against 32 sectors of empty space.

Note which half Windows validates: `parted` reports `The primary GPT table is
corrupt, but the backup appears OK, so that will be used`, and Disk Management
shows a perfectly healthy disk — because it reads the backup, the half that is
fine. Anything reading the primary first, U-Boot included, rejects the table.

No data sector is touched. Only the table is broken, which is exactly why the
symptom is "the board stopped booting" rather than "the card is blank".

## 7. Repeat

The cycle is step 3 through step 6 — re-`dd` the image and you are back to a
known state. No need to recreate the VHDX or the image.

## Notes for a bug report

* **The defect, stated once:** on rescan, Windows rewrites the primary GPT
  header's `PartitionEntryLBA` to `FirstUsableLBA - 32` instead of leaving it
  pointing at the partition entry array, invalidating
  `PartitionEntryArrayCRC32` on any disk whose `FirstUsableLBA` is not 34. The
  relocation of the backup GPT that accompanies it is correct and is not part
  of the complaint.
* It is not specific to SD cards or to removable media: this reproduces on a
  fixed VHDX on a synthetic SCSI controller.
* It affects any image that reserves space ahead of its first partition —
  rk3588 and most ARM boards put idbloader and u-boot below LBA 2048. Images
  with the default `FirstUsableLBA` of 34 are unaffected only because `34 - 32`
  is coincidentally the correct answer.
* Building the same image with `first-lba: 34` and running the identical test
  is a one-command control that shows the table surviving.
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
