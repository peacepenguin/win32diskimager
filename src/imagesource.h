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
 *  https://github.com/peacepenguin/windiskimager                   *
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

    bool isCompressed() const { return myFormat != FORMAT_RAW; }
    // True when sizeInSectors() is the exact size of the image. xz carries an
    // index, so it usually is -- but readXzSize reports the size unknown rather
    // than guess when that index cannot be read, so "always" would be too
    // strong. gzip stores only the low 32 bits of the size, so it is exact only
    // for an image small enough that those bits cannot have wrapped. When this
    // is false the image has to be written until the stream ends.
    bool sizeKnown() const { return mySizeKnown; }
    // Exact when sizeKnown(), otherwise a lower bound useful for a progress
    // estimate, or 0 when even that could not be worked out. Never use it to
    // decide where the image ends unless sizeKnown().
    unsigned long long sizeInSectors() const { return mySectors; }
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
    // The size probes share a convention. A false return means only that the
    // size could not be worked out, which the write survives by running until
    // the stream ends; a set errorString() means the file itself could not be
    // read, which it does not. Without the distinction a disk error during the
    // probe would pass for "size unknown" and the write would run to the end of
    // the device instead of stopping.
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
    bool refillInput(size_t kept, DWORD *got);
    std::vector<unsigned char> myInput;
    unsigned char *myNextIn;
    unsigned long long myAvailIn;
};

// The counterpart to ImageSource for "Read to .img.gz" / "Read to .img.xz":
// compresses bytes and writes them out as they arrive, rather than decoding
// bytes already on disk. There is no seeking either way: write() must be
// called with the image's bytes in order, from the start, with nothing
// skipped -- a gap has to be written as explicit zeros, since a compressed
// stream has no notion of "leave this part unwritten" the way a sparse raw
// file does.
class ImageSink
{
public:
    enum Format { FORMAT_GZIP, FORMAT_XZ };

    ImageSink();
    ~ImageSink();

    bool open(const QString &path, Format format);

    // Compresses len bytes from data and writes the result out. Nothing is
    // guaranteed to be on disk, or even decodable, until finish() succeeds.
    bool write(const char *data, unsigned long long len);

    // Flushes the compressor and closes the file. Required for the image to
    // be valid; call abort() instead to give up.
    bool finish();

    // Closes the file without flushing the compressor, leaving whatever has
    // already reached disk -- a stream that stops mid-block, same as
    // canceling a raw read leaves a short raw file.
    void abort();

    const QString &errorString() const { return myError; }

private:
    bool drain(bool finishing);

    HANDLE myHandle;
    Format myFormat;
    QString myError;
    void *myEncoder;                   // z_stream or lzma_stream, owned
    std::vector<unsigned char> myOutput;
};

#endif // IMAGESOURCE_H
