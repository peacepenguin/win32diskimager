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
 *  Copyright (C) 2026 peacepenguin (personal fork, not affiliated    *
 *  with the upstream ImageWriter project)                            *
 *  https://github.com/peacepenguin/win32diskimager                   *
 **********************************************************************/

// Exercises relocateBackupGPT() against a file standing in for a device, so the
// GPT repair can be changed without an SD card, a VM or a UAC prompt. The real
// disk.cpp is linked in: rawSeekRead/rawSeekWrite go through ReadFile/WriteFile,
// which work on a plain file handle exactly as they do on a raw device.
//
// It builds a GPT whose backup sits mid-"device", the way writing a small image
// to a larger card leaves it, relocates it, and checks what came out. The cases
// that matter most are the ones where nothing should be written: this code
// zeroes sectors, and a guard that stops working would quietly destroy data.
//
//   cmake -S tools/gpttest -B build-gpttest -G Ninja
//   cmake --build build-gpttest && ./build-gpttest/gpttest.exe
//
// Exits non-zero if any check fails. See BUILD.md.

#include <QCoreApplication>
#include <QByteArray>
#include <QString>
#include <windows.h>
#include <cstdio>
#include <cstring>
#include "disk.h"
#include "mainwindow.h"

// disk.cpp parents its message boxes on this. Nothing here creates a window, so
// a null instance is all the definition that is needed to link.
MainWindow *MainWindow::instance = NULL;

static const unsigned long long SEC = 512;
static const unsigned long long ENTRIES = 128;
static const unsigned long long ENTRYSIZE = 128;
static const unsigned long long ENTRYSECTORS = (ENTRIES * ENTRYSIZE) / SEC;   // 32

// GPT header field offsets, as in disk.cpp.
enum { H_MYLBA = 24, H_ALTLBA = 32, H_FIRSTUSABLE = 40, H_LASTUSABLE = 48,
       H_ENTRYLBA = 72, H_HEADERCRC = 16 };
// Partition entry field offsets.
enum { P_START = 32, P_END = 40 };

static unsigned long long rd64(const unsigned char *p, int o)
{
    unsigned long long v = 0;
    for (int i = 7; i >= 0; --i) v = (v << 8) | p[o + i];
    return v;
}
static unsigned int rd32(const unsigned char *p, int o)
{
    return (unsigned int)p[o] | ((unsigned int)p[o + 1] << 8)
         | ((unsigned int)p[o + 2] << 16) | ((unsigned int)p[o + 3] << 24);
}
static void wr64(unsigned char *p, int o, unsigned long long v)
{
    for (int i = 0; i < 8; ++i) p[o + i] = (unsigned char)((v >> (8 * i)) & 0xFF);
}
static void wr32(unsigned char *p, int o, unsigned int v)
{
    for (int i = 0; i < 4; ++i) p[o + i] = (unsigned char)((v >> (8 * i)) & 0xFF);
}

static unsigned int crc32of(const unsigned char *d, size_t n)
{
    static unsigned int tbl[256];
    static bool init = false;
    if (!init)
    {
        for (unsigned int i = 0; i < 256; ++i)
        {
            unsigned int c = i;
            for (int k = 0; k < 8; ++k) c = (c & 1) ? (0xEDB88320u ^ (c >> 1)) : (c >> 1);
            tbl[i] = c;
        }
        init = true;
    }
    unsigned int c = 0xFFFFFFFFu;
    for (size_t i = 0; i < n; ++i) c = tbl[(c ^ d[i]) & 0xFF] ^ (c >> 8);
    return c ^ 0xFFFFFFFFu;
}

static int failures = 0;
static int checks = 0;
static void check(bool ok, const char *what)
{
    printf("  %s %s\n", ok ? "ok  " : "FAIL", what);
    ++checks;
    if (!ok) ++failures;
}

