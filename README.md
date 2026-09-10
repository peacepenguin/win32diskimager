# Personal Fork of Image Writer (win32diskimager) for Microsoft Windows

I am investigating an issue where images written to SD Cards from windows get their GPT structure corrupted.

This utility reads and writes raw image files to SD and USB memory devices.
Run it, point it at your raw image, and select the removable device to write
to. It cannot write CD-ROMs, and USB floppy is not supported.

Must be run as Administrator to access raw devices.

This program has no warranty. The authors take no responsibility for lost or
damaged data.

This tool does not currently work to create bootable Rock 5B (rk3588) images (like Batocera and Armbian) onto an SD card.

Rufus has also been seen to cause the same issue. So it may be windows or usb-sdcard adapter related.

Use Linux 'dd' if you are doing something important. I am investigating why this happens on Windows 11.

| | |
|---|---|
| Building | [BUILD.md](BUILD.md) |
| Changes | [Changelog.txt](Changelog.txt) |
| Planned work | [TODO.txt](TODO.txt) |
| Translating | [readme-translations.txt](readme-translations.txt) |

## Fork changes:
 * Removed installer
 * Updated to QT6
 * Added build notes and scripts

## Legal

Image Writer for Windows is licensed under the General Public License v2. The
full text of this license is available in [GPL-2](GPL-2).

This project uses and includes binaries of the MinGW runtime library, which is
available at http://www.mingw.org

This project uses and includes binaries of the Qt library, licensed under the
"Library General Public License" and is available at http://www.qt-project.org/.

The license text is available in [LGPL-2.1](LGPL-2.1).

Original version developed by Justin Davis <tuxdavis@gmail.com>.
Maintained by the ImageWriter developers
(http://sourceforge.net/projects/win32diskimager).
