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

#include "imagesource.h"

#include <QObject>
#include <QFileInfo>
#include <cstring>
#include <zlib.h>
#include <lzma.h>

// Compressed bytes handed to the decoder at a time. Large enough that the read
// syscalls are not what limits throughput, small enough to stay out of the way.
static const unsigned long INPUT_CHUNK = 1024ul * 1024ul;

// Bytes as whole sectors, rounding up. Written out six times before, which is
// six chances to get the rounding the wrong way round.
static inline unsigned long long sectorsFor(unsigned long long bytes,
                                            unsigned long long sectorsize)
{
    return (bytes / sectorsize) + ((bytes % sectorsize) ? 1ull : 0ull);
}


// The most DEFLATE can expand: a 258-byte match encoded in the shortest
// possible way. Used to decide whether a gzip stream could have passed 4 GiB.
static const unsigned long long MAX_DEFLATE_RATIO = 1032ull;

ImageSource::ImageSource()
    : myHandle(INVALID_HANDLE_VALUE), myFormat(FORMAT_RAW), mySectorSize(0ull),
      mySectors(0ull), myCompressedSize(0ull), myPos(0ull), mySizeKnown(false),
      myEof(false), myFinishing(false), myDecoder(NULL), myNextIn(NULL), myAvailIn(0ull)
{
}

ImageSource::~ImageSource()
{
    close();
}

bool ImageSource::nameLooksCompressed(const QString &path)
{
    QString name = path.toLower();
    return name.endsWith(".gz") || name.endsWith(".xz");
}

QString ImageSource::formatName(Format f)
{
    switch (f)
    {
        case FORMAT_GZIP: return QString("gzip");
        case FORMAT_XZ:   return QString("xz");
        default:          return QString("raw");
    }
}

void ImageSource::close()
{
    if (myDecoder != NULL)
    {
        if (myFormat == FORMAT_GZIP)
        {
            inflateEnd((z_stream *)myDecoder);
            delete (z_stream *)myDecoder;
        }
        else if (myFormat == FORMAT_XZ)
        {
            lzma_end((lzma_stream *)myDecoder);
            delete (lzma_stream *)myDecoder;
        }
        myDecoder = NULL;
    }
    if (myHandle != INVALID_HANDLE_VALUE)
    {
        CloseHandle(myHandle);
        myHandle = INVALID_HANDLE_VALUE;
    }
    myInput.clear();
    myNextIn = NULL;
    myAvailIn = 0ull;
    myFormat = FORMAT_RAW;
    mySectorSize = 0ull;
    mySectors = 0ull;
    myCompressedSize = 0ull;
    myPos = 0ull;
    mySizeKnown = false;
    myEof = false;
    myFinishing = false;
}

