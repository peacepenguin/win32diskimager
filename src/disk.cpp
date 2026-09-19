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

#ifndef WINVER
#define WINVER 0x0601
#endif

#include <QtWidgets>
#include <algorithm>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <new>
#include <windows.h>
#include <winioctl.h>
#include "disk.h"
#include "mainwindow.h"

// Report a Win32 failure with the system's own description of it. `message`
// carries %1 for the error code and %2 for that description, or the next two
// free placeholders when the caller has already filled some in.
//
// The code is read once, up front: FormatMessageW and the dialog can both
// overwrite it, so reading it again at the end -- as all ten copies of this
// did -- risks reporting an error other than the one that happened.
static void reportWin32Error(const QString &title, const QString &message)
{
    DWORD code = GetLastError();
    wchar_t *text = NULL;
    FormatMessageW(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_ALLOCATE_BUFFER,
                   NULL, code, 0, (LPWSTR)&text, 0, NULL);
    QMessageBox::critical(MainWindow::getInstanceIfAvailable(), title,
                          message.arg(code)
                                 .arg(text ? QString::fromUtf16((const char16_t *)text)
                                           : QString()));
    LocalFree(text);
}

HANDLE getHandleOnFile(LPCWSTR filelocation, DWORD access)
{
    HANDLE hFile;
    hFile = CreateFileW(filelocation, access, (access == GENERIC_READ) ? FILE_SHARE_READ : 0, NULL, (access == GENERIC_READ) ? OPEN_EXISTING:CREATE_ALWAYS, 0, NULL);
    if (hFile == INVALID_HANDLE_VALUE)
    {
        reportWin32Error(QObject::tr("File Error"),
                         QObject::tr("An error occurred when attempting to get a handle on the file.\n"
                         "Error %1: %2"));
    }
    return hFile;
}

HANDLE getHandleOnDevice(int device, DWORD access)
{
    HANDLE hDevice;
    QString devicename = QString("\\\\.\\PhysicalDrive%1").arg(device);
    // Prefer sharing reads only: allowing concurrent writers lets Windows
    // modify the partition table underneath us while the image is written.
    // Fall back to the permissive mode rather than failing outright, since an
    // exclusive open is refused if anything still holds the disk.
    hDevice = CreateFile(devicename.toLatin1().data(), access, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL);
    if (hDevice == INVALID_HANDLE_VALUE)
    {
        hDevice = CreateFile(devicename.toLatin1().data(), access,
                             FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, 0, NULL);
    }
    if (hDevice == INVALID_HANDLE_VALUE)
    {
        reportWin32Error(QObject::tr("Device Error"),
                         QObject::tr("An error occurred when attempting to get a handle on the device.\n"
                         "Error %1: %2"));
    }
    return hDevice;
}

bool removeLockOnVolume(HANDLE handle)
{
    DWORD junk;
    BOOL bResult;
    bResult = DeviceIoControl(handle, FSCTL_UNLOCK_VOLUME, NULL, 0, NULL, 0, &junk, NULL);
    if (!bResult)
    {
        reportWin32Error(QObject::tr("Unlock Error"),
                         QObject::tr("An error occurred when attempting to unlock the volume.\n"
                         "Error %1: %2"));
    }
    return (bResult);
}

bool unmountVolume(HANDLE handle)
{
    DWORD junk;
    BOOL bResult;
    bResult = DeviceIoControl(handle, FSCTL_DISMOUNT_VOLUME, NULL, 0, NULL, 0, &junk, NULL);
    if (!bResult)
    {
        reportWin32Error(QObject::tr("Dismount Error"),
                         QObject::tr("An error occurred when attempting to dismount the volume.\n"
                         "Error %1: %2"));
    }
    return (bResult);
}

char *readSectorDataFromHandle(HANDLE handle, unsigned long long startsector, unsigned long long numsectors, unsigned long long sectorsize)
{
    // Add overflow check
    if (sectorsize == 0 || numsectors > ULLONG_MAX / sectorsize) {
        reportWin32Error(QObject::tr("Read Error"),
                         QObject::tr("Sector count too large."));
        return NULL;
    }

    unsigned long bytesread;
    char *data = new(std::nothrow) char[sectorsize * numsectors];
    if (!data)
    {
        reportWin32Error(QObject::tr("Read Error"),
                         QObject::tr("Unable to allocate memory for read buffer."));
        return NULL;
    }
    LARGE_INTEGER li;
    li.QuadPart = startsector * sectorsize;
    // Checked the way rawSeekRead does it. A seek that silently failed would
    // read from wherever the pointer happened to be.
    if (SetFilePointer(handle, li.LowPart, &li.HighPart, FILE_BEGIN) == INVALID_SET_FILE_POINTER
        && GetLastError() != NO_ERROR)
    {
        reportWin32Error(QObject::tr("Read Error"),
                         QObject::tr("An error occurred when attempting to read data from handle.\n"
                         "Error %1: %2"));
        delete[] data;
        return NULL;
    }
    if (!ReadFile(handle, data, sectorsize * numsectors, &bytesread, NULL))
    {
        reportWin32Error(QObject::tr("Read Error"),
                         QObject::tr("An error occurred when attempting to read data from handle.\n"
                         "Error %1: %2"));
        delete[] data;
        data = NULL;
    }
    if (data && bytesread < (sectorsize * numsectors))
    {
            memset(data + bytesread,0,(sectorsize * numsectors) - bytesread);
    }
    return data;
}

bool writeSectorDataToHandle(HANDLE handle, char *data, unsigned long long startsector, unsigned long long numsectors, unsigned long long sectorsize)
{
    unsigned long byteswritten;
    BOOL bResult;
    LARGE_INTEGER li;
    li.QuadPart = startsector * sectorsize;
    // Checked, and this one matters most: a seek that silently failed would put
    // this chunk of the image somewhere else on the device entirely.
    if (SetFilePointer(handle, li.LowPart, &li.HighPart, FILE_BEGIN) == INVALID_SET_FILE_POINTER
        && GetLastError() != NO_ERROR)
    {
        reportWin32Error(QObject::tr("Write Error"),
                         QObject::tr("An error occurred when attempting to write data to handle.\n"
                         "Error %1: %2"));
        return false;
    }
    bResult = WriteFile(handle, data, sectorsize * numsectors, &byteswritten, NULL);
    if (!bResult)
    {
        reportWin32Error(QObject::tr("Write Error"),
                         QObject::tr("An error occurred when attempting to write data to handle.\n"
                         "Error %1: %2"));
        return false;
    }
    if (byteswritten != sectorsize * numsectors)
    {
        // WriteFile can report success having written less than it was asked
        // to. Counting that as a whole chunk leaves a hole in the image on the
        // device that nothing notices until it fails to boot.
        QMessageBox::critical(MainWindow::getInstanceIfAvailable(), QObject::tr("Write Error"),
            QObject::tr("The device took only %1 of %2 bytes. The image on the device "
                        "is incomplete.")
                .arg((qulonglong)byteswritten)
                .arg((qulonglong)(sectorsize * numsectors)));
        return false;
    }
    return true;
}

unsigned long long getNumberOfSectors(HANDLE handle, unsigned long long *sectorsize,
                                      bool *reported)
{
    DWORD junk;
    DISK_GEOMETRY_EX diskgeometry;
    BOOL bResult;
    bResult = DeviceIoControl(handle, IOCTL_DISK_GET_DRIVE_GEOMETRY_EX, NULL, 0, &diskgeometry, sizeof(diskgeometry), &junk, NULL);
    if (!bResult)
    {
        reportWin32Error(QObject::tr("Device Error"),
                         QObject::tr("An error occurred when attempting to get the device's geometry.\n"
                         "Error %1: %2"));
        // Tell the caller this was reported, so it does not stack a second
        // dialog blaming a removed card for what was an ioctl failure.
        if (reported != NULL) *reported = true;
        return 0;
    }
    if (sectorsize != NULL)
    {
        *sectorsize = (unsigned long long)diskgeometry.Geometry.BytesPerSector;
    }
    if (diskgeometry.Geometry.BytesPerSector == 0)
    {
        // Nothing else here divides by a sector size without checking it first,
        // and this is the one place that would fault the process rather than
        // return something wrong. Zero is a size every caller already stops on;
        // what they then say -- that the device reports no size -- is near
        // enough to a device that reports no sector size.
        return 0;
    }
    return (unsigned long long)diskgeometry.DiskSize.QuadPart / (unsigned long long)diskgeometry.Geometry.BytesPerSector;
}

