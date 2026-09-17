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
 *  Copyright (C) 2026 peacepenguin (fork not affiliated    *
 *  with the upstream ImageWriter project)                            *
 *  https://github.com/peacepenguin/win32diskimager                   *
 **********************************************************************/

// Reads images back through the real ImageSource and checks the bytes that come
// out are the bytes that went in.
//
// The decoder is the part of this program with the most ways to be subtly
// wrong -- multi-member gzip, multi-stream xz, stream padding, a size that is
// not a whole number of sectors, a file that stops in the middle -- and none of
// it is reachable from the GPT harness. A mistake here corrupts an image on its
// way to a card without anything saying so.
//
// The fixtures are built here, with zlib and liblzma directly, so the test
// needs no gzip or xz on the path and no compressed files in the repository.
//
// Known gap: nextMemberFollows() keeps a single leftover byte when the input
// buffer runs out with exactly one byte of the next gzip header in it. Reaching
// that from here would mean sizing a member's *compressed* length to land one
// byte short of the 1 MB input buffer, which these fixtures do not do. Removing
// that line leaves every check below passing.

#include <QCoreApplication>
#include <QByteArray>
#include <QFile>
#include <QString>
#include <windows.h>
#include <cstdio>
#include <cstring>
#include <zlib.h>
#include <lzma.h>
#include "imagesource.h"

static const unsigned long long SS = 512;

static int checks = 0;
static int failures = 0;

static void check(bool ok, const char *what)
{
    printf("  %s %s\n", ok ? "ok  " : "FAIL", what);
    ++checks;
    if (!ok) ++failures;
}

// A pattern that does not compress to nothing and where any displacement shows
// up immediately: each 8 bytes carries its own offset.
static QByteArray pattern(int bytes)
{
    QByteArray b(bytes, 0);
    unsigned char *p = (unsigned char *)b.data();
    for (int i = 0; i + 8 <= bytes; i += 8)
    {
        unsigned int a = (unsigned int)(i / 8);
        unsigned int c = a * 2654435761u;
        memcpy(p + i, &a, 4);
        memcpy(p + i + 4, &c, 4);
    }
    return b;
}

static QByteArray gzipOf(const QByteArray &raw)
{
    z_stream zs;
    memset(&zs, 0, sizeof(zs));
    // 16 + MAX_WBITS asks zlib for a gzip wrapper rather than a zlib one.
    if (deflateInit2(&zs, 6, Z_DEFLATED, 16 + MAX_WBITS, 8, Z_DEFAULT_STRATEGY) != Z_OK)
    {
        return QByteArray();
    }
    QByteArray out(raw.size() + (raw.size() / 8) + 4096, 0);
    zs.next_in = (Bytef *)raw.constData();
    zs.avail_in = (uInt)raw.size();
    zs.next_out = (Bytef *)out.data();
    zs.avail_out = (uInt)out.size();
    const int r = deflate(&zs, Z_FINISH);
    const int used = (int)((char *)zs.next_out - out.constData());
    deflateEnd(&zs);
    if (r != Z_STREAM_END)
    {
        return QByteArray();
    }
    out.truncate(used);
    return out;
}

static QByteArray xzOf(const QByteArray &raw)
{
    size_t cap = (size_t)raw.size() + 65536;
    QByteArray out((int)cap, 0);
    size_t used = 0;
    if (lzma_easy_buffer_encode(1, LZMA_CHECK_CRC64, NULL,
                                (const uint8_t *)raw.constData(), (size_t)raw.size(),
                                (uint8_t *)out.data(), &used, cap) != LZMA_OK)
    {
        return QByteArray();
    }
    out.truncate((int)used);
    return out;
}

static bool writeFile(const QString &name, const QByteArray &bytes)
{
    QFile f(name);
    if (!f.open(QIODevice::WriteOnly))
    {
        return false;
    }
    const bool ok = f.write(bytes) == (qint64)bytes.size();
    f.close();
    return ok;
}

// Read the whole image through ImageSource, a few sectors at a time, the way
// the write path does. Returns false with the reason in *why.
static bool readBack(const QString &name, QByteArray *got, QString *why)
{
    ImageSource src;
    if (!src.open(name, SS))
    {
        *why = src.errorString();
        return false;
    }
    got->clear();
    unsigned long long sector = 0;
    for (;;)
    {
        unsigned long long produced = 0;
        char *data = src.read(sector, 8, &produced);
        if (data == NULL)
        {
            *why = src.errorString();
            src.close();
            return false;
        }
        if (produced == 0)
        {
            delete[] data;
            break;
        }
        got->append(data, (int)(produced * SS));
        delete[] data;
        sector += produced;
    }
    src.close();
    return true;
}