bool ImageSource::open(const QString &path, unsigned long long sectorsize)
{
    close();
    myError.clear();
    if (!sectorsize)
    {
        myError = QObject::tr("The device reports a sector size of zero.");
        return false;
    }
    mySectorSize = sectorsize;

    myHandle = CreateFileW((LPCWSTR)path.utf16(), GENERIC_READ, FILE_SHARE_READ,
                           NULL, OPEN_EXISTING, FILE_FLAG_SEQUENTIAL_SCAN, NULL);
    if (myHandle == INVALID_HANDLE_VALUE)
    {
        myError = QObject::tr("The image file could not be opened (error %1).")
                      .arg(GetLastError());
        return false;
    }

    LARGE_INTEGER filesize;
    if (!GetFileSizeEx(myHandle, &filesize))
    {
        myError = QObject::tr("The size of the image file could not be read (error %1).")
                      .arg(GetLastError());
        close();
        return false;
    }
    myCompressedSize = (unsigned long long)filesize.QuadPart;

    // Decide by content, not by name: a renamed image should still work, and a
    // raw image called .img.gz should not be fed to a decompressor.
    unsigned char magic[6];
    DWORD magicread = 0;
    if (!ReadFile(myHandle, magic, sizeof(magic), &magicread, NULL))
    {
        myError = QObject::tr("The image file could not be read (error %1).")
                      .arg(GetLastError());
        close();
        return false;
    }
    if (magicread >= 2 && magic[0] == 0x1f && magic[1] == 0x8b)
    {
        myFormat = FORMAT_GZIP;
    }
    else if (magicread >= 6 && memcmp(magic, "\xfd" "7zXZ\x00", 6) == 0)
    {
        myFormat = FORMAT_XZ;
    }
    else
    {
        myFormat = FORMAT_RAW;
    }

    if (myFormat == FORMAT_RAW)
    {
        // Raw images keep the old behaviour, random access included.
        mySectors = sectorsFor(myCompressedSize, mySectorSize);
        mySizeKnown = true;
        // No rewind: the raw path seeks to the sector it wants before every
        // read, so where the magic-byte read left the pointer never matters.
        return true;
    }

    // False is recoverable, a set myError is not; see imagesource.h.
    bool gotsize = (myFormat == FORMAT_GZIP) ? readGzipSize(myCompressedSize)
                                             : readXzSize(myCompressedSize);
    if (!myError.isEmpty())
    {
        close();
        return false;
    }
    mySizeKnown = gotsize;

    LARGE_INTEGER zero;
    zero.QuadPart = 0;
    if (!SetFilePointerEx(myHandle, zero, NULL, FILE_BEGIN))
    {
        myError = QObject::tr("The image file could not be rewound (error %1).")
                      .arg(GetLastError());
        close();
        return false;
    }
    if (!initDecoder())
    {
        close();
        return false;
    }
    myInput.resize(INPUT_CHUNK);
    return true;
}

// Every caller computes an offset it has already checked to be inside the file,
// so a failure here is a real I/O problem rather than a fact about the format,
// and is recorded as one.
bool ImageSource::readAt(unsigned long long offset, void *buf, DWORD len)
{
    LARGE_INTEGER pos;
    pos.QuadPart = (LONGLONG)offset;
    if (!SetFilePointerEx(myHandle, pos, NULL, FILE_BEGIN))
    {
        myError = QObject::tr("The image file could not be read (error %1).")
                      .arg(GetLastError());
        return false;
    }
    DWORD got = 0;
    if (!ReadFile(myHandle, buf, len, &got, NULL) || got != len)
    {
        myError = QObject::tr("The image file could not be read (error %1).")
                      .arg(GetLastError());
        return false;
    }
    return true;
}

// gzip stores the uncompressed size in the last four bytes of the file, but
// only modulo 4 GiB, and only for the last member of a multi-member file. That
// makes it a lower bound, not a size: a 6 GiB image records 2 GiB, and a value
// smaller than the compressed file records nothing usable at all.
//
// The stored value is exact only when the image cannot have reached 4 GiB in
// the first place. DEFLATE cannot expand by more than 1032:1, so once even that
// ratio keeps the file under 4 GiB there is nothing to wrap around -- which
// covers a few megabytes of compressed data and no real disk image. Everything
// larger is kept as an estimate for the progress bar, with the size reported as
// unknown so the write runs until the stream ends instead of stopping at a
// wrapped value and calling a third of an image a complete one.
//
// Returns true only when the size is exact. mySectors is set whenever the value
// is worth anything as an estimate.
bool ImageSource::readGzipSize(unsigned long long filesize)
{
    if (filesize < 18ull)
    {
        // Too small to hold a header and a trailer.
        return false;
    }
    unsigned char isize[4];
    if (!readAt(filesize - 4ull, isize, 4))
    {
        return false;
    }
    unsigned long long size = (unsigned long long)isize[0] |
                              ((unsigned long long)isize[1] << 8) |
                              ((unsigned long long)isize[2] << 16) |
                              ((unsigned long long)isize[3] << 24);
    if (size < filesize)
    {
        // Below the compressed size the value has certainly wrapped, and there
        // is no telling how many times. Not even an estimate.
        return false;
    }
    mySectors = sectorsFor(size, mySectorSize);
    return filesize * MAX_DEFLATE_RATIO < 0x100000000ull;
}

