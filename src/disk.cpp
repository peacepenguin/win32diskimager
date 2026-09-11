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

#ifndef WINVER
#define WINVER 0x0601
#endif

#include <QtWidgets>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <windows.h>
#include <winioctl.h>
#include "disk.h"
#include "mainwindow.h"

HANDLE getHandleOnFile(LPCWSTR filelocation, DWORD access)
{
    HANDLE hFile;
    hFile = CreateFileW(filelocation, access, (access == GENERIC_READ) ? FILE_SHARE_READ : 0, NULL, (access == GENERIC_READ) ? OPEN_EXISTING:CREATE_ALWAYS, 0, NULL);
    if (hFile == INVALID_HANDLE_VALUE)
    {
        wchar_t *errormessage=NULL;
        ::FormatMessageW(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_ALLOCATE_BUFFER, NULL, GetLastError(), 0,
                         (LPWSTR)&errormessage, 0, NULL);
        QString errText = QString::fromUtf16((const char16_t *)errormessage);
        QMessageBox::critical(MainWindow::getInstanceIfAvailable(), QObject::tr("File Error"), QObject::tr("An error occurred when attempting to get a handle on the file.\n"
                                                              "Error %1: %2").arg(GetLastError()).arg(errText));
        LocalFree(errormessage);
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
        wchar_t *errormessage=NULL;
        FormatMessageW(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_ALLOCATE_BUFFER, NULL, GetLastError(), 0, (LPWSTR)&errormessage, 0, NULL);
        QString errText = QString::fromUtf16((const char16_t *)errormessage);
        QMessageBox::critical(MainWindow::getInstanceIfAvailable(), QObject::tr("Device Error"),
                              QObject::tr("An error occurred when attempting to get a handle on the device.\n"
                                          "Error %1: %2").arg(GetLastError()).arg(errText));
        LocalFree(errormessage);
    }
    return hDevice;
}

bool getLockOnVolume(HANDLE handle)
{
    DWORD bytesreturned;
    BOOL bResult;
    bResult = DeviceIoControl(handle, FSCTL_LOCK_VOLUME, NULL, 0, NULL, 0, &bytesreturned, NULL);
    if (!bResult)
    {
        wchar_t *errormessage=NULL;
        FormatMessageW(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_ALLOCATE_BUFFER, NULL, GetLastError(), 0, (LPWSTR)&errormessage, 0, NULL);
        QString errText = QString::fromUtf16((const char16_t *)errormessage);
        QMessageBox::critical(MainWindow::getInstanceIfAvailable(), QObject::tr("Lock Error"),
                              QObject::tr("An error occurred when attempting to lock the volume.\n"
                                          "Error %1: %2").arg(GetLastError()).arg(errText));
        LocalFree(errormessage);
    }
    return (bResult);
}

bool removeLockOnVolume(HANDLE handle)
{
    DWORD junk;
    BOOL bResult;
    bResult = DeviceIoControl(handle, FSCTL_UNLOCK_VOLUME, NULL, 0, NULL, 0, &junk, NULL);
    if (!bResult)
    {
        wchar_t *errormessage=NULL;
        FormatMessageW(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_ALLOCATE_BUFFER, NULL, GetLastError(), 0, (LPWSTR)&errormessage, 0, NULL);
        QString errText = QString::fromUtf16((const char16_t *)errormessage);
        QMessageBox::critical(MainWindow::getInstanceIfAvailable(), QObject::tr("Unlock Error"),
                              QObject::tr("An error occurred when attempting to unlock the volume.\n"
                                          "Error %1: %2").arg(GetLastError()).arg(errText));
        LocalFree(errormessage);
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
        wchar_t *errormessage=NULL;
        FormatMessageW(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_ALLOCATE_BUFFER, NULL, GetLastError(), 0, (LPWSTR)&errormessage, 0, NULL);
        QString errText = QString::fromUtf16((const char16_t *)errormessage);
        QMessageBox::critical(MainWindow::getInstanceIfAvailable(), QObject::tr("Dismount Error"),
                              QObject::tr("An error occurred when attempting to dismount the volume.\n"
                                          "Error %1: %2").arg(GetLastError()).arg(errText));
        LocalFree(errormessage);
    }
    return (bResult);
}

bool isVolumeUnmounted(HANDLE handle)
{
    DWORD junk;
    BOOL bResult;
    bResult = DeviceIoControl(handle, FSCTL_IS_VOLUME_MOUNTED, NULL, 0, NULL, 0, &junk, NULL);
    return (!bResult);
}

