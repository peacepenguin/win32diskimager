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

typedef struct _DEVICE_NUMBER
{
    DEVICE_TYPE  DeviceType;
    ULONG  DeviceNumber;
    ULONG  PartitionNumber;
} DEVICE_NUMBER, *PDEVICE_NUMBER;

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

HANDLE getHandleOnFile(LPCWSTR filelocation, DWORD access);
HANDLE getHandleOnDevice(int device, DWORD access);
HANDLE getHandleOnVolume(int volume, DWORD access);
QString getDriveLabel(const char *drv);
DWORD getDeviceID(HANDLE handle);
bool getLockOnVolume(HANDLE handle);
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

// Erase any existing partition tables before writing an image. A card that
// previously held a larger image still has that image's backup GPT sitting at
// the very end of the device; writing a smaller image over the front leaves it
// there, and Windows then tries to reconcile the new primary header with the
// stale backup. Zeroing both ends first removes the trace.
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
// AlternateLBA/LastUsableLBA to match, the way "sgdisk -e" does. Writing an
// image smaller than the target leaves the backup GPT stranded mid-device;
// Windows treats that as damage and silently rewrites the table. Making the
// table consistent ourselves removes the trigger.
GptFixResult relocateBackupGPT(HANDLE hRawDisk, unsigned long long sectorsize,
                               unsigned long long devicesectors, QString *detail);

bool flushDevice(HANDLE handle);
bool setDiskOffline(HANDLE handle, bool offline);
bool ejectDevice(HANDLE handle);
bool isVolumeUnmounted(HANDLE handle);
char *readSectorDataFromHandle(HANDLE handle, unsigned long long startsector, unsigned long long numsectors, unsigned long long sectorsize);
bool writeSectorDataToHandle(HANDLE handle, char *data, unsigned long long startsector, unsigned long long numsectors, unsigned long long sectorsize);
unsigned long long getNumberOfSectors(HANDLE handle, unsigned long long *sectorsize);
unsigned long long getFileSizeInSectors(HANDLE handle, unsigned long long sectorsize);
bool spaceAvailable(char *location, unsigned long long spaceneeded);
bool checkDriveType(char *name, ULONG *pid);

#endif // DISK_H