unsigned long long getFileSizeInSectors(HANDLE handle, unsigned long long sectorsize)
{
    unsigned long long retVal = 0;
    if (sectorsize) // avoid divide by 0
    {
        LARGE_INTEGER filesize;
        if(GetFileSizeEx(handle, &filesize) == 0)
        {
            // error
            reportWin32Error(QObject::tr("File Error"),
                             QObject::tr("An error occurred while getting the file size.\n"
                             "Error %1: %2"));
            retVal = 0;
        }
        else
        {
            retVal = ((unsigned long long)filesize.QuadPart / sectorsize ) + (((unsigned long long)filesize.QuadPart % sectorsize )?1:0);
        }
    }
    return(retVal);
}

bool spaceAvailable(const QString &location, unsigned long long spaceneeded)
{
    ULARGE_INTEGER freespace;
    BOOL bResult;
    bResult = GetDiskFreeSpaceExW((LPCWSTR)location.utf16(), NULL, NULL, &freespace);
    if (!bResult)
    {
        reportWin32Error(QObject::tr("Free Space Error"),
                         QObject::tr("Failed to get the free space on the volume holding %1.\n"
                                     "Error %2: %3\n"
                                     "Checking of free space will be skipped.").arg(location));
        return true;
    }
    return (spaceneeded <= freespace.QuadPart);
}




// Open a volume by drive letter and report which physical disk it lives on.
// access is what CreateFile is asked for: 0 puts the question without needing
// the volume to be readable by us and without disturbing whatever else has it
// open, while a caller that means to lock the volume asks for read and write.
// Returns INVALID_HANDLE_VALUE unless the volume opened and named a disk, so
// *disk is set whenever a handle comes back.
static HANDLE openVolumeOnDisk(char letter, DWORD access, int *disk)
{
    char volumename[] = "\\\\.\\A:";
    volumename[4] = letter;
    HANDLE h = CreateFile(volumename, access, FILE_SHARE_READ | FILE_SHARE_WRITE,
                          NULL, OPEN_EXISTING, 0, NULL);
    if (h == INVALID_HANDLE_VALUE)
    {
        return INVALID_HANDLE_VALUE;
    }
    VOLUME_DISK_EXTENTS sd;
    DWORD bytesreturned;
    if (!DeviceIoControl(h, IOCTL_VOLUME_GET_VOLUME_DISK_EXTENTS, NULL, 0,
                         &sd, sizeof(sd), &bytesreturned, NULL)
        || sd.NumberOfDiskExtents == 0)
    {
        CloseHandle(h);
        return INVALID_HANDLE_VALUE;
    }
    *disk = (int)sd.Extents[0].DiskNumber;
    return h;
}

// Physical disk a mounted volume lives on, or -1 if it cannot be determined.
static int diskNumberOfVolume(char letter)
{
    int disk = -1;
    HANDLE h = openVolumeOnDisk(letter, 0, &disk);
    if (h == INVALID_HANDLE_VALUE)
    {
        return -1;
    }
    CloseHandle(h);
    return disk;
}

QString driveLettersOnDevice(ULONG deviceID)
{
    QStringList found;
    unsigned long driveMask = GetLogicalDrives();
    for (int i = 0; i < 26; ++i)
    {
        if (!(driveMask & (1ul << i)))
        {
            continue;
        }
        char letter = 'A' + i;
        if (diskNumberOfVolume(letter) == (int)deviceID)
        {
            found.append(QString("%1:").arg(QChar(letter)));
        }
    }
    return found.join(", ");
}

QMap<unsigned long long, QString> driveLettersByOffset(ULONG deviceID)
{
    QMap<unsigned long long, QString> found;
    unsigned long driveMask = GetLogicalDrives();
    for (int i = 0; i < 26; ++i)
    {
        if (!(driveMask & (1ul << i)))
        {
            continue;
        }
        char letter = 'A' + i;
        char volumename[] = "\\\\.\\A:";
        volumename[4] = letter;
        HANDLE h = CreateFile(volumename, 0, FILE_SHARE_READ | FILE_SHARE_WRITE,
                              NULL, OPEN_EXISTING, 0, NULL);
        if (h == INVALID_HANDLE_VALUE)
        {
            continue;
        }
        VOLUME_DISK_EXTENTS sd;
        DWORD bytesreturned;
        if (DeviceIoControl(h, IOCTL_VOLUME_GET_VOLUME_DISK_EXTENTS, NULL, 0,
                            &sd, sizeof(sd), &bytesreturned, NULL)
            && sd.NumberOfDiskExtents > 0
            && sd.Extents[0].DiskNumber == deviceID)
        {
            found.insert((unsigned long long)sd.Extents[0].StartingOffset.QuadPart,
                        QString("%1:").arg(QChar(letter)));
        }
        CloseHandle(h);
    }
    return found;
}

bool diskPartitionNumbers(HANDLE hRawDisk, unsigned long long sectorsize,
                          QMap<unsigned long long, int> *numbersBySector)
{
    if (sectorsize == 0)
    {
        return false;
    }
    // The partition count is not known ahead of time, so this grows the
    // buffer and retries until the ioctl stops asking for more room. 32
    // entries covers any real device on the first try; the cap just keeps a
    // pathological reply from growing forever.
    DWORD size = sizeof(DRIVE_LAYOUT_INFORMATION_EX) + 32 * sizeof(PARTITION_INFORMATION_EX);
    QByteArray buf(size, 0);
    DWORD bytesreturned = 0;
    for (;;)
    {
        if (DeviceIoControl(hRawDisk, IOCTL_DISK_GET_DRIVE_LAYOUT_EX, NULL, 0,
                            buf.data(), size, &bytesreturned, NULL))
        {
            break;
        }
        if (GetLastError() != ERROR_INSUFFICIENT_BUFFER || size > 4u * 1024u * 1024u)
        {
            return false;
        }
        size *= 2;
        buf.resize(size);
    }

    const DRIVE_LAYOUT_INFORMATION_EX *layout =
        (const DRIVE_LAYOUT_INFORMATION_EX *)buf.constData();
    numbersBySector->clear();
    for (DWORD i = 0; i < layout->PartitionCount; ++i)
    {
        const PARTITION_INFORMATION_EX &p = layout->PartitionEntry[i];
        // An unused table slot is reported with a partition number of 0
        // (GPT) or an MBR type byte of 0; neither is a real partition.
        if (p.PartitionNumber == 0)
        {
            continue;
        }
        if (p.PartitionStyle == PARTITION_STYLE_MBR && p.Mbr.PartitionType == 0)
        {
            continue;
        }
        unsigned long long sector = (unsigned long long)p.StartingOffset.QuadPart / sectorsize;
        numbersBySector->insert(sector, (int)p.PartitionNumber);
    }
    return true;
}

// Disk that holds the running Windows installation, or -1 if unknown.
static int systemDiskNumber()
{
    char windir[MAX_PATH + 1] = {0};
    if (GetWindowsDirectoryA(windir, MAX_PATH) == 0)
    {
        return -1;
    }
    return diskNumberOfVolume(windir[0]);
}

// A trailing run of spaces and NULs is normal in the descriptor strings.
static QString descriptorString(const BYTE *buf, DWORD offset)
{
    if (offset == 0)
    {
        return QString();
    }
    return QString::fromLatin1((const char *)buf + offset).trimmed();
}

