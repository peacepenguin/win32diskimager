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

#ifndef IMAGESOURCE_H
#define IMAGESOURCE_H

#ifndef WINVER
#define WINVER 0x0601
#endif

#include <QString>
#include <windows.h>
#include <vector>

// A sector-oriented reader over an image file that may be stored raw, gzip
// compressed (.img.gz) or xz compressed (.img.xz). Compressed images are
// decompressed on the fly while they are written to the device, so no
// intermediate copy of the full image is ever put on disk.
//
// Raw images are read exactly as before, with random access. A compressed
// image can only be read forwards: read() may skip ahead (the skipped data is
// decompressed and discarded) but never back.
class ImageSource
{
public:
    enum Format { FORMAT_RAW, FORMAT_GZIP, FORMAT_XZ };

    ImageSource();
    ~ImageSource();

    // True if the name looks like a compressed image, used before anything is
    // opened (the Read button, for one). open() decides by content, not name.
    static bool nameLooksCompressed(const QString &path);
    // "gzip" / "xz", for messages.
    static QString formatName(Format f);

    // Opens path and works out its format and, where possible, the size of the
    // uncompressed image. Returns false and sets errorString() on failure.
    bool open(const QString &path, unsigned long long sectorsize);
    void close();

    Format format() const { return myFormat; }
    bool isCompressed() const { return myFormat != FORMAT_RAW; }
    // True when sizeInSectors() is the exact size of the image. xz carries an
    // index, so it always is; gzip stores only the low 32 bits of the size, so
    // it only is for an image small enough that those bits cannot have wrapped.
    // When this is false the image has to be written until the stream ends.
    bool sizeKnown() const { return mySizeKnown; }
    // Exact when sizeKnown(), otherwise a lower bound useful for a progress
    // estimate, or 0 when even that could not be worked out. Never use it to
    // decide where the image ends unless sizeKnown().
    unsigned long long sizeInSectors() const { return mySectors; }
    // Compressed bytes consumed so far, for progress on an unknown size.
    unsigned long long compressedSize() const { return myCompressedSize; }
    const QString &errorString() const { return myError; }

    // Reads count sectors starting at startsector into a newly allocated
    // buffer the caller deletes with delete[]. Short reads at the end of the
    // image are zero-padded and reported through sectorsread, which counts the
    // sectors that actually contain image data. Returns NULL on error.
    // For a compressed image startsector must not go backwards.
    char *read(unsigned long long startsector, unsigned long long count,
               unsigned long long *sectorsread);

private:
    bool initDecoder();
    // Decompresses up to len bytes, returning the number produced. Short only
    // at the end of the stream.
    bool fill(char *buf, unsigned long long len, unsigned long long *produced);
    // gzip: true when a further member starts where the last one ended, rather
    // than padding or a trailer that is not ours to decode.
    bool nextMemberFollows(bool *follows);
    bool skipTo(unsigned long long startsector);
    bool readAt(unsigned long long offset, void *buf, unsigned long len);
    bool readGzipSize(unsigned long long filesize);
    bool readXzSize(unsigned long long filesize);

    HANDLE myHandle;
    Format myFormat;
    unsigned long long mySectorSize;
    unsigned long long mySectors;
    unsigned long long myCompressedSize;
    unsigned long long myPos;          // next sector the stream will produce
    bool mySizeKnown;
    bool myEof;
    bool myFinishing;                  // xz: input ended, flushing the decoder
    QString myError;

    void *myDecoder;                   // z_stream or lzma_stream, owned
    std::vector<unsigned char> myInput;
    unsigned char *myNextIn;
    unsigned long long myAvailIn;
};

#endif // IMAGESOURCE_H
