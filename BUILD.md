# Build

Everything is done by a script in `tools/`. You run all of them the same way —
from the repo root, on your own machine. What differs is where the work then
happens.

Every build lands in `build/`, whichever route it took.

## What runs what

**On Windows**, in the MSYS2 UCRT64 shell:

- **`tools/build.sh`** → `build/`
  - cmake and ninja against the native Qt
- **`tools/deploy.sh`** → `dist/`
  - `windeployqt6`, then `ntldd` for what it misses
- **`tools/gpttest.sh`** → pass or fail
  - builds the harness in `tools/gpttest/`, which links the real
    `src/disk.cpp`, and runs it
- **`tools/imgtest.sh`** → pass or fail
  - the same idea for `src/imagesource.cpp`: reads images back and compares
    them with what went in. Builds its own fixtures
- **`tools/mkicon/`** → the executable's `.ico`, into the build directory
  - renders the app icon SVG into the multi-size `.ico` the executable needs.
    Run by the build itself, on both platforms; never invoked by hand
- **`tools/gui-probe.ps1`** → measurements of the running window
  - PowerShell and UI Automation, against a test build already on screen

**On Linux, with the cross toolchain installed:**

- **`tools/build-cross.sh`** → `build/`
  - cmake and ninja against the MinGW Qt
- **`tools/deploy-cross.sh`** → `dist/`
  - `objdump`, and the Qt plugins gathered by hand

**On Linux, without it** — the same build, one step further out:

- **`tools/build-container.sh`** → `build/`
  - podman, which runs **`tools/build-cross.sh`** inside the image

  You run this on the host; it is the *build* that happens in a container. The
  script is a wrapper and nothing else.

**On Windows or Linux:**

- **`tools/lupdate.sh`** → `src/lang/*.ts`
  - whichever Qt 6 `lupdate` the host has — MSYS2's `lupdate`, Fedora's
    `lupdate-qt6` — else podman running *itself* in the image

**On Linux:**

- **`tools/make-test-images.sh`** → test images
  - `sfdisk`, `mkfs.vfat`, gzip and xz
- **`tools/verify-flashed.sh`** → pass or fail
  - `cmp`, an image against a device

Everything that cross-builds reads **`tools/build-env.sh`** for the packages,
toolchain paths, cmake flags and podman plumbing — including
`tools/Containerfile.build` when it builds the image, and
[.github/workflows/build.yml](.github/workflows/build.yml), which runs
`tools/build-cross.sh` the same as anyone else.

## Windows, natively

Once:

```
# non-admin powershell:
winget install --id MSYS2.MSYS2 -e --source winget
```

Then in the **MSYS2 UCRT64** shell, from the repo root:

```
# update all msys2 base packages first:
pacman -Suy

# then install dependancy packages:
pacman -S --needed $(bash tools/build-env.sh packages-msys2)
```

Then, any time:

```
tools/build.sh                 # into build/
tools/deploy.sh                # into dist/
```

`build/Win32DiskImager.exe` only runs inside the MSYS2 shell, because its Qt
DLLs are not next to it. `dist/` is the standalone copy: move it anywhere and run
`dist/Win32DiskImager.exe` **as Administrator**, which raw device access needs.

`tools/build.sh clean` drops the build directory first. Without it, ninja stays
incremental.

## Working on the interface

Every launch of the real binary raises a UAC prompt, which gets old when the
change under test is a tooltip. This builds one that does not:

```
tools/build.sh test
```

It lands in `build/` like any other build, replacing whatever was there, so
switching back is just `tools/build.sh` again.

It **cannot open a device**, so use it for layout, tooltips, translations and
dialog text, and a normal build for anything that touches a card. Both
deploy scripts refuse to package one, since the two are indistinguishable once
the exe sits in a folder of its own. To tell them apart by hand:

```
grep -ac 'level="asInvoker"' build/Win32DiskImager.exe    # 1 = test build
```

### Measuring what it drew

Some interface faults are only a few pixels wide. A tooltip is clipped when its
box is narrower than its text needs, and no screenshot tells you that to the
pixel. `tools/gui-probe.ps1` asks the running application where its widgets are,
over UI Automation, and measures what came out:

```
powershell -NoProfile -ExecutionPolicy Bypass -File tools/gui-probe.ps1 -List
powershell -NoProfile -ExecutionPolicy Bypass -File tools/gui-probe.ps1 -Hover cboxHashType
powershell -NoProfile -ExecutionPolicy Bypass -File tools/gui-probe.ps1 -Hover bHashCopy,fixGptCheckBox -Shot tip.png
```

`-List` prints the widgets under the object names they have in
`src/mainwindow.ui`, which are the names `-Hover` takes. Hovering moves the
pointer onto each in turn and reports the tooltip left showing:

```
tip w=170 h=20 at 562,390: Generate selected hash on file
```