QList<PhysicalDevice> enumeratePhysicalDevices(bool includeFixed)
{
    QList<PhysicalDevice> devices;
    const int systemDisk = systemDiskNumber();

    // 128 covers anything a machine is likely to have attached; the numbers are
    // not dense, so the loop cannot stop at the first gap.
    for (ULONG n = 0; n < 128; ++n)
    {
        QString devicename = QString("\\\\.\\PhysicalDrive%1").arg(n);
        // No access rights, for the same reason as openVolumeOnDisk above.
        HANDLE hDevice = CreateFile(devicename.toLatin1().data(), 0,
                                    FILE_SHARE_READ | FILE_SHARE_WRITE, NULL,
                                    OPEN_EXISTING, 0, NULL);
        if (hDevice == INVALID_HANDLE_VALUE)
        {
            continue;
        }

        PhysicalDevice dev;
        dev.deviceNumber = n;
        dev.sizeBytes = 0;
        dev.removable = false;

        int arrSz = sizeof(STORAGE_DEVICE_DESCRIPTOR) + 512 - 1;
        BYTE *buf = new BYTE[arrSz];
        PSTORAGE_DEVICE_DESCRIPTOR pDevDesc = (PSTORAGE_DEVICE_DESCRIPTOR)buf;
        pDevDesc->Size = arrSz;
        STORAGE_PROPERTY_QUERY query;
        query.PropertyId = StorageDeviceProperty;
        query.QueryType = PropertyStandardQuery;
        DWORD dwOutBytes;
        bool described = DeviceIoControl(hDevice, IOCTL_STORAGE_QUERY_PROPERTY,
                                         &query, sizeof(query), pDevDesc,
                                         pDevDesc->Size, &dwOutBytes, NULL);
        if (described)
        {
            QString vendor = descriptorString(buf, pDevDesc->VendorIdOffset);
            QString product = descriptorString(buf, pDevDesc->ProductIdOffset);
            dev.description = QString("%1 %2").arg(vendor).arg(product).trimmed();
            // eSATA reports removable media but is a fixed internal disk in
            // practice, so it is only offered when fixed disks are shown.
            dev.removable = (pDevDesc->RemovableMedia && pDevDesc->BusType != BusTypeSata)
                            || pDevDesc->BusType == BusTypeUsb
                            || pDevDesc->BusType == BusTypeSd
                            || pDevDesc->BusType == BusTypeMmc;
        }
        delete[] buf;

        // The two filters that can be settled without a size are applied here,
        // ahead of the geometry call, and a disk failing either is dropped
        // without ever being asked how big it is.
        //
        // Asking is what wakes a sleeping disk. Measured on a spun-down 4 TB
        // SATA drive: opening the handle 0.8 ms, the descriptor query above
        // 0.0 ms, the geometry call below 11,911 ms. The disk was still in
        // standby after the descriptor query -- which is how we know the
        // geometry call spun it up, and not the open or the classification.
        //
        // Nothing cheaper answers either: a drive in standby spins up for any
        // command that needs the media, and capacity is not one of the
        // exceptions. So the fix is not to ask more cheaply but not to ask at
        // all about a disk that is about to be discarded. With "Show all
        // devices" off -- the default, and where every session starts -- the
        // machine's internal disks are classified out of metadata their
        // drivers already hold and their platters are never touched. Ticking
        // it asks for them by name, and the wait is the price of a real size.
        //
        // This is ordering only. A device still has to pass every one of these
        // tests, so the list itself is unchanged.

        // Never offer the disk Windows is running from, whatever the filter.
        if (systemDisk >= 0 && (int)n == systemDisk)
        {
            CloseHandle(hDevice);
            continue;
        }
        if (!dev.removable && !includeFixed)
        {
            CloseHandle(hDevice);
            continue;
        }

        // A card reader with no card in it still has a PhysicalDrive node, but
        // reports no size. Size doubles as the "media present" test that
        // IOCTL_STORAGE_CHECK_VERIFY used to provide.
        //
        // Geometry rather than IOCTL_DISK_GET_LENGTH_INFO: the latter demands
        // FILE_READ_ACCESS on the handle and returns nothing for a handle
        // opened purely to query, which would empty the list entirely.
        DISK_GEOMETRY_EX geometry;
        DWORD junk;
        if (DeviceIoControl(hDevice, IOCTL_DISK_GET_DRIVE_GEOMETRY_EX, NULL, 0,
                            &geometry, sizeof(geometry), &junk, NULL))
        {
            dev.sizeBytes = (unsigned long long)geometry.DiskSize.QuadPart;
        }
        CloseHandle(hDevice);

        if (dev.sizeBytes == 0)
        {
            continue;
        }
        if (dev.description.isEmpty())
        {
            dev.description = QObject::tr("Unknown device");
        }
        dev.letters = driveLettersOnDevice(n);
        devices.append(dev);
    }
    return devices;
}

bool LockedVolumes::lockAll(DWORD deviceID)
{
    // Volumes on other disks are left alone; only the target disk is locked.
    for (int i = 0; i < 26; ++i)
    {
        char root[] = "A:\\";
        root[0] = 'A' + i;
        UINT type = GetDriveTypeA(root);
        if (type != DRIVE_REMOVABLE && type != DRIVE_FIXED)
        {
            continue;
        }
        int disk = -1;
        HANDLE h = openVolumeOnDisk((char)('A' + i),
                                    GENERIC_READ | GENERIC_WRITE, &disk);
        if (h == INVALID_HANDLE_VALUE)
        {
            continue;
        }
        if (disk != (int)deviceID)
        {
            CloseHandle(h);
            continue;
        }
        // A volume that is merely busy (indexer, antivirus, an open Explorer
        // window) fails FSCTL_LOCK_VOLUME with ERROR_ACCESS_DENIED, so retry
        // rather than giving up on the first refusal.
        bool gotlock = false;
        DWORD junk;
        for (int attempt = 0; attempt < 20 && !gotlock; ++attempt)
        {
            gotlock = DeviceIoControl(h, FSCTL_LOCK_VOLUME, NULL, 0, NULL, 0, &junk, NULL);
            if (!gotlock)
            {
                Sleep(100);
            }
        }
        if (!gotlock)
        {
            QMessageBox::critical(MainWindow::getInstanceIfAvailable(), QObject::tr("Lock Error"),
                                  QObject::tr("Could not lock volume %1: it is still in use.\n"
                                              "Close any program using the device and try again.\n"
                                              "Error %2").arg(QChar('A' + i)).arg(GetLastError()));
            CloseHandle(h);
            release();
            return false;
        }
        if (!unmountVolume(h))
        {
            CloseHandle(h);
            release();
            return false;
        }
        handles.append(h);
    }
    return true;
}

void LockedVolumes::release()
{
    for (int i = 0; i < handles.size(); ++i)
    {
        removeLockOnVolume(handles.at(i));
        CloseHandle(handles.at(i));
    }
    handles.clear();
}

bool flushDevice(HANDLE handle)
{
    // Deliberately no IOCTL_DISK_UPDATE_PROPERTIES here: asking Windows to
    // re-read the partition table is what triggers the automatic GPT "repair"
    // that rewrites the table we just wrote.
    return FlushFileBuffers(handle);
}

bool setDiskOffline(HANDLE handle, bool offline)
{
    SET_DISK_ATTRIBUTES sda;
    DWORD junk;
    ZeroMemory(&sda, sizeof(sda));
    sda.Version = sizeof(sda);
    sda.Persist = FALSE;
    sda.Attributes = offline ? DISK_ATTRIBUTE_OFFLINE : 0;
    sda.AttributesMask = DISK_ATTRIBUTE_OFFLINE;
    return DeviceIoControl(handle, IOCTL_DISK_SET_DISK_ATTRIBUTES, &sda, sizeof(sda),
                           NULL, 0, &junk, NULL);
}

bool ejectDevice(HANDLE handle)
{
    DWORD junk;
    PREVENT_MEDIA_REMOVAL pmr;
    pmr.PreventMediaRemoval = FALSE;
    DeviceIoControl(handle, IOCTL_STORAGE_MEDIA_REMOVAL, &pmr, sizeof(pmr), NULL, 0, &junk, NULL);
    return DeviceIoControl(handle, IOCTL_STORAGE_EJECT_MEDIA, NULL, 0, NULL, 0, &junk, NULL);
}

// ---------------------------------------------------------------------------
// GPT repair
// ---------------------------------------------------------------------------

// Offsets within the 92-byte GPT header (UEFI 2.x, section 5.3).
#define GPT_OFF_SIGNATURE      0
#define GPT_OFF_HEADERSIZE    12
#define GPT_OFF_HEADERCRC     16
#define GPT_OFF_MYLBA         24
#define GPT_OFF_ALTLBA        32
#define GPT_OFF_FIRSTUSABLE   40
#define GPT_OFF_LASTUSABLE    48
#define GPT_OFF_ENTRYLBA      72
#define GPT_OFF_NUMENTRIES    80
#define GPT_OFF_ENTRYSIZE     84
#define GPT_OFF_ENTRIESCRC    88
// Offsets within one partition entry.
#define GPT_ENT_TYPEGUID       0
#define GPT_ENT_FIRSTLBA      32
#define GPT_ENT_LASTLBA       40

static DWORD gptCrc32(const unsigned char *data, size_t len)
{
    static DWORD table[256];
    static bool built = false;
    if (!built)
    {
        for (DWORD i = 0; i < 256; ++i)
        {
            DWORD c = i;
            for (int k = 0; k < 8; ++k)
            {
                c = (c & 1) ? (0xEDB88320u ^ (c >> 1)) : (c >> 1);
            }
            table[i] = c;
        }
        built = true;
    }
    DWORD crc = 0xFFFFFFFFu;
    for (size_t i = 0; i < len; ++i)
    {
        crc = table[(crc ^ data[i]) & 0xFF] ^ (crc >> 8);
    }
    return crc ^ 0xFFFFFFFFu;
}

static DWORD rd32(const unsigned char *p, int off)
{
    return (DWORD)p[off] | ((DWORD)p[off+1] << 8) | ((DWORD)p[off+2] << 16) | ((DWORD)p[off+3] << 24);
}

static unsigned long long rd64(const unsigned char *p, int off)
{
    unsigned long long v = 0;
    for (int i = 7; i >= 0; --i)
    {
        v = (v << 8) | p[off + i];
    }
    return v;
}

static void wr32(unsigned char *p, int off, DWORD v)
{
    for (int i = 0; i < 4; ++i)
    {
        p[off + i] = (unsigned char)((v >> (8 * i)) & 0xFF);
    }
}

