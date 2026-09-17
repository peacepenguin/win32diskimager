/**********************************************************************
 *  This program is free software; you can redistribute it and/or     *
 *  modify it under the terms of the GNU General Public License       *
 *  as published by the Free Software Foundation; either version 2    *
 *  of the License, or (at your option) any later version.            *
 *                                                                    *
 *  This program is distributed in the hope that it will be useful,   *
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of    *
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the     *
 *  GNU General Public License for more details.                      *
 *                                                                    *
 *  You should have received a copy of the GNU General Public License *
 *  along with this program; if not, see http://gnu.org/licenses/     *
 *  ---                                                               *
 *  Copyright (C) 2009, Justin Davis <tuxdavis@gmail.com>             *
 *  Copyright (C) 2009-2017 ImageWriter developers                    *
 *                 https://sourceforge.net/projects/win32diskimager/  *
 *  ---                                                               *
 *  Modified 2026 by peacepenguin (fork not affiliated      *
 *  with the upstream ImageWriter project):                           *
 *  GPT repair, device enumeration, volume locking                    *
 *  https://github.com/peacepenguin/windiskimager                   *
 **********************************************************************/

#ifndef DISK_H
#define DISK_H

#ifndef WINVER
#define WINVER 0x0601
#endif

#include <QtWidgets>
#include <QString>
#include <cstdio>
#include <cstdlib>
#include <windows.h>
#include <winioctl.h>
#ifndef FSCTL_IS_VOLUME_MOUNTED
#define FSCTL_IS_VOLUME_MOUNTED  CTL_CODE(FILE_DEVICE_FILE_SYSTEM, 10, METHOD_BUFFERED, FILE_ANY_ACCESS)
#endif // FSCTL_IS_VOLUME_MOUNTED

// Not declared by the MinGW headers.
#ifndef IOCTL_DISK_SET_DISK_ATTRIBUTES
#define IOCTL_DISK_SET_DISK_ATTRIBUTES  CTL_CODE(IOCTL_DISK_BASE, 0x003d, METHOD_BUFFERED, FILE_READ_ACCESS | FILE_WRITE_ACCESS)
#define DISK_ATTRIBUTE_OFFLINE          0x0000000000000001
#define DISK_ATTRIBUTE_READ_ONLY        0x0000000000000002
typedef struct _SET_DISK_ATTRIBUTES
{
    DWORD      Version;
    BOOLEAN    Persist;
    BYTE       Reserved1[3];
    DWORDLONG  Attributes;
    DWORDLONG  AttributesMask;
    DWORD      Reserved2[4];
} SET_DISK_ATTRIBUTES, *PSET_DISK_ATTRIBUTES;
#endif // IOCTL_DISK_SET_DISK_ATTRIBUTES

// IOCTL control code
#define IOCTL_STORAGE_QUERY_PROPERTY   CTL_CODE(IOCTL_STORAGE_BASE, 0x0500, METHOD_BUFFERED, FILE_ANY_ACCESS)

// One physical disk, as offered in the device list. Enumeration goes through
// \\.\PhysicalDriveN rather than through drive letters: a card holding a Linux
// image has no filesystem Windows can mount, so it gets no letter and a
// letter-based scan never sees it at all.
struct PhysicalDevice
{
    ULONG deviceNumber;             // N in \\.\PhysicalDriveN
    QString description;            // vendor + product, as the device reports it
    QString letters;                // "E:, F:", empty when nothing is mounted
    unsigned long long sizeBytes;
    bool removable;                 // removable media, or on the USB/SD/MMC bus
};

// Every physical disk that could plausibly be a target. Removable and
// USB/SD/MMC devices are always listed; the rest only when includeFixed is
// set, for internal card readers that present the card as a fixed disk. The
// disk holding the running Windows installation is never listed.
QList<PhysicalDevice> enumeratePhysicalDevices(bool includeFixed);

// Mounted volumes on physical disk deviceID as "E:, F:"; empty when none.
QString driveLettersOnDevice(ULONG deviceID);

HANDLE getHandleOnFile(LPCWSTR filelocation, DWORD access);
HANDLE getHandleOnDevice(int device, DWORD access);
bool removeLockOnVolume(HANDLE handle);
bool unmountVolume(HANDLE handle);

// A physical disk usually carries more than one volume. Locking only the one
// the user picked leaves the other filesystem drivers live, free to flush
// cached metadata over the image while it is being written.
class LockedVolumes
{
public:
    LockedVolumes() {}
    ~LockedVolumes() { release(); }
    // Lock and dismount every volume that lives on physical disk deviceID.
    bool lockAll(DWORD deviceID);
    void release();
private:
    QList<HANDLE> handles;
};