Compare that width against what the text needs; if the box is the narrower of
the two, the text is cut off. Naming several widgets walks the pointer from one
to the next while the first tooltip is still up, which is when Qt reuses one
tooltip label for the next -- the case where sizing has gone wrong before. If it
reports no tooltip, the previous one is usually covering the next widget, so the
pointer landed on the tooltip instead: hover that widget on its own.

Positions are read from the application again before every move, so the window
can be anywhere. It drives the real pointer, though, so leave the mouse alone
while it runs.

## Windows, cross-compiled from Linux

It has to be Fedora: Debian and Ubuntu ship no MinGW Qt6 packages, so there is
nothing to link against there. That is why CI runs `ubuntu-latest` but inside a
Fedora container. Which release is named once, as `CROSS_BASE_IMAGE` in
[tools/build-env.sh](tools/build-env.sh).

`tools/build-cross.sh` *is* the cross build. Run it on a Fedora host and it
builds; `tools/build-container.sh` runs that same script inside the container;
[.github/workflows/build.yml](.github/workflows/build.yml) runs it too. However
this is built, it is built by that one script, with the toolchain and flags from
`tools/build-env.sh`.

**On a Fedora host.** Install the toolchain once:

```
sudo bash tools/build-env.sh install
```

then:

```
tools/build-cross.sh
tools/deploy-cross.sh
```

**Anywhere podman runs**, including a Fedora host that would rather not install
the toolchain. The image builds itself on first use:

```
tools/build-container.sh
tools/deploy-cross.sh
```

That script is a wrapper: it starts the container and runs `build-cross.sh`
inside it, where the toolchain is already installed. Both take the same
arguments — `clean` to start over, `test` for a no-elevation build — and both
fail if what comes out is not a win64 PE binary, which is what a host compiler
picked up by mistake would produce.

Either one before a push catches a broken cross build without waiting on the
workflow. Everything they write is gitignored, so the build directory persists
and ninja stays incremental: a no-op rebuild is well under a second, a one-file
change around twenty.

They share `build/`, and switching between them costs one reconfigure. A cmake
cache is tied to the path it was generated for, and the container sees this tree
as `/src`, so a cache from the other route is dropped and rebuilt -- the script
says so when it happens. `BUILD_DIR=...` overrides the directory.

## Testing the GPT repair

`relocateBackupGPT()` rewrites partition tables and zeroes the stale backup left
mid-device, so a mistake there destroys data rather than merely misbehaving.
This exercises it against a file standing in for a device — no card, no VM, no
UAC prompt, about a second per run:

```
tools/gpttest.sh
```

It compiles the real `src/disk.cpp` into the harness, so the code under test is
the shipped code, and exits non-zero if any check fails — enough to gate a
commit or a release. MSYS2 UCRT64 only, being Win32 code. `clean` starts over;
otherwise it stays incremental.

The sources are in `tools/gpttest/`, a separate cmake project that builds into
`build-gpttest/` rather than `build/`, so it cannot fight the application's
cache.

Nine cases; the harness prints the check total when it finishes. Three relocate
a backup stranded mid-device. Three are cases where **nothing may be written** —
no GPT at all, a backup already at the last LBA, and a corrupt header. One has a
partition sitting over the stale copy: the relocate still happens, but nothing
inside the partition may be zeroed. Two start from a table Windows has already
rewritten — one that must be detected and repaired, and the ordinary layout,
which must *not* be reported as damage.

The cases where nothing may happen are the point. When changing the repair,
confirm the harness still fails when it should: break a guard on purpose, watch
it go red, put it back.

## Testing the image decoder

`src/imagesource.cpp` decides what bytes reach the card when the image is
compressed, and it is the easiest place in the program to be subtly wrong: a
member boundary landing mid-sector, a stream with padding between, a file that
stops in the middle. None of that is reachable from the GPT harness.

```
tools/imgtest.sh
```

It compiles the real `src/imagesource.cpp`, then reads images back through it
and compares them with the bytes that went in -- raw, gzip, xz, two-member
gzip, two-stream xz, padding between streams, and an image whose length is not
a whole number of sectors. Two more must be *rejected*: a gzip and an xz that
stop in the middle, because writing what did come out and calling it done would
put half an image on a card.

The fixtures are built by the harness itself with zlib and liblzma, so nothing
compressed is checked in and neither `gzip` nor `xz` needs to be on the path.

## Changing the icons

The icons are SVGs in `src/images/`, embedded through `gui_icons.qrc`: the three
action icons on the buttons, the folder on the browse button, and
`Win32DiskImager.svg`, which is both the window icon and the source of the
executable's icon. Edit an SVG, rebuild, and the change is in the program. There
is nothing to regenerate by hand.

The executable's icon takes one extra step, because the resource compiler accepts
an `.ico` and nothing else. The build renders it: `tools/mkicon` turns the SVG
into an `.ico` in the build directory, writing eight sizes into one file — 16
through 64 as DIBs, which every version of Windows reads, and 128 and 256 as
PNG, which is what the format expects for the large ones and keeps the file to
tens of kilobytes rather than hundreds.