static void wr64(unsigned char *p, int off, unsigned long long v)
{
    for (int i = 0; i < 8; ++i)
    {
        p[off + i] = (unsigned char)((v >> (8 * i)) & 0xFF);
    }
}

static bool rawSeekRead(HANDLE h, unsigned long long offset, void *buf, DWORD len)
{
    LARGE_INTEGER li;
    DWORD got = 0;
    li.QuadPart = (LONGLONG)offset;
    if (SetFilePointer(h, li.LowPart, &li.HighPart, FILE_BEGIN) == INVALID_SET_FILE_POINTER
        && GetLastError() != NO_ERROR)
    {
        return false;
    }
    return ReadFile(h, buf, len, &got, NULL) && got == len;
}

static bool rawSeekWrite(HANDLE h, unsigned long long offset, const void *buf, DWORD len)
{
    LARGE_INTEGER li;
    DWORD put = 0;
    li.QuadPart = (LONGLONG)offset;
    if (SetFilePointer(h, li.LowPart, &li.HighPart, FILE_BEGIN) == INVALID_SET_FILE_POINTER
        && GetLastError() != NO_ERROR)
    {
        return false;
    }
    return WriteFile(h, buf, len, &put, NULL) && put == len;
}

// An entry describes a partition when its type GUID is anything but zero. The
// array is mostly empty on a normal table -- 128 slots, a handful used -- so
// every walk over it has to skip the rest.
static bool gptEntryInUse(const unsigned char *e)
{
    for (int b = 0; b < 16; ++b)
    {
        if (e[GPT_ENT_TYPEGUID + b] != 0)
        {
            return true;
        }
    }
    return false;
}

// Validate a GPT header's entry-array geometry and report the space it takes.
// The signature is the caller's business: some of them tell "no GPT here" apart
// from "a GPT that makes no sense", and the two mean different things.
static bool gptEntryGeometry(const unsigned char *hdr, unsigned long long sectorsize,
                             unsigned long long *entrysectors)
{
    unsigned long long numentries = rd32(hdr, GPT_OFF_NUMENTRIES);
    unsigned long long entrysize  = rd32(hdr, GPT_OFF_ENTRYSIZE);
    if (numentries == 0 || numentries > 65536 || entrysize < 128 || entrysize > 4096)
    {
        return false;
    }
    *entrysectors = (numentries * entrysize + sectorsize - 1) / sectorsize;
    return true;
}

GptFixResult relocateBackupGPT(HANDLE hRawDisk, unsigned long long sectorsize,
                               unsigned long long devicesectors, QString *detail)
{
    if (sectorsize < 512 || devicesectors < 96)
    {
        return GPT_FIX_NO_GPT;
    }

    QByteArray primary(sectorsize, 0);
    unsigned char *hdr = (unsigned char *)primary.data();
    if (!rawSeekRead(hRawDisk, sectorsize, hdr, (DWORD)sectorsize))
    {
        return GPT_FIX_FAILED;
    }
    if (memcmp(hdr + GPT_OFF_SIGNATURE, "EFI PART", 8) != 0)
    {
        return GPT_FIX_NO_GPT;
    }

    DWORD headersize = rd32(hdr, GPT_OFF_HEADERSIZE);
    if (headersize < 92 || headersize > sectorsize)
    {
        if (detail) *detail = QObject::tr("the primary GPT header size is out of range");
        return GPT_FIX_BAD_GPT;
    }

    // Verify the header we are about to rewrite is itself intact.
    {
        QByteArray probe = primary.left(headersize);
        wr32((unsigned char *)probe.data(), GPT_OFF_HEADERCRC, 0);
        if (gptCrc32((const unsigned char *)probe.constData(), headersize) != rd32(hdr, GPT_OFF_HEADERCRC))
        {
            if (detail) *detail = QObject::tr("the primary GPT header checksum is invalid");
            return GPT_FIX_BAD_GPT;
        }
    }

    unsigned long long entrylba   = rd64(hdr, GPT_OFF_ENTRYLBA);
    unsigned long long numentries = rd32(hdr, GPT_OFF_NUMENTRIES);
    unsigned long long entrysize  = rd32(hdr, GPT_OFF_ENTRYSIZE);
    unsigned long long lastlba    = devicesectors - 1;

    if (numentries == 0 || numentries > 65536 || entrysize < 128 || entrysize > 4096
        || entrylba < 2 || entrylba >= devicesectors)
    {
        if (detail) *detail = QObject::tr("the GPT partition entry array is not where the header says");
        return GPT_FIX_BAD_GPT;
    }

    if (rd64(hdr, GPT_OFF_ALTLBA) == lastlba)
    {
        return GPT_FIX_NOT_NEEDED;
    }

    // Entry array, rounded up to a whole number of sectors.
    unsigned long long entrybytes = numentries * entrysize;
    unsigned long long entrysectors = (entrybytes + sectorsize - 1) / sectorsize;
    if (entrysectors + 2 >= devicesectors)
    {
        // The arithmetic below subtracts this from the last LBA. A table
        // claiming more entries than the device can hold would wrap round and
        // put the backup GPT at an enormous sector number.
        if (detail) *detail = QObject::tr("the GPT entry array does not fit on the device");
        return GPT_FIX_BAD_GPT;
    }
    QByteArray entries(entrysectors * sectorsize, 0);
    if (!rawSeekRead(hRawDisk, entrylba * sectorsize, entries.data(), (DWORD)(entrysectors * sectorsize)))
    {
        return GPT_FIX_FAILED;
    }

    // Nothing here modifies the entry array, so the checksum in the header must
    // already describe it. Recomputing and storing it instead would hand a
    // damaged table a fresh valid checksum -- and on a table Windows has
    // already rewritten, where PartitionEntryLBA points at empty space, it
    // would overwrite the one field still recording what the real entries hash
    // to. That field is exactly what repairPrimaryGpt() uses to find them
    // again, so destroying it would make the damage unrepairable.
    if (gptCrc32((const unsigned char *)entries.constData(), (size_t)entrybytes)
        != rd32(hdr, GPT_OFF_ENTRIESCRC))
    {
        if (detail) *detail = QObject::tr("the GPT partition entry array checksum is invalid");
        return GPT_FIX_BAD_GPT;
    }

    unsigned long long backuphdr     = lastlba;
    unsigned long long backupentries = backuphdr - entrysectors;
    unsigned long long firstusable   = rd64(hdr, GPT_OFF_FIRSTUSABLE);
    unsigned long long lastusable    = backupentries - 1;
    // Where the image left its backup GPT. Kept before the header is rewritten
    // so the stale copy can be cleared once the new one is in place.
    unsigned long long oldbackuphdr  = rd64(hdr, GPT_OFF_ALTLBA);

    if (backupentries <= firstusable || lastusable <= firstusable)
    {
        return GPT_FIX_FAILED;
    }

    // No partition may extend past the new last usable LBA. Growing the usable
    // area cannot cause that, but a malformed table could.
    for (unsigned long long i = 0; i < numentries; ++i)
    {
        const unsigned char *e = (const unsigned char *)entries.constData() + i * entrysize;
        if (!gptEntryInUse(e))
        {
            continue;
        }
        if (rd64(e, GPT_ENT_LASTLBA) > lastusable)
        {
            if (detail) *detail = QObject::tr("a partition extends past the end of the device");
            return GPT_FIX_FAILED;
        }
    }

    // Rebuild the primary header in place. Only the fields that describe where
    // the device ends change: PartitionEntryLBA and FirstUsableLBA are left
    // exactly as the image wrote them. Forcing the entry array to LBA 2 would
    // move it out from under a FirstUsableLBA that still reserves room ahead of
    // it -- the very mismatch this repair exists to remove -- and would write
    // over whatever the image put between LBA 2 and the array.
    wr64(hdr, GPT_OFF_MYLBA, 1);
    wr64(hdr, GPT_OFF_ALTLBA, backuphdr);
    wr64(hdr, GPT_OFF_LASTUSABLE, lastusable);
    wr32(hdr, GPT_OFF_HEADERCRC, 0);
    wr32(hdr, GPT_OFF_HEADERCRC, gptCrc32(hdr, headersize));

    // The backup header is the primary with MyLBA/AlternateLBA swapped and its
    // own copy of the entry array.
    QByteArray backup = primary;
    unsigned char *bhdr = (unsigned char *)backup.data();
    wr64(bhdr, GPT_OFF_MYLBA, backuphdr);
    wr64(bhdr, GPT_OFF_ALTLBA, 1);
    wr64(bhdr, GPT_OFF_ENTRYLBA, backupentries);
    wr32(bhdr, GPT_OFF_HEADERCRC, 0);
    wr32(bhdr, GPT_OFF_HEADERCRC, gptCrc32(bhdr, headersize));

    // Backup copies first: if power is lost midway the primary still describes
    // a consistent, if stale, table.
    if (!rawSeekWrite(hRawDisk, backupentries * sectorsize, entries.constData(),
                      (DWORD)(entrysectors * sectorsize))
        || !rawSeekWrite(hRawDisk, backuphdr * sectorsize, bhdr, (DWORD)sectorsize))
    {
        return GPT_FIX_FAILED;
    }
    // The primary entry array is not rewritten: it was read from entrylba, it
    // has not changed, and it is already where the header says it is.
    if (!rawSeekWrite(hRawDisk, sectorsize, hdr, (DWORD)sectorsize))
    {
        return GPT_FIX_FAILED;
    }

    // The protective MBR must span the whole device too, or Windows still sees
    // a mismatch. Only a genuine 0xEE protective entry is touched.
    QByteArray mbr(sectorsize, 0);
    if (rawSeekRead(hRawDisk, 0, mbr.data(), (DWORD)sectorsize))
    {
        unsigned char *m = (unsigned char *)mbr.data();
        if (m[450] == 0xEE)
        {
            unsigned long long span = (lastlba > 0xFFFFFFFFull) ? 0xFFFFFFFFull : lastlba;
            if (rd32(m, 454) == 1 && rd32(m, 458) != (DWORD)span)
            {
                wr32(m, 458, (DWORD)span);
                if (!rawSeekWrite(hRawDisk, 0, m, (DWORD)sectorsize))
                {
                    return GPT_FIX_FAILED;
                }
            }
        }
    }

    // The image's own backup GPT is still sitting where the image ended, in the
    // middle of the device. Nothing reads it -- both Windows and Linux follow
    // the pointers in the primary header, which now lead to the copy at the end
    // -- but a stray "EFI PART" signature mid-device is exactly the kind of
    // thing a later scan, clone or recovery tool picks up and acts on. Clear it
    // now that the relocated pair is on the disk.
    //
    // Only a sector that really is the stale backup header is touched, and only
    // once every location involved has been checked. Failing to clear it does
    // not fail the repair: the table on the device is already correct.
    bool stalecleared = false;
    if (oldbackuphdr >= 2 && oldbackuphdr < backupentries)
    {
        QByteArray stale(sectorsize, 0);
        unsigned char *shdr = (unsigned char *)stale.data();
        if (rawSeekRead(hRawDisk, oldbackuphdr * sectorsize, shdr, (DWORD)sectorsize)
            && memcmp(shdr + GPT_OFF_SIGNATURE, "EFI PART", 8) == 0
            && rd64(shdr, GPT_OFF_MYLBA) == oldbackuphdr)
        {
            // Its entry array, as that header itself describes it, rather than
            // an assumption about where it ought to be.
            unsigned long long staleentrylba = rd64(shdr, GPT_OFF_ENTRYLBA);
            unsigned long long stalefirst = staleentrylba;
            unsigned long long stalelast  = oldbackuphdr;
            if (staleentrylba < 2 || staleentrylba > oldbackuphdr
                || oldbackuphdr - staleentrylba != entrysectors)
            {
                // Not the layout this code understands; clear the header sector
                // alone, which is what carries the signature.
                stalefirst = oldbackuphdr;
            }

            // Never touch anything a partition claims, nor the area the new
            // table occupies.
            // At or past FirstUsableLBA keeps it clear of LBA 0/1 and the
            // primary entry array; below the new entry array keeps it clear of
            // the table just written.
            bool safe = (stalefirst >= firstusable) && (stalelast < backupentries);
            for (unsigned long long i = 0; safe && i < numentries; ++i)
            {
                const unsigned char *e =
                    (const unsigned char *)entries.constData() + i * entrysize;
                if (!gptEntryInUse(e))
                {
                    continue;
                }
                unsigned long long pstart = rd64(e, GPT_ENT_FIRSTLBA);
                unsigned long long pend   = rd64(e, GPT_ENT_LASTLBA);
                if (pstart <= stalelast && stalefirst <= pend)
                {
                    safe = false;
                }
            }

            if (safe)
            {
                unsigned long long count = stalelast - stalefirst + 1;
                QByteArray zeros(count * sectorsize, 0);
                stalecleared = rawSeekWrite(hRawDisk, stalefirst * sectorsize,
                                            zeros.constData(),
                                            (DWORD)(count * sectorsize));
            }
        }
    }

    FlushFileBuffers(hRawDisk);
    if (detail)
    {
        *detail = stalecleared
            ? QObject::tr("backup GPT moved to LBA %1; last usable LBA is now %2; "
                          "the stale copy at LBA %3 was cleared")
                  .arg(backuphdr).arg(lastusable).arg(oldbackuphdr)
            : QObject::tr("backup GPT moved to LBA %1; last usable LBA is now %2")
                  .arg(backuphdr).arg(lastusable);
    }
    return GPT_FIX_OK;
}