// xz carries an index of every block, so the uncompressed size is exact. The
// file is walked backwards stream by stream: footer, then index, then on to the
// stream before it, which is what xz --list does. (liblzma's own
// lzma_file_info_decode would do this, but it is not in every version we build
// against.)
bool ImageSource::readXzSize(unsigned long long filesize)
{
    unsigned long long pos = filesize;
    unsigned long long total = 0ull;
    std::vector<unsigned char> buf;

    while (pos > 0ull)
    {
        // Streams may be separated by padding: whole groups of four zero bytes.
        while (pos >= 4ull)
        {
            unsigned char pad[4];
            if (!readAt(pos - 4ull, pad, 4))
            {
                return false;
            }
            if (pad[0] || pad[1] || pad[2] || pad[3])
            {
                break;
            }
            pos -= 4ull;
        }
        if (pos == 0ull)
        {
            break;
        }
        if (pos < LZMA_STREAM_HEADER_SIZE)
        {
            return false;
        }

        unsigned char footer[LZMA_STREAM_HEADER_SIZE];
        if (!readAt(pos - LZMA_STREAM_HEADER_SIZE, footer, sizeof(footer)))
        {
            return false;
        }
        lzma_stream_flags flags;
        if (lzma_stream_footer_decode(&flags, footer) != LZMA_OK)
        {
            return false;
        }
        if (flags.backward_size > pos - LZMA_STREAM_HEADER_SIZE ||
            flags.backward_size > (1ull << 26))   // an index this large is not an image we wrote
        {
            return false;
        }

        buf.resize((size_t)flags.backward_size);
        if (!readAt(pos - LZMA_STREAM_HEADER_SIZE - flags.backward_size,
                    &buf[0], (DWORD)flags.backward_size))
        {
            return false;
        }

        lzma_index *index = NULL;
        uint64_t memlimit = UINT64_MAX;
        size_t inpos = 0;
        if (lzma_index_buffer_decode(&index, &memlimit, NULL, &buf[0], &inpos,
                                     buf.size()) != LZMA_OK)
        {
            if (index != NULL)
            {
                lzma_index_end(index, NULL);
            }
            return false;
        }
        total += lzma_index_uncompressed_size(index);
        unsigned long long streamsize = lzma_index_stream_size(index);
        lzma_index_end(index, NULL);
        if (streamsize > pos)
        {
            return false;
        }
        pos -= streamsize;
    }

    if (total == 0ull)
    {
        return false;
    }
    mySectors = sectorsFor(total, mySectorSize);
    return true;
}

bool ImageSource::initDecoder()
{
    if (myFormat == FORMAT_GZIP)
    {
        z_stream *zs = new z_stream;
        memset(zs, 0, sizeof(*zs));
        // 15 + 16: gzip wrapper, the only one .img.gz uses.
        int ret = inflateInit2(zs, 15 + 16);
        if (ret != Z_OK)
        {
            delete zs;
            myError = QObject::tr("The gzip decompressor could not be started "
                                  "(zlib error %1).").arg(ret);
            return false;
        }
        myDecoder = zs;
        return true;
    }

    lzma_stream *ls = new lzma_stream;
    memset(ls, 0, sizeof(*ls));
    // LZMA_CONCATENATED: xz files may be several streams appended together.
    lzma_ret ret = lzma_stream_decoder(ls, UINT64_MAX, LZMA_CONCATENATED);
    if (ret != LZMA_OK)
    {
        delete ls;
        myError = QObject::tr("The xz decompressor could not be started "
                              "(lzma error %1).").arg((int)ret);
        return false;
    }
    myDecoder = ls;
    return true;
}