char *readSectorDataFromHandle(HANDLE handle, unsigned long long startsector, unsigned long long numsectors, unsigned long long sectorsize)
{
    unsigned long bytesread;
    char *data = new char[sectorsize * numsectors];
    LARGE_INTEGER li;
    li.QuadPart = startsector * sectorsize;
    SetFilePointer(handle, li.LowPart, &li.HighPart, FILE_BEGIN);
    if (!ReadFile(handle, data, sectorsize * numsectors, &bytesread, NULL))
    {
        wchar_t *errormessage=NULL;
        FormatMessageW(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_ALLOCATE_BUFFER, NULL, GetLastError(), 0, (LPWSTR)&errormessage, 0, NULL);
        QString errText = QString::fromUtf16((const char16_t *)errormessage);
        QMessageBox::critical(MainWindow::getInstanceIfAvailable(), QObject::tr("Read Error"),
                              QObject::tr("An error occurred when attempting to read data from handle.\n"
                                          "Error %1: %2").arg(GetLastError()).arg(errText));
        LocalFree(errormessage);
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
    SetFilePointer(handle, li.LowPart, &li.HighPart, FILE_BEGIN);
    bResult = WriteFile(handle, data, sectorsize * numsectors, &byteswritten, NULL);
    if (!bResult)
    {
        wchar_t *errormessage=NULL;
        FormatMessageW(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_ALLOCATE_BUFFER, NULL, GetLastError(), 0, (LPWSTR)&errormessage, 0, NULL);
        QString errText = QString::fromUtf16((const char16_t *)errormessage);
        QMessageBox::critical(MainWindow::getInstanceIfAvailable(), QObject::tr("Write Error"),
                              QObject::tr("An error occurred when attempting to write data to handle.\n"
                                          "Error %1: %2").arg(GetLastError()).arg(errText));
        LocalFree(errormessage);
    }
    return (bResult);
}

unsigned long long getNumberOfSectors(HANDLE handle, unsigned long long *sectorsize)
{
    DWORD junk;
    DISK_GEOMETRY_EX diskgeometry;
    BOOL bResult;
    bResult = DeviceIoControl(handle, IOCTL_DISK_GET_DRIVE_GEOMETRY_EX, NULL, 0, &diskgeometry, sizeof(diskgeometry), &junk, NULL);
    if (!bResult)
    {
        wchar_t *errormessage=NULL;
        FormatMessageW(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_ALLOCATE_BUFFER, NULL, GetLastError(), 0, (LPWSTR)&errormessage, 0, NULL);
        QString errText = QString::fromUtf16((const char16_t *)errormessage);
        QMessageBox::critical(MainWindow::getInstanceIfAvailable(), QObject::tr("Device Error"),
                              QObject::tr("An error occurred when attempting to get the device's geometry.\n"
                                          "Error %1: %2").arg(GetLastError()).arg(errText));
        LocalFree(errormessage);
        return 0;
    }
    if (sectorsize != NULL)
    {
        *sectorsize = (unsigned long long)diskgeometry.Geometry.BytesPerSector;
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
            wchar_t *errormessage=NULL;
            FormatMessageW(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_ALLOCATE_BUFFER, NULL, GetLastError(), 0, (LPWSTR)&errormessage, 0, NULL);
            QString errText = QString::fromUtf16((const char16_t *)errormessage);
            QMessageBox::critical(MainWindow::getInstanceIfAvailable(), QObject::tr("File Error"),
                                  QObject::tr("An error occurred while getting the file size.\n"
                                              "Error %1: %2").arg(GetLastError()).arg(errText));
            LocalFree(errormessage);
            retVal = 0;
        }
        else
        {
            retVal = ((unsigned long long)filesize.QuadPart / sectorsize ) + (((unsigned long long)filesize.QuadPart % sectorsize )?1:0);
        }
    }
    return(retVal);
}

bool spaceAvailable(char *location, unsigned long long spaceneeded)
{
    ULARGE_INTEGER freespace;
    BOOL bResult;
    bResult = GetDiskFreeSpaceEx(location, NULL, NULL, &freespace);
    if (!bResult)
    {
        wchar_t *errormessage=NULL;
        FormatMessageW(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_ALLOCATE_BUFFER, NULL, GetLastError(), 0, (LPWSTR)&errormessage, 0, NULL);
        QString errText = QString::fromUtf16((const char16_t *)errormessage);
        QMessageBox::critical(MainWindow::getInstanceIfAvailable(), QObject::tr("Free Space Error"),
                              QObject::tr("Failed to get the free space on drive %1.\n"
                                          "Error %2: %3\n"
                                          "Checking of free space will be skipped.").arg(location).arg(GetLastError()).arg(errText));
        return true;
    }
    return (spaceneeded <= freespace.QuadPart);
}