static void makeHeader(unsigned char *h, unsigned long long hdrlba,
                       unsigned long long altlba, unsigned long long entrylba,
                       unsigned long long firstusable, unsigned long long lastusable,
                       unsigned int entriescrc)
{
    memset(h, 0, SEC);
    memcpy(h, "EFI PART", 8);
    wr32(h, 8, 0x00010000);      // revision 1.0
    wr32(h, 12, 92);             // header size
    wr64(h, H_MYLBA, hdrlba);
    wr64(h, H_ALTLBA, altlba);
    wr64(h, H_FIRSTUSABLE, firstusable);
    wr64(h, H_LASTUSABLE, lastusable);
    wr64(h, H_ENTRYLBA, entrylba);
    wr32(h, 80, (unsigned int)ENTRIES);
    wr32(h, 84, (unsigned int)ENTRYSIZE);
    wr32(h, 88, entriescrc);
    wr32(h, H_HEADERCRC, 0);
    wr32(h, H_HEADERCRC, crc32of(h, 92));
}

static bool headerCrcValid(const unsigned char *h)
{
    QByteArray probe((const char *)h, 92);
    wr32((unsigned char *)probe.data(), H_HEADERCRC, 0);
    return crc32of((const unsigned char *)probe.constData(), 92) == rd32(h, H_HEADERCRC);
}

// One partitioned "device": a GPT written as if an image of imagesectors had
// just been written to a card of devicesectors, so the backup lands mid-device.
// partend, when non-zero, overrides where the single partition ends -- used to
// park a partition on top of the stale backup.
struct Disk
{
    QByteArray bytes;
    QByteArray entries;
    unsigned long long imglast, imgbackupentries;
};

static Disk buildDisk(unsigned long long firstusable, unsigned long long imagesectors,
                      unsigned long long devicesectors, unsigned long long partend = 0)
{
    Disk dk;
    dk.bytes = QByteArray(devicesectors * SEC, 0);
    unsigned char *d = (unsigned char *)dk.bytes.data();

    dk.imglast = imagesectors - 1;
    dk.imgbackupentries = dk.imglast - ENTRYSECTORS;
    unsigned long long imglastusable = dk.imgbackupentries - 1;

    dk.entries = QByteArray(ENTRIES * ENTRYSIZE, 0);
    unsigned char *e = (unsigned char *)dk.entries.data();
    memset(e, 0xAB, 16);                 // type GUID: any non-zero value
    memset(e + 16, 0xCD, 16);            // unique GUID
    wr64(e, P_START, firstusable);
    wr64(e, P_END, partend ? partend : imglastusable - 1);
    unsigned int ecrc = crc32of((const unsigned char *)dk.entries.constData(),
                                ENTRIES * ENTRYSIZE);

    // Protective MBR spanning the image only, as the image left it.
    d[446 + 4] = 0xEE;
    wr32(d, 446 + 8, 1);
    wr32(d, 446 + 12, (unsigned int)dk.imglast);
    d[510] = 0x55; d[511] = 0xAA;

    makeHeader(d + SEC, 1, dk.imglast, 2, firstusable, imglastusable, ecrc);
    memcpy(d + 2 * SEC, dk.entries.constData(), ENTRIES * ENTRYSIZE);
    makeHeader(d + dk.imglast * SEC, dk.imglast, 1, dk.imgbackupentries,
               firstusable, imglastusable, ecrc);
    memcpy(d + dk.imgbackupentries * SEC, dk.entries.constData(), ENTRIES * ENTRYSIZE);
    return dk;
}

static const char *TESTFILE = "gpttest.img";