// Looks at the two bytes after the member that just ended, refilling the input
// buffer if it holds fewer than that, and says whether they are a gzip header.
// Returns false only on a read error; a file that simply ran out reports that
// no member follows.
// Refill the compressed-input buffer, keeping `kept` bytes already sitting at
// its front. nextMemberFollows() and fill() had a copy of this each, including
// a copy of the error it reports.
bool ImageSource::refillInput(size_t kept, DWORD *got)
{
    *got = 0;
    if (!ReadFile(myHandle, &myInput[kept], (DWORD)(myInput.size() - kept), got, NULL))
    {
        myError = QObject::tr("The image file could not be read (error %1).")
                      .arg(GetLastError());
        return false;
    }
    return true;
}

bool ImageSource::nextMemberFollows(bool *follows)
{
    *follows = false;
    if (myAvailIn < 2ull)
    {
        // Keep the odd leftover byte: it may be the first half of the header.
        if (myAvailIn == 1ull)
        {
            myInput[0] = myNextIn[0];
        }
        size_t kept = (size_t)myAvailIn;
        DWORD got = 0;
        if (!refillInput(kept, &got))
        {
            return false;
        }
        myNextIn = &myInput[0];
        myAvailIn = (unsigned long long)kept + got;
    }
    if (myAvailIn >= 2ull)
    {
        *follows = (myNextIn[0] == 0x1f && myNextIn[1] == 0x8b);
    }
    return true;
}

bool ImageSource::fill(char *buf, unsigned long long len, unsigned long long *produced)
{
    *produced = 0ull;
    while (*produced < len && !myEof)
    {
        if (myAvailIn == 0ull && !myFinishing)
        {
            DWORD got = 0;
            if (!refillInput(0, &got))
            {
                return false;
            }
            if (got == 0)
            {
                // The file has run out. A gzip stream announces its own end, so
                // reaching this without one means the file stops in the middle
                // of the compressed data; xz in concatenated mode has to be
                // told the input ended before it will say so.
                if (myFormat != FORMAT_XZ)
                {
                    myError = QObject::tr("The image file ends in the middle of the "
                                          "compressed data. It is truncated or damaged.");
                    return false;
                }
                myFinishing = true;
            }
            else
            {
                myNextIn = &myInput[0];
                myAvailIn = got;
            }
        }

        if (myFormat == FORMAT_GZIP)
        {
            z_stream *zs = (z_stream *)myDecoder;
            zs->next_in = myNextIn;
            zs->avail_in = (uInt)((myAvailIn > 0xffffffffull) ? 0xffffffffull : myAvailIn);
            zs->next_out = (Bytef *)(buf + *produced);
            zs->avail_out = (uInt)((len - *produced > 0xffffffffull) ? 0xffffffffull
                                                                    : (len - *produced));
            uInt availin = zs->avail_in;
            uInt availout = zs->avail_out;
            int ret = inflate(zs, Z_NO_FLUSH);
            *produced += availout - zs->avail_out;
            myAvailIn -= availin - zs->avail_in;
            myNextIn = (unsigned char *)zs->next_in;
            if (ret == Z_STREAM_END)
            {
                // Members can be concatenated, so another one may follow -- but
                // so may padding. A writer working in fixed-size blocks leaves
                // zero bytes after the last member, and anything that is not a
                // gzip header is not ours to decode either way. Feeding that to
                // the decoder would report a perfectly written image as damaged
                // once the whole of it had already gone to the device, so the
                // image ends here unless a real member header follows.
                bool another = false;
                if (!nextMemberFollows(&another))
                {
                    return false;
                }
                if (!another)
                {
                    myEof = true;
                }
                else if (inflateReset(zs) != Z_OK)
                {
                    myError = QObject::tr("The gzip image could not be decompressed.");
                    return false;
                }
            }
            else if (ret != Z_OK && ret != Z_BUF_ERROR)
            {
                myError = QObject::tr("The gzip image is damaged (zlib error %1).").arg(ret);
                return false;
            }
        }
        else
        {
            lzma_stream *ls = (lzma_stream *)myDecoder;
            ls->next_in = myNextIn;
            ls->avail_in = (size_t)myAvailIn;
            ls->next_out = (uint8_t *)(buf + *produced);
            ls->avail_out = (size_t)(len - *produced);
            size_t availout = ls->avail_out;
            lzma_ret ret = lzma_code(ls, myFinishing ? LZMA_FINISH : LZMA_RUN);
            size_t madeout = availout - ls->avail_out;
            *produced += madeout;
            myAvailIn = ls->avail_in;
            myNextIn = (unsigned char *)ls->next_in;
            if (ret == LZMA_STREAM_END)
            {
                myEof = true;
            }
            else if (ret != LZMA_OK)
            {
                myError = QObject::tr("The xz image is damaged (lzma error %1).")
                              .arg((int)ret);
                return false;
            }
            else if (myFinishing && madeout == 0)
            {
                // Out of input, still not at the end of the stream, and nothing
                // more coming out: the file is truncated.
                myError = QObject::tr("The image file ends in the middle of the "
                                      "compressed data. It is truncated or damaged.");
                return false;
            }
        }
    }
    return true;
}

