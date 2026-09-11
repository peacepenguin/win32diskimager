#!/usr/bin/env python3
"""Decode the GPT (protective MBR, primary + backup header, entries) from a raw
image file or a Windows physical device, e.g.

    python tools/gptdump.py //./PhysicalDrive2
    python tools/gptdump.py armbian-rock5b.img

Run elevated when pointing at a device. Use it before and after a write to see
exactly what Windows changed.
"""
import sys, struct, uuid, binascii

SS = 512

def crc(b):
    return binascii.crc32(b) & 0xFFFFFFFF

def read_at(f, lba, n=1):
    f.seek(lba * SS)
    return f.read(n * SS)

def dev_size(path, f):
    try:
        f.seek(0, 2)
        return f.tell()
    except OSError:
        pass
    # Windows raw device: seek-to-end fails, ask the OS.
    import ctypes
    from ctypes import wintypes
    h = ctypes.windll.kernel32.CreateFileW(
        path, 0, 3, None, 3, 0, None)
    if h == -1:
        return None
    buf = ctypes.create_string_buffer(32)
    ret = wintypes.DWORD()
    # IOCTL_DISK_GET_LENGTH_INFO
    ok = ctypes.windll.kernel32.DeviceIoControl(
        h, 0x0007405C, None, 0, buf, 32, ctypes.byref(ret), None)
    ctypes.windll.kernel32.CloseHandle(h)
    return struct.unpack("<q", buf.raw[:8])[0] if ok else None

def show_header(name, raw, disk_lba_max):
    sig, rev, hsize, hcrc, _, mylba, altlba, first, last, gid, plba, pnum, psize, pcrc = \
        struct.unpack_from("<8sIIII QQQQ 16s QIII", raw, 0)
    if sig != b"EFI PART":
        print(f"{name}: no GPT signature (got {sig!r})")
        return None
    chk = bytearray(raw[:hsize]); chk[16:20] = b"\0\0\0\0"
    print(f"{name}:")
    print(f"  MyLBA            {mylba}")
    # Only the primary's AlternateLBA should equal the device's last LBA; in a
    # backup header it points back at LBA 1 and means nothing is wrong.
    if mylba == 1 and disk_lba_max is not None:
        note = "   (ok)" if altlba == disk_lba_max else f"   <-- device last LBA is {disk_lba_max}"
    else:
        note = ""
    print(f"  AlternateLBA     {altlba}{note}")
    print(f"  FirstUsableLBA   {first}")
    print(f"  LastUsableLBA    {last}")
    print(f"  DiskGUID         {uuid.UUID(bytes_le=gid)}")
    span = (pnum * psize + SS - 1) // SS
    print(f"  PartEntryLBA     {plba}   count={pnum} size={psize}"
          f"   (array spans {span} sectors, {plba}..{plba + span - 1})")
    print(f"  HeaderCRC        {hcrc:#010x} {'OK' if crc(bytes(chk)) == hcrc else 'BAD'}")
    print(f"  EntriesCRC       {pcrc:#010x}")
    return altlba, (plba, pnum, psize, pcrc)

def find_entry_array(f, pcrc, pnum, psize, last):
    """Where the entry array the header describes actually lives.

    A header whose EntriesCRC does not match the sectors PartEntryLBA points at
    is the signature of this bug: the table looks self-consistent to a checker
    that only validates the header, while the array it names is somewhere else.
    Knowing where it really is says whether the array moved or the pointer did.
    """
    want = pnum * psize
    windows = [range(1, 96)]
    if last:
        windows.append(range(max(0, last - 96), last + 1))
    for w in windows:
        for lba in w:
            f.seek(lba * SS)
            blob = f.read(want)
            if len(blob) == want and crc(blob) == pcrc:
                return lba
    return None

def show_entries(f, plba, pnum, psize, pcrc, last=None):
    f.seek(plba * SS)
    blob = f.read(pnum * psize)
    ok = crc(blob) == pcrc
    print(f"  entries CRC over array: {crc(blob):#010x} "
          f"{'OK' if ok else 'MISMATCH'}")
    if not ok:
        found = find_entry_array(f, pcrc, pnum, psize, last)
        if found is not None:
            print(f"  ** the array matching EntriesCRC is at LBA {found}, "
                  f"not {plba} -- the header's PartEntryLBA is wrong by "
                  f"{plba - found:+d} sectors")
        else:
            print("  ** no array matching EntriesCRC found near either end; "
                  "the entries themselves were changed, not just the pointer")
    for i in range(pnum):
        e = blob[i*psize:(i+1)*psize]
        tguid = e[0:16]
        if tguid == b"\0"*16:
            continue
        pguid, sl, el, attr = struct.unpack_from("<16sQQQ", e, 16)
        nm = e[56:128].decode("utf-16-le").split("\0")[0]
        print(f"   [{i}] {sl:>10}-{el:<10} attr={attr:#018x} "
              f"type={uuid.UUID(bytes_le=tguid)} name={nm!r}")

def main(path):
    with open(path, "rb") as f:
        size = dev_size(path, f)
        last = size // SS - 1 if size else None
        print(f"== {path}  size={size} bytes ({size//SS if size else '?'} sectors)\n")

        mbr = read_at(f, 0)
        print("Protective MBR partition entries:")
        for i in range(4):
            e = mbr[446+i*16:462+i*16]
            if e[4] == 0: continue
            st, cnt = struct.unpack_from("<II", e, 8)
            print(f"   [{i}] type={e[4]:#04x} start={st} count={cnt}")
        print()

        h = show_header("Primary GPT (LBA 1)", read_at(f, 1), last)
        if h: show_entries(f, *h[1], last)
        print()

        # An image written to a larger device leaves its backup GPT where the
        # image ended, not at the end of the device, so looking only at the last
        # LBA would report "no GPT signature" and miss the table that is
        # actually in use. Follow the primary's AlternateLBA as well.
        alt = h[0] if h else None
        if alt is not None and alt != last:
            hb = show_header(f"Backup GPT where the primary points (LBA {alt})",
                             read_at(f, alt), last)
            if hb: show_entries(f, *hb[1], last)
            print()

        if last:
            hb = show_header(f"Backup GPT at device end (LBA {last})", read_at(f, last), last)
            if hb: show_entries(f, *hb[1], last)

if __name__ == "__main__":
    if len(sys.argv) != 2:
        sys.exit(__doc__)
    main(sys.argv[1])