// One primary MBR entry worth keeping track of while packing: its slot in
// the table (so the right entry gets its start field patched) and where it
// currently is.
struct MbrSlot
{
    int idx;
    unsigned long long first, count;
};

// Reads and validates sector 0 as an MBR (boot signature present). Returns
// false, leaving *sector0 untouched, if the device is too small or the
// signature is missing.
static bool readValidMbr(HANDLE hRawDisk, unsigned long long sectorsize,
                         unsigned long long devicesectors, QByteArray *sector0)
{
    if (sectorsize < 512 || devicesectors < 3)
    {
        return false;
    }
    QByteArray sector(sectorsize, 0);
    unsigned char *mbr = (unsigned char *)sector.data();
    if (!rawSeekRead(hRawDisk, 0ull, mbr, (DWORD)sectorsize)
        || mbr[510] != 0x55 || mbr[511] != 0xAA)
    {
        return false;
    }
    *sector0 = sector;
    return true;
}

// Every in-use primary entry in mbr's table, by slot, in the order it
// currently starts on the device. Entries of type 0 (empty) and 0xEE
// (protective GPT -- this MBR is not really the partition table for a disk
// that has one) are both skipped; only the four primary entries are looked
// at, extended/logical partitions are not. Returns false, via *detail, if an
// entry describes a range that cannot be right.
static bool walkMbrEntries(const unsigned char *mbr, unsigned long long devicesectors,
                           QList<MbrSlot> *order, QString *detail)
{
    for (int i = 0; i < 4; ++i)
    {
        const unsigned char *e = mbr + 446 + i * 16;
        if (e[4] == 0 || e[4] == 0xEE)
        {
            continue;
        }
        unsigned long long start = rd32(e, 8);
        unsigned long long count = rd32(e, 12);
        if (count == 0)
        {
            continue;
        }
        if (start < 1ull || start >= devicesectors || count > devicesectors - start)
        {
            if (detail) *detail = QObject::tr("a partition entry describes an impossible range");
            return false;
        }
        MbrSlot s;
        s.idx = i;
        s.first = start;
        s.count = count;
        order->append(s);
    }
    return true;
}

bool listMbrPartitions(HANDLE hRawDisk, unsigned long long sectorsize,
                       unsigned long long devicesectors,
                       QList<PartitionInfo> *partitions, QString *detail)
{
    QByteArray sector0;
    if (!readValidMbr(hRawDisk, sectorsize, devicesectors, &sector0))
    {
        return false;
    }
    const unsigned char *mbr = (const unsigned char *)sector0.constData();

    QList<MbrSlot> order;
    if (!walkMbrEntries(mbr, devicesectors, &order, detail))
    {
        return false;
    }
    // Table order is whatever slot the partition happens to occupy, which
    // has nothing to do with where it sits on the disk -- a partition
    // deleted and recreated later can land in an earlier slot than one
    // physically ahead of it. List by starting sector instead, the same
    // order Windows' own partition numbering (and diskpart) uses, so
    // "Partition 2" here means the same partition "Partition 2" means
    // there.
    std::sort(order.begin(), order.end(), [](const MbrSlot &a, const MbrSlot &b)
    {
        return a.first < b.first;
    });

    partitions->clear();
    for (const MbrSlot &s : order)
    {
        PartitionInfo info;
        info.slot = s.idx;
        info.firstSector = s.first;
        info.sectors = s.count;
        partitions->append(info);
    }
    return true;
}