// ---------------------------------------------------------------------------
// GPT
//
// Writing an image smaller than the device leaves the image's backup GPT
// stranded where the image ends rather than at the end of the disk. Windows
// does not leave that alone: whenever it rescans such a disk it rewrites the
// partition table to match the device. Most of that rewrite is what "sgdisk -e"
// would do and is welcome, but it also recomputes the primary header's
// PartitionEntryLBA as FirstUsableLBA minus the length of the entry array,
// rather than leaving it pointing at the entry array, which has not moved. On
// the ordinary layout -- FirstUsableLBA 34, a 32-sector array at LBA 2 -- the
// wrong formula arrives at the right answer and nothing breaks. On an image
// that reserves space ahead of its first partition, as ARM board images do, it
// points at empty space and the primary table is corrupt.
//
// The functions below either keep that rewrite from being provoked, report
// whether it would do harm, or undo it after the fact. README.md tells the
// whole story; the comments here assume it.
// ---------------------------------------------------------------------------

// Erase any existing partition tables before writing an image, so a previous
// larger image's backup GPT cannot survive at the end of the device and be
// reconciled against the new one. Zeroing both ends removes the trace.
bool wipePartitionTables(HANDLE hRawDisk, unsigned long long sectorsize,
                         unsigned long long devicesectors);

// Result of relocateBackupGPT().
enum GptFixResult
{
    GPT_FIX_OK,          // table rewritten to match the device
    GPT_FIX_NOT_NEEDED,  // already consistent with the device size
    GPT_FIX_DISABLED,    // not attempted; "Fix GPT after write" is unchecked
    GPT_FIX_NO_GPT,      // the device holds no GPT at all; nothing to repair
    GPT_FIX_BAD_GPT,     // a GPT is present but malformed; nothing was touched
    GPT_FIX_FAILED       // an I/O error occurred
};

// Move the backup GPT to the true last LBA of the device and update
// AlternateLBA/LastUsableLBA to match, the way "sgdisk -e" does. Making the
// table consistent with the device ourselves leaves Windows nothing to rewrite.
GptFixResult relocateBackupGPT(HANDLE hRawDisk, unsigned long long sectorsize,
                               unsigned long long devicesectors, QString *detail);

// True when sector 0 carries an MBR boot signature and at least one partition
// entry with a type. Used only to tell "this image is MBR" from "this image has
// no partition table at all" when reporting that there is no GPT to repair.
bool deviceHasMbrTable(HANDLE hRawDisk, unsigned long long sectorsize);

// Whether this table would survive the rewrite.
enum GptRewriteRisk
{
    GPT_RISK_UNKNOWN,   // could not be determined
    GPT_RISK_NO_GPT,    // no GPT on the device; nothing to rewrite
    GPT_RISK_SAFE,      // the rewrite would land on the correct value anyway
    GPT_RISK_AFFECTED   // the rewrite would invalidate the primary table
};

// Compare the real PartitionEntryLBA against the value Windows would compute
// for it. They differ exactly when the image reserves space ahead of its first
// partition, which is what makes ARM board images vulnerable and ordinary ones
// immune.
GptRewriteRisk gptRewriteRisk(HANDLE hRawDisk, unsigned long long sectorsize);

// What state the device's primary GPT is in, judged against itself.
enum GptPrimaryState
{
    GPT_PRIMARY_OK,       // the header and the entry array it points at agree
    GPT_PRIMARY_NO_GPT,   // there is no GPT here
    GPT_PRIMARY_UNKNOWN,  // unreadable, or damaged in some other way
    GPT_PRIMARY_BROKEN    // the header checks out but points at the wrong entries
};

// Detect a primary table that has been left pointing somewhere the partition
// entries are not. Windows recomputes the header checksum over the value it
// wrote, so the header passes its own CRC while PartitionEntryArrayCRC32 no
// longer describes what it points at. Nothing reading the table will accept it.
GptPrimaryState gptPrimaryState(HANDLE hRawDisk, unsigned long long sectorsize,
                                unsigned long long devicesectors);

// Point the primary header back at the partition entries. The rewrite moves
// the pointer but leaves PartitionEntryArrayCRC32 alone, so the header still
// records what the real entries hash to -- that checksum is what finds them
// again. No data sector is touched. Returns false, with the reason in *detail,
// when the damage is not this shape.
bool repairPrimaryGpt(HANDLE hRawDisk, unsigned long long sectorsize,
                      unsigned long long devicesectors, QString *detail);

// Where the image's own backup GPT sits, read from the image's header rather
// than the device. The fix zeroes that stale copy after relocating it, so those
// sectors differ from the image by design -- and once it has run, the device no
// longer records where the copy used to be. lba1 is the image's sector 1.
// Returns false if the image holds no usable GPT.
bool gptImageBackupRange(const unsigned char *lba1, unsigned long long sectorsize,
                         unsigned long long *first, unsigned long long *last);

// Report the sectors that "Fix GPT after write" may rewrite, so a verify can
// tell a deliberate GPT rewrite apart from a bad card. The front range
// [0, *frontend) covers the protective MBR, the primary header and the primary
// entry array; the tail range [*tailstart, devicesectors) covers the relocated
// backup entry array and header. Returns false if the device holds no usable
// GPT, in which case neither output is set.
bool gptOwnedSectors(HANDLE hRawDisk, unsigned long long sectorsize,
                     unsigned long long devicesectors,
                     unsigned long long *frontend, unsigned long long *tailstart);

