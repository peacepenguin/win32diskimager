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

// Renders an SVG into a Windows .ico holding every size the shell asks for.
//
//   mkicon <in.svg> <out.ico>
//
// The executable's icon has to be an .ico -- the resource compiler takes
// nothing else -- but the source of truth stays the SVG, so the icon can be
// changed by editing a text file and running this again.
//
// Sizes up to 64 are written as DIBs, which every version of Windows reads;
// 128 and 256 are written as PNG, which is what the format expects for the
// large ones and keeps the file from running to megabytes.

#include <QGuiApplication>
#include <QBuffer>
#include <QByteArray>
#include <QFile>
#include <QImage>
#include <QIcon>
#include <QPainter>
#include <QSvgRenderer>
#include <QtEndian>
#include <cstdio>

static void put16(QByteArray &b, quint16 v)
{
    char raw[2];
    qToLittleEndian(v, raw);
    b.append(raw, 2);
}

static void put32(QByteArray &b, quint32 v)
{
    char raw[4];
    qToLittleEndian(v, raw);
    b.append(raw, 4);
}

// An icon image as the format wants it: 32-bit BGRA, bottom-up, with a
// BITMAPINFOHEADER whose height counts both the colour and the mask.
static QByteArray asDib(const QImage &src)
{
    const QImage img = src.convertToFormat(QImage::Format_ARGB32);
    const int w = img.width(), h = img.height();

    QByteArray out;
    put32(out, 40);                 // header size
    put32(out, (quint32)w);
    put32(out, (quint32)(h * 2));   // colour rows + mask rows
    put16(out, 1);                  // planes
    put16(out, 32);                 // bits per pixel
    put32(out, 0);                  // BI_RGB
    put32(out, (quint32)(w * h * 4));
    put32(out, 0); put32(out, 0);   // pixels per metre
    put32(out, 0); put32(out, 0);   // palette

    for (int y = h - 1; y >= 0; --y)
    {
        const QRgb *row = reinterpret_cast<const QRgb *>(img.constScanLine(y));
        for (int x = 0; x < w; ++x)
        {
            const QRgb p = row[x];
            out.append((char)qBlue(p));
            out.append((char)qGreen(p));
            out.append((char)qRed(p));
            out.append((char)qAlpha(p));
        }
    }

    // The AND mask is vestigial for 32-bit icons -- the alpha channel decides --
    // but it has to be there, rows padded to four bytes.
    const int maskStride = ((w + 31) / 32) * 4;
    out.append(QByteArray(maskStride * h, '\0'));
    return out;
}

static QByteArray asPng(const QImage &img)
{
    QByteArray out;
    QBuffer buf(&out);
    buf.open(QIODevice::WriteOnly);
    img.save(&buf, "PNG");
    return out;
}

int main(int argc, char **argv)
{
    QGuiApplication app(argc, argv);
    if (argc != 3)
    {
        fprintf(stderr, "usage: mkicon <in.svg> <out.ico>\n");
        return 2;
    }
    const QString in = QString::fromLocal8Bit(argv[1]);
    const QString out = QString::fromLocal8Bit(argv[2]);

    QSvgRenderer renderer(in);
    if (!renderer.isValid())
    {
        fprintf(stderr, "mkicon: %s is not an SVG this can read\n", argv[1]);
        return 1;
    }

    const int sizes[] = {16, 20, 24, 32, 48, 64, 128, 256};
    QList<QByteArray> blobs;
    QList<int> dims;
    for (int px : sizes)
    {
        QImage img(px, px, QImage::Format_ARGB32);
        img.fill(Qt::transparent);
        QPainter painter(&img);
        painter.setRenderHint(QPainter::Antialiasing, true);
        renderer.render(&painter);
        painter.end();

        blobs.append(px >= 128 ? asPng(img) : asDib(img));
        dims.append(px);
    }

    QByteArray ico;
    put16(ico, 0);                      // reserved
    put16(ico, 1);                      // 1 = icon
    put16(ico, (quint16)blobs.size());

    // Entries come first, so every offset counts the whole directory.
    quint32 offset = 6 + 16 * (quint32)blobs.size();
    for (int i = 0; i < blobs.size(); ++i)
    {
        const int px = dims[i];
        ico.append((char)(px >= 256 ? 0 : px));   // 0 means 256
        ico.append((char)(px >= 256 ? 0 : px));
        ico.append((char)0);                      // palette size
        ico.append((char)0);                      // reserved
        put16(ico, 1);                            // planes
        put16(ico, 32);                           // bits per pixel
        put32(ico, (quint32)blobs[i].size());
        put32(ico, offset);
        offset += (quint32)blobs[i].size();
    }
    for (const QByteArray &b : blobs)
    {
        ico.append(b);
    }

    QFile f(out);
    if (!f.open(QIODevice::WriteOnly))
    {
        fprintf(stderr, "mkicon: cannot write %s\n", argv[2]);
        return 1;
    }
    // Checked: the build depends on this file now, so a disk that is full or
    // an I/O error must fail the build rather than leave a truncated .ico
    // behind and report success.
    if (f.write(ico) != (qint64)ico.size() || !f.flush())
    {
        fprintf(stderr, "mkicon: failed writing %s: %s\n", argv[2],
                qPrintable(f.errorString()));
        f.close();
        return 1;
    }
    f.close();

    printf("%s: %d images (", argv[2], (int)blobs.size());
    for (int i = 0; i < dims.size(); ++i)
    {
        printf("%s%d", i ? " " : "", dims[i]);
    }
    printf("), %lld bytes\n", (long long)ico.size());
    return 0;
}