bool planMbrShrink(HANDLE hRawDisk, unsigned long long sectorsize,
                   unsigned long long devicesectors, unsigned long long alignsectors,
                   PartitionShrinkPlan *plan, QString *detail,
                   const QList<int> *excludeSlots)
{
    if (alignsectors == 0)
    {
        return false;
    }

    QByteArray sector0;
    if (!readValidMbr(hRawDisk, sectorsize, devicesectors, &sector0))
    {
        return false;
    }
    unsigned char *mbr = (unsigned char *)sector0.data();

    // Every in-use primary entry, by its slot in the table, in the order it
    // currently starts -- the same packing order planGptShrink() uses, and
    // for the same reason: nothing crosses over anything else. Only the four
    // primary entries are looked at; extended/logical partitions are not.
    QList<MbrSlot> order;
    if (!walkMbrEntries(mbr, devicesectors, &order, detail))
    {
        return false;
    }
    if (excludeSlots)
    {
        for (int i = order.size() - 1; i >= 0; --i)
        {
            if (excludeSlots->contains(order[i].idx))
            {
                memset(mbr + 446 + order[i].idx * 16, 0, 16);
                order.removeAt(i);
            }
        }
    }
    if (order.isEmpty())
    {
        if (detail) *detail = QObject::tr("the MBR holds no partitions to shrink to");
        return false;
    }
    std::sort(order.begin(), order.end(), [](const MbrSlot &a, const MbrSlot &b)
    {
        return a.first < b.first;
    });

    QList<ShrinkCopyRange> ranges;
    unsigned long long cursor = 1ull;   // sector 0 is the boot sector itself
    for (const MbrSlot &s : order)
    {
        unsigned long long newfirst = ((cursor + alignsectors - 1) / alignsectors) * alignsectors;
        unsigned long long newlast  = newfirst + s.count - 1;
        if (newfirst > 0xFFFFFFFFull)
        {
            // The classic MBR start field is 32 bits; a device needing more
            // than that to describe a repacked start is not one this table
            // format can express, shrunk or not.
            if (detail) *detail = QObject::tr("the repacked layout no longer fits a 32-bit MBR entry");
            return false;
        }
        ranges.append(ShrinkCopyRange{s.first, newfirst, s.count});
        wr32(mbr + 446 + s.idx * 16, 8, (DWORD)newfirst);
        cursor = newlast + 1;
    }

    if (cursor >= devicesectors)
    {
        if (detail) *detail = QObject::tr("the device is already this tight; nothing to shrink");
        return false;
    }

    plan->headerregion  = sector0;
    plan->headersectors = 1ull;
    plan->ranges        = ranges;
    plan->backupregion.clear();
    plan->backupsectors = 0ull;
    plan->totalsectors  = cursor;
    return true;
}

// Reads and validates the primary GPT header and its entry array. On
// success every out parameter describes the table as found on the device;
// on failure they are untouched and, when given, *detail explains why.
static bool readValidGpt(HANDLE hRawDisk, unsigned long long sectorsize,
                         unsigned long long devicesectors, QByteArray *primaryOut,
                         QByteArray *entriesOut, unsigned long long *entrylbaOut,
                         unsigned long long *entrysectorsOut, unsigned long long *numentriesOut,
                         unsigned long long *entrysizeOut, unsigned long long *firstusableOut,
                         QString *detail)
{
    if (sectorsize < 512 || devicesectors < 96)
    {
        return false;
    }

    QByteArray primary(sectorsize, 0);
    unsigned char *hdr = (unsigned char *)primary.data();
    if (!rawSeekRead(hRawDisk, sectorsize, hdr, (DWORD)sectorsize)
        || memcmp(hdr + GPT_OFF_SIGNATURE, "EFI PART", 8) != 0)
    {
        return false;
    }

    DWORD headersize = rd32(hdr, GPT_OFF_HEADERSIZE);
    if (headersize < 92 || headersize > sectorsize)
    {
        if (detail) *detail = QObject::tr("the primary GPT header size is out of range");
        return false;
    }
    {
        QByteArray probe = primary.left(headersize);
        wr32((unsigned char *)probe.data(), GPT_OFF_HEADERCRC, 0);
        if (gptCrc32((const unsigned char *)probe.constData(), headersize) != rd32(hdr, GPT_OFF_HEADERCRC))
        {
            if (detail) *detail = QObject::tr("the primary GPT header checksum is invalid");
            return false;
        }
    }

    unsigned long long entrysectors;
    if (!gptEntryGeometry(hdr, sectorsize, &entrysectors))
    {
        if (detail) *detail = QObject::tr("the GPT entry array geometry is not usable");
        return false;
    }
    unsigned long long entrylba    = rd64(hdr, GPT_OFF_ENTRYLBA);
    unsigned long long numentries  = rd32(hdr, GPT_OFF_NUMENTRIES);
    unsigned long long entrysize   = rd32(hdr, GPT_OFF_ENTRYSIZE);
    unsigned long long firstusable = rd64(hdr, GPT_OFF_FIRSTUSABLE);
    if (entrylba < 2 || entrylba >= devicesectors || entrysectors > devicesectors - entrylba)
    {
        if (detail) *detail = QObject::tr("the GPT partition entry array is not where the header says");
        return false;
    }
    if (firstusable < entrylba + entrysectors || firstusable >= devicesectors
        || firstusable > devicesectors / 2)
    {
        // The second bound is a sanity check, not a spec requirement: the
        // reserved area ahead of FirstUsableLBA is read whole, in one piece,
        // below, and a table claiming most of the device as "reserved" is not
        // one this repack should guess about.
        if (detail) *detail = QObject::tr("FirstUsableLBA is not usable for repacking");
        return false;
    }

    unsigned long long entrybytes = numentries * entrysize;
    QByteArray entries(entrysectors * sectorsize, 0);
    if (!rawSeekRead(hRawDisk, entrylba * sectorsize, entries.data(), (DWORD)(entrysectors * sectorsize)))
    {
        return false;
    }
    if (gptCrc32((const unsigned char *)entries.constData(), (size_t)entrybytes)
        != rd32(hdr, GPT_OFF_ENTRIESCRC))
    {
        if (detail) *detail = QObject::tr("the GPT partition entry array checksum is invalid");
        return false;
    }

    *primaryOut = primary;
    *entriesOut = entries;
    *entrylbaOut = entrylba;
    *entrysectorsOut = entrysectors;
    *numentriesOut = numentries;
    *entrysizeOut = entrysize;
    *firstusableOut = firstusable;
    return true;
}

// The partition name from a GPT entry (UTF-16LE, 36 code units, offset 56),
// truncated at the first NUL.
static QString gptEntryName(const unsigned char *e)
{
    QString name = QString::fromUtf16(reinterpret_cast<const char16_t *>(e + 56), 36);
    int nul = name.indexOf(QChar(0));
    if (nul >= 0)
    {
        name.truncate(nul);
    }
    return name;
}

bool listGptPartitions(HANDLE hRawDisk, unsigned long long sectorsize,
                       unsigned long long devicesectors,
                       QList<PartitionInfo> *partitions, QString *detail)
{
    QByteArray primary, entries;
    unsigned long long entrylba, entrysectors, numentries, entrysize, firstusable;
    if (!readValidGpt(hRawDisk, sectorsize, devicesectors, &primary, &entries,
                      &entrylba, &entrysectors, &numentries, &entrysize, &firstusable, detail))
    {
        return false;
    }

    QList<QPair<unsigned long long, PartitionInfo>> found;
    for (unsigned long long i = 0; i < numentries; ++i)
    {
        const unsigned char *e = (const unsigned char *)entries.constData() + i * entrysize;
        if (!gptEntryInUse(e))
        {
            continue;
        }
        unsigned long long first = rd64(e, GPT_ENT_FIRSTLBA);
        unsigned long long last  = rd64(e, GPT_ENT_LASTLBA);
        if (last < first)
        {
            continue;
        }
        PartitionInfo info;
        info.slot = (int)i;
        info.firstSector = first;
        info.sectors = last - first + 1;
        info.name = gptEntryName(e);
        found.append(qMakePair(first, info));
    }
    // Entry-array order is whatever slot the partition happens to occupy,
    // which has nothing to do with where it sits on the disk -- a partition
    // deleted and recreated later can land in an earlier slot than one
    // physically ahead of it. List by starting sector instead, the same
    // order Windows' own partition numbering (and diskpart) uses, so
    // "Partition 2" here means the same partition "Partition 2" means
    // there.
    std::sort(found.begin(), found.end(), [](const QPair<unsigned long long, PartitionInfo> &a,
                                              const QPair<unsigned long long, PartitionInfo> &b)
    {
        return a.first < b.first;
    });

    partitions->clear();
    for (const auto &pair : found)
    {
        partitions->append(pair.second);
    }
    return true;
}