// One partition, as planGptShrink()/planMbrShrink() repack it: sectors
// [srcfirst, srcfirst + length) on the device become
// [dstfirst, dstfirst + length) in the image.
struct ShrinkCopyRange
{
    unsigned long long srcfirst;
    unsigned long long dstfirst;
    unsigned long long length;
};

// A "Shrink image on Read" plan, from either planGptShrink() or
// planMbrShrink() -- the two only differ in what goes into headerregion and
// whether there is a backupregion at all, so callers drive both the same way.
struct PartitionShrinkPlan
{
    // Sectors [0, headersectors) of the image, verbatim except for the
    // partition table itself, which is patched here to describe the repacked
    // partitions below. For GPT this covers the protective MBR, the primary
    // header and entry array, and whatever reserved space (e.g. an ARM
    // board's U-Boot) an image keeps ahead of FirstUsableLBA; for MBR it is
    // just the boot sector. Either way, nothing here is repacked, only the
    // table entries describing what comes after it.
    QByteArray headerregion;
    unsigned long long headersectors;
    // Every in-use partition, packed back-to-back right after headerregion
    // with no gaps, each aligned to the caller's alignsectors, in the order
    // it originally started on the device. Between headerregion and the
    // first range, and between two ranges, alignment may leave a gap that
    // has to be written as explicit zero sectors -- there is no partition
    // data to read for it, and unlike a plain contiguous read there is no
    // guarantee the caller's output is a sparse file that zero-fills a
    // skipped-over region on its own.
    QList<ShrinkCopyRange> ranges;
    // Sectors [totalsectors - backupsectors, totalsectors) of the image: a
    // fresh backup entry array and header, already computed against the
    // repacked layout above. Written once every range has actually been
    // copied that short, this is what makes the primary header (already
    // patched into headerregion) correct -- no read-modify-write against the
    // device or the image is needed afterward, which is what lets this work
    // for a compressed output stream and not just a raw file. Empty, with
    // backupsectors 0, for an MBR plan: there is no backup table to build.
    QByteArray backupregion;
    unsigned long long backupsectors;
    // The size, in sectors, the image should be read to.
    unsigned long long totalsectors;
};

// Plan a "Shrink image on Read" that removes every unpartitioned gap on a GPT
// device -- between FirstUsableLBA and the first partition, between
// partitions, and after the last one -- rather than only the trailing one.
// Partitions are repacked in their original order and each is aligned to
// alignsectors (pass 4096 / sectorsize, so an image made from a 512-byte-
// sector device still starts every partition on a 4K boundary, matching what
// 4Kn media needs). Returns false, with *plan untouched, if the device holds
// no usable GPT, a partition's range makes no sense, or there is nothing to
// gain by repacking.
bool planGptShrink(HANDLE hRawDisk, unsigned long long sectorsize,
                   unsigned long long devicesectors, unsigned long long alignsectors,
                   PartitionShrinkPlan *plan, QString *detail);

// The same idea for a legacy MBR: every unpartitioned gap goes -- after the
// boot sector and before the first partition, between partitions, and after
// the last one -- packed and aligned exactly as planGptShrink() does. Only
// the four primary entries are walked; extended/logical partitions are not.
// There being no backup table to build, plan->backupregion is left empty and
// plan->backupsectors 0. Returns false, with *plan untouched, if the device
// holds no MBR, an entry describes an impossible range, or there is nothing
// to gain by repacking.
bool planMbrShrink(HANDLE hRawDisk, unsigned long long sectorsize,
                   unsigned long long devicesectors, unsigned long long alignsectors,
                   PartitionShrinkPlan *plan, QString *detail);

bool flushDevice(HANDLE handle);
bool setDiskOffline(HANDLE handle, bool offline);
bool ejectDevice(HANDLE handle);
char *readSectorDataFromHandle(HANDLE handle, unsigned long long startsector, unsigned long long numsectors, unsigned long long sectorsize);
bool writeSectorDataToHandle(HANDLE handle, char *data, unsigned long long startsector, unsigned long long numsectors, unsigned long long sectorsize);
// Sectors on the device, or 0. *reported is set when the failure has already
// been put in front of the user, so the caller can stay quiet rather than
// explain it a second time and differently.
unsigned long long getNumberOfSectors(HANDLE handle, unsigned long long *sectorsize,
                                      bool *reported = NULL);
unsigned long long getFileSizeInSectors(HANDLE handle, unsigned long long sectorsize);
// Free space on the volume that holds `location`, which is a directory. Taken
// as a QString and asked over the wide API: an image sitting under a user
// directory whose name is not ASCII cannot be named in the ANSI code page, and
// the check would quietly be skipped for everyone whose name is spelled that
// way.
bool spaceAvailable(const QString &location, unsigned long long spaceneeded);

#endif // DISK_H