// The image is padded out to a whole sector, so only the original bytes are
// compared; everything past them must be zero rather than stale buffer.
static void caseRoundTrip(const char *name, const QString &file, const QByteArray &raw)
{
    printf("%s\n", name);
    QByteArray got;
    QString why;
    if (!readBack(file, &got, &why))
    {
        printf("  FAIL %s\n", why.toLocal8Bit().constData());
        ++checks; ++failures;
        printf("\n");
        return;
    }
    check(got.size() >= raw.size(), "read back at least the whole image");
    check(got.size() >= raw.size()
          && memcmp(got.constData(), raw.constData(), (size_t)raw.size()) == 0,
          "every byte matches the original");
    bool padzero = true;
    for (int i = raw.size(); i < got.size(); ++i)
    {
        if (got.at(i) != 0) { padzero = false; break; }
    }
    check(padzero, "the padding to a whole sector is zeros");
    printf("\n");
}

static void caseRejected(const char *name, const QString &file)
{
    printf("%s\n", name);
    QByteArray got;
    QString why;
    const bool ok = readBack(file, &got, &why);
    check(!ok, "reported a failure rather than returning short data");
    if (!ok)
    {
        printf("  -> %s\n", why.toLocal8Bit().constData());
        check(!why.isEmpty(), "said why");
    }
    printf("\n");
}

int main(int argc, char **argv)
{
    QCoreApplication app(argc, argv);

    // Not a whole number of sectors, so the last-sector padding is exercised.
    const QByteArray raw = pattern(3 * 1024 * 1024) + QByteArray("TAIL");
    const QByteArray gz = gzipOf(raw);
    const QByteArray xz = xzOf(raw);

    printf("fixtures\n");
    check(!gz.isEmpty(), "gzip fixture built");
    check(!xz.isEmpty(), "xz fixture built");
    check((raw.size() % (int)SS) != 0, "the image is not a whole number of sectors");
    printf("\n");
    if (gz.isEmpty() || xz.isEmpty())
    {
        printf("%d checks, %d failures\n", checks, failures);
        return 1;
    }

    // Split at an offset that is deliberately not sector-aligned, so a member
    // boundary falls in the middle of a sector.
    const int cut = 1234567;
    const QByteArray gz1 = gzipOf(raw.left(cut)), gz2 = gzipOf(raw.mid(cut));
    const QByteArray xz1 = xzOf(raw.left(cut)),   xz2 = xzOf(raw.mid(cut));

    writeFile("imgtest.img", raw);
    writeFile("imgtest.img.gz", gz);
    writeFile("imgtest.img.xz", xz);
    writeFile("imgtest-multi.img.gz", gz1 + gz2);
    writeFile("imgtest-multi.img.xz", xz1 + xz2);
    writeFile("imgtest-padded.img.xz", xz1 + QByteArray(4, '\0') + xz2);
    writeFile("imgtest-trunc.img.gz", gz.left(gz.size() / 2));
    writeFile("imgtest-trunc.img.xz", xz.left(xz.size() / 2));

    caseRoundTrip("raw", "imgtest.img", raw);
    caseRoundTrip("gzip", "imgtest.img.gz", raw);
    caseRoundTrip("xz", "imgtest.img.xz", raw);
    caseRoundTrip("gzip, two members", "imgtest-multi.img.gz", raw);
    caseRoundTrip("xz, two streams", "imgtest-multi.img.xz", raw);
    caseRoundTrip("xz, two streams with padding between", "imgtest-padded.img.xz", raw);

    // Truncation has to be reported. Writing what did come out and calling it
    // done would put half an image on a card and say it succeeded.
    caseRejected("gzip that stops in the middle", "imgtest-trunc.img.gz");
    caseRejected("xz that stops in the middle", "imgtest-trunc.img.xz");

    const char *leftovers[] = {
        "imgtest.img", "imgtest.img.gz", "imgtest.img.xz",
        "imgtest-multi.img.gz", "imgtest-multi.img.xz", "imgtest-padded.img.xz",
        "imgtest-trunc.img.gz", "imgtest-trunc.img.xz",
    };
    for (size_t i = 0; i < sizeof(leftovers) / sizeof(leftovers[0]); ++i)
    {
        DeleteFileA(leftovers[i]);
    }

    printf("%d checks, %d failures\n", checks, failures);
    return failures ? 1 : 0;
}