// Writes the disk to a file, runs the repair, reads the result back.
static GptFixResult runRepair(const Disk &dk, unsigned long long devicesectors,
                              QByteArray *after, QString *detail)
{
    DeleteFileA(TESTFILE);
    HANDLE h = CreateFileA(TESTFILE, GENERIC_READ | GENERIC_WRITE, 0, NULL,
                           CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    if (h == INVALID_HANDLE_VALUE)
    {
        printf("  FAIL could not create %s\n", TESTFILE);
        ++failures;
        return GPT_FIX_FAILED;
    }
    DWORD put = 0;
    WriteFile(h, dk.bytes.constData(), (DWORD)dk.bytes.size(), &put, NULL);

    GptFixResult r = relocateBackupGPT(h, SEC, devicesectors, detail);

    *after = QByteArray(devicesectors * SEC, 0);
    SetFilePointer(h, 0, NULL, FILE_BEGIN);
    DWORD got = 0;
    ReadFile(h, after->data(), (DWORD)after->size(), &got, NULL);
    CloseHandle(h);
    return r;
}

static bool rangeIsZero(const unsigned char *a, unsigned long long first,
                        unsigned long long last)
{
    for (unsigned long long s = first; s <= last; ++s)
    {
        for (unsigned long long i = 0; i < SEC; ++i)
        {
            if (a[s * SEC + i] != 0) return false;
        }
    }
    return true;
}

// The ordinary path: a backup stranded mid-device is moved to the end and the
// stale copy is cleared.
static void caseRelocate(const char *name, unsigned long long firstusable,
                         unsigned long long imagesectors, unsigned long long devicesectors)
{
    printf("%s (FirstUsableLBA=%llu, image=%llu sectors, device=%llu sectors)\n",
           name, firstusable, imagesectors, devicesectors);

    Disk dk = buildDisk(firstusable, imagesectors, devicesectors);
    QByteArray after;
    QString detail;
    GptFixResult r = runRepair(dk, devicesectors, &after, &detail);
    printf("  -> %s\n", detail.toLocal8Bit().constData());

    const unsigned char *a = (const unsigned char *)after.constData();
    unsigned long long lastlba = devicesectors - 1;
    unsigned long long newentries = lastlba - ENTRYSECTORS;

    check(r == GPT_FIX_OK, "returned GPT_FIX_OK");

    const unsigned char *p = a + SEC;
    check(memcmp(p, "EFI PART", 8) == 0, "primary is still a GPT header");
    check(headerCrcValid(p), "primary header CRC is valid");
    check(rd64(p, H_ALTLBA) == lastlba, "primary AlternateLBA is the last LBA");
    check(rd64(p, H_LASTUSABLE) == newentries - 1, "primary LastUsableLBA extended");
    // The whole point of the repair: these two must survive untouched, or the
    // mismatch that triggers Windows' rewrite is reintroduced.
    check(rd64(p, H_FIRSTUSABLE) == firstusable, "primary FirstUsableLBA untouched");
    check(rd64(p, H_ENTRYLBA) == 2, "primary PartitionEntryLBA untouched");

    const unsigned char *b = a + lastlba * SEC;
    check(memcmp(b, "EFI PART", 8) == 0, "backup header sits at the last LBA");
    check(headerCrcValid(b), "backup header CRC is valid");
    check(rd64(b, H_MYLBA) == lastlba, "backup MyLBA is the last LBA");
    check(rd64(b, H_ALTLBA) == 1, "backup AlternateLBA is 1");
    check(rd64(b, H_ENTRYLBA) == newentries, "backup entry array precedes it");
    check(memcmp(a + newentries * SEC, dk.entries.constData(), ENTRIES * ENTRYSIZE) == 0,
          "backup entry array copied");

    check(rangeIsZero(a, dk.imgbackupentries, dk.imglast),
          "stale backup GPT and its entry array are zeroed");
    check(memcmp(a + dk.imglast * SEC, "EFI PART", 8) != 0,
          "no stray EFI PART signature mid-device");

    check(rd32(a, 446 + 12) == (unsigned int)lastlba, "protective MBR spans the device");
    check(memcmp(a + 2 * SEC, dk.entries.constData(), ENTRIES * ENTRYSIZE) == 0,
          "primary entry array untouched");
    printf("\n");
}

// A partition sitting on top of where the stale backup is. The relocate must
// still happen, but nothing inside the partition may be zeroed.
static void caseStaleUnderPartition()
{
    const unsigned long long firstusable = 34, image = 4096, device = 16384;
    printf("stale copy covered by a partition (nothing may be zeroed)\n");

    Disk dk = buildDisk(firstusable, image, device, /*partend=*/image - 1);
    // Recognisable data across the sectors the cleanup would otherwise clear.
    unsigned char *d = (unsigned char *)dk.bytes.data();
    for (unsigned long long s = dk.imgbackupentries; s <= dk.imglast; ++s)
    {
        if (s == dk.imglast || s == dk.imgbackupentries) continue;   // table sectors
        memset(d + s * SEC, 0x5A, SEC);
    }

    QByteArray after;
    QString detail;
    GptFixResult r = runRepair(dk, device, &after, &detail);
    printf("  -> %s\n", detail.toLocal8Bit().constData());
    const unsigned char *a = (const unsigned char *)after.constData();

    check(r == GPT_FIX_OK, "returned GPT_FIX_OK");
    check(rd64(a + SEC, H_ALTLBA) == device - 1, "backup still relocated to the end");
    bool datakept = true;
    for (unsigned long long s = dk.imgbackupentries + 1; s < dk.imglast; ++s)
    {
        for (unsigned long long i = 0; i < SEC; ++i)
        {
            if (a[s * SEC + i] != 0x5A) { datakept = false; break; }
        }
    }
    check(datakept, "data inside the partition was NOT zeroed");
    printf("\n");
}

// No GPT at all, a backup already at the end, and a corrupt header: each must
// leave the device exactly as it was.
static void caseUntouched(const char *name, GptFixResult expect,
                          void (*damage)(unsigned char *, unsigned long long))
{
    const unsigned long long firstusable = 34, image = 4096, device = 16384;
    printf("%s\n", name);

    Disk dk = buildDisk(firstusable, image, device);
    damage((unsigned char *)dk.bytes.data(), device);
    QByteArray before = dk.bytes;

    QByteArray after;
    QString detail;
    GptFixResult r = runRepair(dk, device, &after, &detail);

    check(r == expect, "returned the expected result");
    check(after == before, "the device was left byte-identical");
    printf("\n");
}

static void damageNone(unsigned char *, unsigned long long) {}
static void damageSignature(unsigned char *d, unsigned long long)
{
    memset(d + SEC, 0, 8);                       // wipe "EFI PART"
}
static void damageCrc(unsigned char *d, unsigned long long)
{
    wr32(d + SEC, H_HEADERCRC, 0xDEADBEEF);      // header no longer checksums
}
static void damageAlreadyAtEnd(unsigned char *d, unsigned long long device)
{
    // Claim the backup is already at the last LBA: nothing to relocate.
    unsigned char *h = d + SEC;
    wr64(h, H_ALTLBA, device - 1);
    wr32(h, H_HEADERCRC, 0);
    wr32(h, H_HEADERCRC, crc32of(h, 92));
}

int main(int argc, char **argv)
{
    QCoreApplication app(argc, argv);

    // Ordinary layout: FirstUsableLBA 34, which the Windows rewrite happens to
    // land on correctly.
    caseRelocate("ordinary layout", 34, 2048, 8192);
    // The layout the repair exists for: space reserved ahead of the first
    // partition, as ARM board images do it.
    caseRelocate("reserved-space layout", 2048, 16384, 65536);
    // Old and new tables nearly touching, to exercise the overlap guards.
    caseRelocate("image nearly fills device", 34, 8000, 8192);

    caseStaleUnderPartition();

    caseUntouched("no GPT on the device", GPT_FIX_NO_GPT, damageSignature);
    caseUntouched("backup already at the last LBA", GPT_FIX_NOT_NEEDED, damageAlreadyAtEnd);
    caseUntouched("primary header CRC invalid", GPT_FIX_BAD_GPT, damageCrc);
    (void)damageNone;

    DeleteFileA(TESTFILE);
    printf("%d checks, %d failures\n", checks, failures);
    return failures ? 1 : 0;
}