bool planGptShrink(HANDLE hRawDisk, unsigned long long sectorsize,
                   unsigned long long devicesectors, unsigned long long alignsectors,
                   PartitionShrinkPlan *plan, QString *detail,
                   const QList<int> *excludeSlots)
{
    if (alignsectors == 0)
    {
        return false;
    }

    QByteArray primary, entries;
    unsigned long long entrylba, entrysectors, numentries, entrysize, firstusable;
    if (!readValidGpt(hRawDisk, sectorsize, devicesectors, &primary, &entries,
                      &entrylba, &entrysectors, &numentries, &entrysize, &firstusable, detail))
    {
        return false;
    }
    unsigned char *hdr = (unsigned char *)primary.data();
    DWORD headersize = rd32(hdr, GPT_OFF_HEADERSIZE);
    unsigned long long entrybytes = numentries * entrysize;

    // Every in-use partition, by its slot in the entry array, in the order it
    // currently starts -- packing follows that order, so nothing changes
    // relative to anything else, only the gaps between them disappear.
    QList<int> order;
    for (unsigned long long i = 0; i < numentries; ++i)
    {
        unsigned char *e = (unsigned char *)entries.data() + i * entrysize;
        if (!gptEntryInUse(e))
        {
            continue;
        }
        if (excludeSlots && excludeSlots->contains((int)i))
        {
            memset(e, 0, (size_t)entrysize);
            continue;
        }
        order.append((int)i);
    }
    if (order.isEmpty())
    {
        if (detail) *detail = QObject::tr("the GPT holds no partitions to shrink to");
        return false;
    }
    std::sort(order.begin(), order.end(), [&](int a, int b)
    {
        const unsigned char *ea = (const unsigned char *)entries.constData() + (size_t)a * entrysize;
        const unsigned char *eb = (const unsigned char *)entries.constData() + (size_t)b * entrysize;
        return rd64(ea, GPT_ENT_FIRSTLBA) < rd64(eb, GPT_ENT_FIRSTLBA);
    });

    QList<ShrinkCopyRange> ranges;
    unsigned long long cursor = firstusable;
    for (int idx : order)
    {
        unsigned char *e = (unsigned char *)entries.data() + (size_t)idx * entrysize;
        unsigned long long origfirst = rd64(e, GPT_ENT_FIRSTLBA);
        unsigned long long origlast  = rd64(e, GPT_ENT_LASTLBA);
        if (origlast < origfirst || origlast >= devicesectors || origfirst < firstusable)
        {
            if (detail) *detail = QObject::tr("a partition entry describes an impossible range");
            return false;
        }
        unsigned long long length  = origlast - origfirst + 1;
        unsigned long long newfirst = ((cursor + alignsectors - 1) / alignsectors) * alignsectors;
        unsigned long long newlast  = newfirst + length - 1;
        ranges.append(ShrinkCopyRange{origfirst, newfirst, length});
        wr64(e, GPT_ENT_FIRSTLBA, newfirst);
        wr64(e, GPT_ENT_LASTLBA, newlast);
        cursor = newlast + 1;
    }

    if (cursor + entrysectors + 1 >= devicesectors)
    {
        if (detail) *detail = QObject::tr("the device is already this tight; nothing to shrink");
        return false;
    }

    // Everything from here on describes the repacked layout, not the device:
    // a fresh backup entry array and header right after the data, and the
    // primary header updated to point at them -- exactly what
    // relocateBackupGPT() does for a real device, just computed up front so
    // it can be written once, in order, instead of read back and patched
    // afterward. That is what lets a compressed output stream use this too:
    // there is no going back to fix up bytes already handed to the encoder.
    unsigned long long lastlba      = cursor + entrysectors;
    unsigned long long backuphdr    = lastlba;
    unsigned long long backupentries = cursor;
    unsigned long long lastusable   = cursor - 1;

    wr64(hdr, GPT_OFF_ALTLBA, backuphdr);
    wr64(hdr, GPT_OFF_LASTUSABLE, lastusable);
    // The entries moved, so the checksum describing them has to be redone;
    // that and the fields above change the header bytes, so its own checksum
    // follows.
    wr32(hdr, GPT_OFF_ENTRIESCRC, gptCrc32((const unsigned char *)entries.constData(), (size_t)entrybytes));
    wr32(hdr, GPT_OFF_HEADERCRC, 0);
    wr32(hdr, GPT_OFF_HEADERCRC, gptCrc32(hdr, headersize));

    QByteArray region((size_t)(firstusable * sectorsize), 0);
    if (!rawSeekRead(hRawDisk, 0, region.data(), (DWORD)region.size()))
    {
        return false;
    }
    memcpy(region.data() + sectorsize, hdr, headersize);
    memcpy(region.data() + entrylba * sectorsize, entries.constData(), (size_t)entries.size());

    // The protective MBR must span the repacked image too, or Windows sees a
    // mismatch and "fixes" it -- the same adjustment relocateBackupGPT() makes
    // on a real device. Only a genuine 0xEE protective entry is touched.
    unsigned char *mbr = (unsigned char *)region.data();
    if (mbr[450] == 0xEE)
    {
        unsigned long long span = (lastlba > 0xFFFFFFFFull) ? 0xFFFFFFFFull : lastlba;
        if (rd32(mbr, 454) == 1)
        {
            wr32(mbr, 458, (DWORD)span);
        }
    }

    // The backup header is the primary with MyLBA/AlternateLBA swapped and its
    // own copy of the entry array, exactly as relocateBackupGPT() builds it.
    QByteArray backup(sectorsize, 0);
    unsigned char *bhdr = (unsigned char *)backup.data();
    memcpy(bhdr, hdr, headersize);
    wr64(bhdr, GPT_OFF_MYLBA, backuphdr);
    wr64(bhdr, GPT_OFF_ALTLBA, 1);
    wr64(bhdr, GPT_OFF_ENTRYLBA, backupentries);
    wr32(bhdr, GPT_OFF_HEADERCRC, 0);
    wr32(bhdr, GPT_OFF_HEADERCRC, gptCrc32(bhdr, headersize));

    QByteArray backupregion;
    backupregion.append(entries);
    backupregion.append(backup);

    plan->headerregion  = region;
    plan->headersectors = firstusable;
    plan->ranges        = ranges;
    plan->backupregion  = backupregion;
    plan->backupsectors = entrysectors + 1;
    plan->totalsectors  = lastlba + 1;
    return true;
}

bool deviceHasMbrTable(HANDLE hRawDisk, unsigned long long sectorsize)
{
    if (sectorsize < 512)
    {
        return false;
    }

    QByteArray sector(sectorsize, 0);
    unsigned char *mbr = (unsigned char *)sector.data();
    if (!rawSeekRead(hRawDisk, 0ull, mbr, (DWORD)sectorsize))
    {
        return false;
    }
    if (mbr[510] != 0x55 || mbr[511] != 0xAA)
    {
        return false;
    }
    // Four 16-byte entries at 446; byte 4 of each is the partition type. 0xEE
    // is the protective entry that fronts a GPT, which is not an MBR table in
    // the sense meant here: a device whose GPT header is missing or damaged
    // would otherwise be reported as an MBR image.
    for (int i = 0; i < 4; ++i)
    {
        unsigned char type = mbr[446 + i * 16 + 4];
        if (type != 0x00 && type != 0xEE)
        {
            return true;
        }
    }
    return false;
}

GptRewriteRisk gptRewriteRisk(HANDLE hRawDisk, unsigned long long sectorsize)
{
    if (sectorsize < 512)
    {
        return GPT_RISK_UNKNOWN;
    }

    QByteArray primary(sectorsize, 0);
    unsigned char *hdr = (unsigned char *)primary.data();
    if (!rawSeekRead(hRawDisk, sectorsize, hdr, (DWORD)sectorsize))
    {
        return GPT_RISK_UNKNOWN;
    }
    if (memcmp(hdr + GPT_OFF_SIGNATURE, "EFI PART", 8) != 0)
    {
        return GPT_RISK_NO_GPT;
    }

    unsigned long long entrylba    = rd64(hdr, GPT_OFF_ENTRYLBA);
    unsigned long long firstusable = rd64(hdr, GPT_OFF_FIRSTUSABLE);
    unsigned long long entrysectors = 0ull;
    if (!gptEntryGeometry(hdr, sectorsize, &entrysectors))
    {
        return GPT_RISK_UNKNOWN;
    }
    if (firstusable < entrysectors)
    {
        return GPT_RISK_UNKNOWN;
    }

    // The value Windows would write, against the one that is there.
    return (firstusable - entrysectors == entrylba) ? GPT_RISK_SAFE : GPT_RISK_AFFECTED;
}

// Read the primary header and check it against itself. Returns the header in
// *header when it is well enough formed to judge.
static GptPrimaryState readPrimaryForCheck(HANDLE hRawDisk, unsigned long long sectorsize,
                                           QByteArray *header)
{
    header->fill(0, sectorsize);
    unsigned char *hdr = (unsigned char *)header->data();
    if (!rawSeekRead(hRawDisk, sectorsize, hdr, (DWORD)sectorsize))
    {
        return GPT_PRIMARY_UNKNOWN;
    }
    if (memcmp(hdr + GPT_OFF_SIGNATURE, "EFI PART", 8) != 0)
    {
        return GPT_PRIMARY_NO_GPT;
    }
    DWORD headersize = rd32(hdr, GPT_OFF_HEADERSIZE);
    if (headersize < 92 || headersize > sectorsize)
    {
        return GPT_PRIMARY_UNKNOWN;
    }
    QByteArray probe = header->left(headersize);
    wr32((unsigned char *)probe.data(), GPT_OFF_HEADERCRC, 0);
    if (gptCrc32((const unsigned char *)probe.constData(), headersize)
        != rd32(hdr, GPT_OFF_HEADERCRC))
    {
        // A header that fails its own checksum is damaged some other way. The
        // rewrite this is looking for leaves one that passes.
        return GPT_PRIMARY_UNKNOWN;
    }
    return GPT_PRIMARY_OK;
}