The `.ico` is not in the repository. The SVG is the icon; the `.ico` is a build
artefact like the `.qm` files. It was committed once, and every cross build
then showed it as modified: the rendering is identical everywhere -- all six
DIB sizes come out byte for byte the same on MSYS2 and in the container -- but
the two PNG entries are compressed, and different zlib versions turn identical
pixels into different bytes.

mkicon has to run on the machine doing the build, which when cross-compiling is
not the machine being built for. So the application's cmake configures it
separately, without the cross toolchain, against the host's own Qt — which is
why `gcc-c++`, `qt6-qtbase-devel` and `qt6-qtsvg-devel` are in the Fedora package
list even though the application itself is built entirely with the `mingw64-`
ones. MSYS2 already has what it needs.

To see an icon at the sizes it will actually be used at, before committing to
it, `QIcon` renders an SVG at any size -- a dozen-line Qt program showing the
file at 16, 24, 32, 48 and 128 says more than looking at it full size does. A
detail that reads at 128 is often a smudge at 16.

**The wrench in the app icon is not ours.** It is Feathericon's, under the MIT
licence, which requires the notice to travel with every copy including binaries.
`THIRD-PARTY-NOTICES.txt` carries it and both deploy scripts ship it. Anything
else brought in from outside needs the same treatment: a permissive licence that
is compatible with the GPL (MIT and BSD are; Apache-2.0 is not compatible with
GPL-2), its notice added to that file, and the source recorded in a comment at
the top of the SVG.

## Updating the translations

`src/lang/*.ts` hold the translations; the build compiles them to `.qm` and
embeds those through `translations.qrc`. Adding or changing a `tr()` string does
not reach the `.ts` files on its own — until they are refreshed, a new string
falls back to English and a *reworded* one silently loses the translation it had.

```
tools/lupdate.sh              # every language
tools/lupdate.sh de fr        # only those
```

This works anywhere: it uses whichever Qt 6 `lupdate` it can find — MSYS2's
`lupdate` on Windows, Fedora's `lupdate-qt6` — and falls back to running itself
in the container when the host has neither. Only version 6 is accepted, since a
Qt 5 `lupdate` writes `.ts` files the Qt 6 build then has to interpret.

Unlike the build scripts it rewrites tracked files, so review the diff:

- **New strings** arrive as `<translation type="unfinished"></translation>`.
  `lrelease` skips them and the app shows English until someone fills them in.
- **Reworded strings** appear as a new unfinished entry while the old translation
  is kept beside it as `type="vanished"`, so a translator can adapt it. Nothing
  is lost.
- Most of the diff is `<location>` line numbers moving with the source. Expected
  and harmless.

Run it before a release, or after any commit that touches user-visible text.

## Where things are defined

Two lists would otherwise be written down in several places, so each has one home
and everything else reads it from there.

**The toolchain** — Fedora packages, the MinGW sysroot, the paths to
`lrelease-qt6` and `lupdate-qt6`, the cmake flags, the MSYS2 package list, and
the podman plumbing every container run goes through — lives in
[tools/build-env.sh](tools/build-env.sh). The container image, CI and every cross
script read it, so a local container build and the CI job cannot end up on
different toolchains. Each path can be pointed elsewhere for a host that lays
them out differently:

```
CROSS_LUPDATE=/usr/bin/lupdate-qt6 tools/lupdate.sh
```

```
tools/build-env.sh packages ci      # what CI installs
tools/build-env.sh print SYSROOT    # where it expects the MinGW tree
```

**The shipped languages** live in `src/CMakeLists.txt`, and both deploy scripts
read the list from there, so trimming Qt's translations cannot drift from the set
the build compiles:

```
set(LANGUAGES es it pl nl de fr zh_CN zh_TW ta_IN ko ja)
```

Adding one takes two edits, not one: that list, and a matching
`<file>lang/diskimager_xx.qm</file>` in `src/translations.qrc`, which names every
embedded file explicitly. Miss the second and the language compiles but is never
shipped.

## Why the deploy scripts look alike

The two deploy scripts are deliberately parallel — same files copied in, same Qt
plugin groups, same trimming of Qt's translations, same iterate-until-stable DLL
closure, same refusal to package a test build. They differ only where the
platform forces it: `windeployqt6` and `ntldd` on Windows against a hand-rolled
copy and `objdump -p` on Linux, and `deploy.sh` empties `dist/` in place rather
than deleting it, because Explorer locks the directory node on Windows.

`windeployqt` cannot run on Linux at all, being a Windows binary, which is why
the cross deploy gathers the Qt plugins and translations by hand. It also strips
the result: Fedora ships its MinGW DLLs unstripped, and `libstdc++-6.dll` alone
is ~26 MB otherwise.

The `.rc` file must reference the icon with a forward slash. Windows `windres`
accepts a backslash there; the cross build does not.