// Physical disk a mounted volume lives on, or -1 if it cannot be determined.
static int diskNumberOfVolume(char letter)
{
    char volumename[] = "\\\\.\\A:";
    volumename[4] = letter;
    // No access rights are requested: this only queries the volume, and asking
    // for read access would need the volume to be readable by us.
    HANDLE h = CreateFile(volumename, 0, FILE_SHARE_READ | FILE_SHARE_WRITE,
                          NULL, OPEN_EXISTING, 0, NULL);
    if (h == INVALID_HANDLE_VALUE)
    {
        return -1;
    }
    VOLUME_DISK_EXTENTS sd;
    DWORD bytesreturned;
    int disk = -1;
    if (DeviceIoControl(h, IOCTL_VOLUME_GET_VOLUME_DISK_EXTENTS, NULL, 0,
                        &sd, sizeof(sd), &bytesreturned, NULL)
        && sd.NumberOfDiskExtents > 0)
    {
        disk = (int)sd.Extents[0].DiskNumber;
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
        // Querying needs no access rights, so this works without the disk being
        // readable and without disturbing whatever else has it open.
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
        // Never offer the disk Windows is running from, whatever the filter.
        if (systemDisk >= 0 && (int)n == systemDisk)
        {
            continue;
        }
        if (!dev.removable && !includeFixed)
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
        char volumename[] = "\\\\.\\A:";
        volumename[4] = 'A' + i;
        HANDLE h = CreateFile(volumename, GENERIC_READ | GENERIC_WRITE,
                              FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, 0, NULL);
        if (h == INVALID_HANDLE_VALUE)
        {
            continue;
        }
        VOLUME_DISK_EXTENTS sd;
        DWORD bytesreturned;
        if (!DeviceIoControl(h, IOCTL_VOLUME_GET_VOLUME_DISK_EXTENTS, NULL, 0,
                             &sd, sizeof(sd), &bytesreturned, NULL)
            || sd.Extents[0].DiskNumber != deviceID)
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
    QByteArray entries(entrysectors * sectorsize, 0);
    if (!rawSeekRead(hRawDisk, entrylba * sectorsize, entries.data(), (DWORD)(entrysectors * sectorsize)))
    {
        return GPT_FIX_FAILED;
    }

    unsigned long long backuphdr     = lastlba;
    unsigned long long backupentries = backuphdr - entrysectors;
    unsigned long long firstusable   = rd64(hdr, GPT_OFF_FIRSTUSABLE);
    unsigned long long lastusable    = backupentries - 1;

    if (backupentries <= firstusable || lastusable <= firstusable)
    {
        return GPT_FIX_FAILED;
    }

    // No partition may extend past the new last usable LBA. Growing the usable
    // area cannot cause that, but a malformed table could.
    for (unsigned long long i = 0; i < numentries; ++i)
    {
        const unsigned char *e = (const unsigned char *)entries.constData() + i * entrysize;
        bool empty = true;
        for (int b = 0; b < 16; ++b)
        {
            if (e[b] != 0) { empty = false; break; }
        }
        if (empty)
        {
            continue;
        }
        if (rd64(e, 40) > lastusable)
        {
            if (detail) *detail = QObject::tr("a partition extends past the end of the device");
            return GPT_FIX_FAILED;
        }
    }

    DWORD entriescrc = gptCrc32((const unsigned char *)entries.constData(), (size_t)entrybytes);

    // Rebuild the primary header in place.
    wr64(hdr, GPT_OFF_MYLBA, 1);
    wr64(hdr, GPT_OFF_ALTLBA, backuphdr);
    wr64(hdr, GPT_OFF_LASTUSABLE, lastusable);
    wr64(hdr, GPT_OFF_ENTRYLBA, 2);
    wr32(hdr, GPT_OFF_ENTRIESCRC, entriescrc);
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
    if (entrylba != 2
        && !rawSeekWrite(hRawDisk, 2 * sectorsize, entries.constData(),
                         (DWORD)(entrysectors * sectorsize)))
    {
        return GPT_FIX_FAILED;
    }
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

    FlushFileBuffers(hRawDisk);
    if (detail)
    {
        *detail = QObject::tr("backup GPT moved to LBA %1; last usable LBA is now %2")
                      .arg(backuphdr).arg(lastusable);
    }
    return GPT_FIX_OK;
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

    unsigned long long numentries = rd32(hdr, GPT_OFF_NUMENTRIES);
    unsigned long long entrysize  = rd32(hdr, GPT_OFF_ENTRYSIZE);
    if (numentries == 0 || numentries > 65536 || entrysize < 128 || entrysize > 4096)
    {
        return false;
    }

    unsigned long long entrysectors =
        (numentries * entrysize + sectorsize - 1) / sectorsize;
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

// GPT reserves 33 sectors at each end: one header plus 32 sectors of partition
// entries. 34 covers that plus the protective MBR, with a sector to spare.
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