bool ImageSource::skipTo(unsigned long long startsector)
{
    if (startsector < myPos)
    {
        myError = QObject::tr("A compressed image can only be read forwards.");
        return false;
    }
    std::vector<char> scrap;
    while (myPos < startsector && !myEof)
    {
        unsigned long long chunk = startsector - myPos;
        if (chunk > 1024ull)
        {
            chunk = 1024ull;
        }
        unsigned long long bytes = chunk * mySectorSize;
        if (scrap.size() < bytes)
        {
            scrap.resize((size_t)bytes);
        }
        unsigned long long produced = 0ull;
        if (!fill(&scrap[0], bytes, &produced))
        {
            return false;
        }
        myPos += sectorsFor(produced, mySectorSize);
    }
    return true;
}

char *ImageSource::read(unsigned long long startsector, unsigned long long count,
                        unsigned long long *sectorsread)
{
    myError.clear();
    if (sectorsread != NULL)
    {
        *sectorsread = 0ull;
    }
    if (count == 0ull)
    {
        return NULL;
    }

    if (myFormat == FORMAT_RAW)
    {
        char *data = new char[(size_t)(mySectorSize * count)];
        DWORD bytesread = 0;
        LARGE_INTEGER li;
        li.QuadPart = (LONGLONG)(startsector * mySectorSize);
        if (!SetFilePointerEx(myHandle, li, NULL, FILE_BEGIN) ||
            !ReadFile(myHandle, data, (DWORD)(mySectorSize * count), &bytesread, NULL))
        {
            myError = QObject::tr("The image file could not be read (error %1).")
                          .arg(GetLastError());
            delete[] data;
            return NULL;
        }
        if (bytesread < mySectorSize * count)
        {
            memset(data + bytesread, 0, (size_t)(mySectorSize * count - bytesread));
        }
        if (sectorsread != NULL)
        {
            *sectorsread = sectorsFor(bytesread, mySectorSize);
        }
        return data;
    }

    if (!skipTo(startsector))
    {
        return NULL;
    }

    char *data = new char[(size_t)(mySectorSize * count)];
    unsigned long long produced = 0ull;
    if (!fill(data, mySectorSize * count, &produced))
    {
        delete[] data;
        return NULL;
    }
    if (produced < mySectorSize * count)
    {
        // Pad the tail of the image out to a whole sector; the device is
        // written a sector at a time either way.
        memset(data + produced, 0, (size_t)(mySectorSize * count - produced));
    }
    unsigned long long full = sectorsFor(produced, mySectorSize);
    myPos += full;
    if (sectorsread != NULL)
    {
        *sectorsread = full;
    }
    return data;
}