GptPrimaryState gptPrimaryState(HANDLE hRawDisk, unsigned long long sectorsize,
                                unsigned long long devicesectors)
{
    if (sectorsize < 512 || devicesectors < 96)
    {
        return GPT_PRIMARY_UNKNOWN;
    }

    QByteArray primary;
    GptPrimaryState st = readPrimaryForCheck(hRawDisk, sectorsize, &primary);
    if (st != GPT_PRIMARY_OK)
    {
        return st;
    }
    const unsigned char *hdr = (const unsigned char *)primary.constData();

    unsigned long long entrysectors = 0ull;
    if (!gptEntryGeometry(hdr, sectorsize, &entrysectors))
    {
        return GPT_PRIMARY_UNKNOWN;
    }
    unsigned long long entrylba = rd64(hdr, GPT_OFF_ENTRYLBA);
    if (entrylba < 2 || entrylba + entrysectors > devicesectors)
    {
        // Pointing off the device is the same fault, just further out.
        return GPT_PRIMARY_BROKEN;
    }

    QByteArray entries(entrysectors * sectorsize, 0);
    if (!rawSeekRead(hRawDisk, entrylba * sectorsize, entries.data(),
                     (DWORD)(entrysectors * sectorsize)))
    {
        return GPT_PRIMARY_UNKNOWN;
    }
    size_t crclen = (size_t)(rd32(hdr, GPT_OFF_NUMENTRIES) * rd32(hdr, GPT_OFF_ENTRYSIZE));
    DWORD crc = gptCrc32((const unsigned char *)entries.constData(), crclen);
    return (crc == rd32(hdr, GPT_OFF_ENTRIESCRC)) ? GPT_PRIMARY_OK : GPT_PRIMARY_BROKEN;
}

bool repairPrimaryGpt(HANDLE hRawDisk, unsigned long long sectorsize,
                      unsigned long long devicesectors, QString *detail)
{
    if (sectorsize < 512 || devicesectors < 96)
    {
        if (detail) *detail = QObject::tr("the device geometry is not usable");
        return false;
    }

    QByteArray primary;
    if (readPrimaryForCheck(hRawDisk, sectorsize, &primary) != GPT_PRIMARY_OK)
    {
        if (detail) *detail = QObject::tr("the primary GPT header is not readable");
        return false;
    }
    unsigned char *hdr = (unsigned char *)primary.data();

    unsigned long long entrysectors = 0ull;
    if (!gptEntryGeometry(hdr, sectorsize, &entrysectors))
    {
        if (detail) *detail = QObject::tr("the GPT entry array geometry is not usable");
        return false;
    }

    // The rewrite moves the pointer but leaves PartitionEntryArrayCRC32 alone,
    // so the header still says what the real entries hash to. That checksum is
    // what identifies them, and the entries have not moved: LBA 2 is where a
    // primary array lives. Confirm by checksum rather than assume.
    DWORD wantcrc = rd32(hdr, GPT_OFF_ENTRIESCRC);
    size_t crclen = (size_t)(rd32(hdr, GPT_OFF_NUMENTRIES) * rd32(hdr, GPT_OFF_ENTRYSIZE));
    if (2ull + entrysectors > devicesectors)
    {
        if (detail) *detail = QObject::tr("the device is too small to hold an entry array");
        return false;
    }

    QByteArray entries(entrysectors * sectorsize, 0);
    if (!rawSeekRead(hRawDisk, 2ull * sectorsize, entries.data(),
                     (DWORD)(entrysectors * sectorsize)))
    {
        if (detail) *detail = QObject::tr("the partition entries could not be read");
        return false;
    }
    if (gptCrc32((const unsigned char *)entries.constData(), crclen) != wantcrc)
    {
        if (detail)
        {
            *detail = QObject::tr("the partition entries are not at LBA 2, so this is not "
                                  "the damage this can repair");
        }
        return false;
    }

    wr64(hdr, GPT_OFF_ENTRYLBA, 2ull);
    wr32(hdr, GPT_OFF_HEADERCRC, 0);
    wr32(hdr, GPT_OFF_HEADERCRC, gptCrc32(hdr, rd32(hdr, GPT_OFF_HEADERSIZE)));
    if (!rawSeekWrite(hRawDisk, sectorsize, hdr, (DWORD)sectorsize))
    {
        if (detail) *detail = QObject::tr("the repaired header could not be written");
        return false;
    }
    FlushFileBuffers(hRawDisk);
    if (detail)
    {
        *detail = QObject::tr("PartitionEntryLBA pointed back at LBA 2 and the header "
                              "checksum rebuilt");
    }
    return true;
}

bool gptImageBackupRange(const unsigned char *lba1, unsigned long long sectorsize,
                         unsigned long long *first, unsigned long long *last)
{
    if (lba1 == NULL || sectorsize < 512)
    {
        return false;
    }
    if (memcmp(lba1 + GPT_OFF_SIGNATURE, "EFI PART", 8) != 0)
    {
        return false;
    }

    unsigned long long backuphdr = rd64(lba1, GPT_OFF_ALTLBA);
    unsigned long long entrysectors = 0ull;
    if (backuphdr < 2 || !gptEntryGeometry(lba1, sectorsize, &entrysectors))
    {
        return false;
    }

    // Same shape as the clearing code in relocateBackupGPT: the entry array
    // sits directly below the header, and where it does not, only the header
    // sector carries the signature and only that one gets cleared.
    unsigned long long lo = backuphdr;
    if (entrysectors < backuphdr - 1)
    {
        lo = backuphdr - entrysectors;
    }

    if (first) *first = lo;
    if (last)  *last  = backuphdr;
    return true;
}

bool gptOwnedSectors(HANDLE hRawDisk, unsigned long long sectorsize,
                     unsigned long long devicesectors,
                     unsigned long long *frontend, unsigned long long *tailstart)
{
    if (sectorsize < 512 || devicesectors < 96)
    {
        return false;
    }

    QByteArray primary(sectorsize, 0);
    unsigned char *hdr = (unsigned char *)primary.data();
    if (!rawSeekRead(hRawDisk, sectorsize, hdr, (DWORD)sectorsize))
    {
        return false;
    }
    if (memcmp(hdr + GPT_OFF_SIGNATURE, "EFI PART", 8) != 0)
    {
        return false;
    }

    unsigned long long entrysectors = 0ull;
    if (!gptEntryGeometry(hdr, sectorsize, &entrysectors))
    {
        return false;
    }
    if (entrysectors + 2 >= devicesectors)
    {
        return false;
    }

    // Front: protective MBR, primary header, primary entry array.
    if (frontend)  *frontend  = 2 + entrysectors;
    // Tail: relocated backup entry array plus its header at the last LBA.
    if (tailstart) *tailstart = devicesectors - 1 - entrysectors;
    return true;
}

// A GPT takes 33 sectors: one header and 32 of partition entries. At the front
// the protective MBR sits ahead of them, so 34 covers it exactly; at the tail
// there is no MBR and 34 is one sector more than needed. Both ends use the
// same figure because the spare sector costs nothing and one number is easier
// to be sure of than two.
#define GPT_RESERVED_SECTORS 34

bool wipePartitionTables(HANDLE hRawDisk, unsigned long long sectorsize,
                         unsigned long long devicesectors)
{
    if (sectorsize < 512 || devicesectors < (GPT_RESERVED_SECTORS * 2))
    {
        return false;
    }

    QByteArray zeros(GPT_RESERVED_SECTORS * sectorsize, 0);

    // Front: protective MBR and primary GPT. The image overwrites this region
    // immediately afterwards; clearing it first means a partial write cannot
    // leave a hybrid of the old and new tables.
    if (!rawSeekWrite(hRawDisk, 0, zeros.constData(), (DWORD)zeros.size()))
    {
        return false;
    }

    // Tail: wherever a backup GPT from any previous image would sit.
    unsigned long long tail = (devicesectors - GPT_RESERVED_SECTORS) * sectorsize;
    if (!rawSeekWrite(hRawDisk, tail, zeros.constData(), (DWORD)zeros.size()))
    {
        return false;
    }

    return FlushFileBuffers(hRawDisk);
}
