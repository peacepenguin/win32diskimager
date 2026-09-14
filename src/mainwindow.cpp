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
 *  ---                                                               *
 *  Modified 2026 by peacepenguin (personal fork, not affiliated      *
 *  with the upstream ImageWriter project):                           *
 *  GPT repair, device list, dialogs, hashing                         *
 *  https://github.com/peacepenguin/win32diskimager                   *
 **********************************************************************/

#ifndef WINVER
#define WINVER 0x0601
#endif

#include <QtWidgets>
#include <QCoreApplication>
#include <QFileInfo>
#include <QDirIterator>
#include <QClipboard>
#include <cstdio>
#include <cstdlib>
#include <windows.h>
#include <winioctl.h>
#include <dbt.h>
#include <shlobj.h>
#include <iostream>
#include <climits>

#include "disk.h"
#include "mainwindow.h"
#include "imagesource.h"
#include "elapsedtimer.h"

MainWindow* MainWindow::instance = NULL;

// Sectors moved per pass through a transfer loop, and the step the progress
// bar advances by. Named once so read, write and verify cannot drift apart.
static const unsigned long long TRANSFER_SECTORS = 1024ull;

// QProgressBar counts in int, and a multi-terabyte disk has more sectors than
// an int holds -- which "Show all devices" makes reachable. Progress is
// reported shifted right by this much so the bar does not wrap negative.
static int progressShift(unsigned long long total)
{
    int shift = 0;
    while ((total >> shift) > (unsigned long long)INT_MAX)
    {
        ++shift;
    }
    return shift;
}

// How far the bar should count. The loop runs to the device size when the image
// size is only an estimate, but the bar tracks the estimate: it then describes
// the image rather than the card.
static unsigned long long progressTotalFor(const ImageSource &image,
                                           unsigned long long numsectors)
{
    if (!image.sizeKnown() && image.sizeInSectors() > 0ull
        && image.sizeInSectors() < numsectors)
    {
        return image.sizeInSectors();
    }
    return numsectors;
}

// Qt turns word wrap on for a tooltip only when the text looks like rich text
// (Qt::mightBeRichText). A long plain tooltip therefore becomes one enormous
// line, which Qt then clamps against the edges of the screen: the beginning and
// the end are both cut off.
//
// So break the text into lines here and leave it as plain text. The obvious
// alternative, wrapping it in a little HTML table to turn Qt's own wrapping on,
// costs more than it looks: the cell needs a width in pixels, which is a made-up
// number that every tooltip over the threshold then gets padded out to whether
// it needs it or not, and rich text makes Qt size the label through a
// QTextDocument. Plain text with newlines in it sizes to its widest line and
// nothing else.
//
// Applied here rather than in the .ui so the translated strings are wrapped
// too -- a translation is often longer than the English -- and so the strings
// the translators work from stay free of markup.
static QString wrapToolTipText(const QString &tip, int maxWidthPx, const QFontMetrics &fm)
{
    QStringList out;
    QString line;

    const QStringList words = tip.split(QChar(' '), Qt::SkipEmptyParts);
    for (const QString &word : words)
    {
        const QString candidate = line.isEmpty() ? word : line + QChar(' ') + word;
        // A single word wider than the budget still goes on its own line: a
        // tooltip a little too wide beats one with a word broken in half.
        if (!line.isEmpty() && fm.horizontalAdvance(candidate) > maxWidthPx)
        {
            out.append(line);
            line = word;
        }
        else
        {
            line = candidate;
        }
    }
    if (!line.isEmpty())
    {
        out.append(line);
    }
    return out.join(QChar('\n'));
}

static void wrapLongToolTips(QWidget *root)
{
    // Wide enough to read a sentence across without the eye losing its place,
    // narrow enough to sit beside the window rather than across it. Nothing is
    // padded out to this: it is a ceiling, not a width.
    const int maxWidthPx = 380;
    const QFontMetrics fm(QToolTip::font());

    const QList<QWidget *> widgets = root->findChildren<QWidget *>();
    for (QWidget *w : widgets)
    {
        const QString tip = w->toolTip();
        // Anything that already fits needs no help, and anything already marked
        // up is the author's business.
        if (tip.isEmpty() || Qt::mightBeRichText(tip)
            || fm.horizontalAdvance(tip) <= maxWidthPx)
        {
            continue;
        }
        w->setToolTip(wrapToolTipText(tip, maxWidthPx, fm));
    }
}

// Point the bar at a run of total sectors, start the clocks, and reset the
// marker showThroughput() measures from. Returns the shift the loop has to
// apply before calling setValue(), since the bar counts in int and the sector
// count does not.
int MainWindow::beginProgress(unsigned long long total, unsigned long long *lastsector)
{
    const int shift = progressShift(total);
    progressbar->setRange(0, (total == 0ull) ? 100 : (int)(total >> shift));
    *lastsector = 0ull;
    update_timer.start();
    elapsed_timer->start();
    return shift;
}

// An idle progress bar is a line that means nothing, so the bar is hidden until
// something is running. The group it sits in stays where it is, and the bar goes
// on reserving its space while hidden, so the window neither empties out nor
// shifts when an operation starts.
void MainWindow::showProgress(bool show)
{
    progressbar->reset();
    progressbar->setVisible(show);
}

// Take the device and open the image, which write and verify both have to do
// before they can start. On failure it has already reported, cleaned up and
// put the window back to idle, so the caller only returns.
//
// Every failure path below closes the disk before releasing the volume locks,
// the same order the successful write uses and for the same reason: unlocking
// lets mountmgr rescan the disk at once, and a rescan is what triggers the
// Windows GPT "repair" this program exists to avoid. It matters most on the
// paths taken after sectors have already been written.
bool MainWindow::acquireDeviceAndImage(int deviceID, LockedVolumes &locked,
                                       ImageSource &image,
                                       unsigned long long *devicesectors,
                                       const QString &errorTitle,
                                       const QString &failedMessage)
{
    // Leaving the other volumes mounted lets their filesystem drivers flush
    // cached metadata into the middle of the run.
    if (!locked.lockAll(deviceID))
    {
        endRun(failedMessage);
        return false;
    }
    // The device goes first: the image reader needs its sector size to hand out
    // whole sectors, compressed or not. Read-write either way -- the GPT fix
    // reads the table back, and verify needs write access to offline the disk
    // when it is done.
    hRawDisk = getHandleOnDevice(deviceID, GENERIC_READ | GENERIC_WRITE);
    if (hRawDisk == INVALID_HANDLE_VALUE)
    {
        locked.release();
        endRun(failedMessage);
        return false;
    }
    bool geometryreported = false;
    *devicesectors = getNumberOfSectors(hRawDisk, &sectorsize, &geometryreported);
    if (!*devicesectors && !geometryreported)
    {
        // A card reader whose card has been pulled stays present and reports
        // size zero, with no WM_DEVICECHANGE to say so.
        QMessageBox::critical(this, tr("Device Error"),
            tr("The device reports a size of zero. If it is a card reader, "
               "the card may have been removed."));
        CloseHandle(hRawDisk);
        hRawDisk = INVALID_HANDLE_VALUE;
        locked.release();
        endRun(failedMessage);
        return false;
    }
    if (!*devicesectors)
    {
        // getNumberOfSectors already said what went wrong.
        CloseHandle(hRawDisk);
        hRawDisk = INVALID_HANDLE_VALUE;
        locked.release();
        endRun(failedMessage);
        return false;
    }
    // A .img.gz or .img.xz is decompressed as it goes, so the machine never
    // needs room for the expanded image.
    if (!image.open(leFile->text(), sectorsize))
    {
        QMessageBox::critical(this, errorTitle, image.errorString());
        CloseHandle(hRawDisk);
        hRawDisk = INVALID_HANDLE_VALUE;
        locked.release();
        endRun(failedMessage);
        return false;
    }
    return true;
}

// Does the part of the image that will not fit hold anything but zeros? Write
// and verify both ask before offering to go ahead with a truncated run.
// A compressed image is read forwards only, so examining its tail would mean
// decompressing the whole image first; that case returns false for "not
// examined" and leaves *datafound alone.
bool MainWindow::imageTailHasData(ImageSource &image, unsigned long long from,
                                  unsigned long long to, bool *datafound)
{
    *datafound = false;
    if (image.isCompressed())
    {
        return false;
    }
    for (unsigned long long at = from; at < to && !*datafound; )
    {
        unsigned long chunk = ((to - at) >= TRANSFER_SECTORS) ? TRANSFER_SECTORS
                                                     : (unsigned long)(to - at);
        char *data = image.read(at, chunk, NULL);
        if (data == NULL)
        {
            // A read error in a stretch that will not be written or compared
            // says nothing useful; stop looking rather than report it.
            break;
        }
        unsigned long long limit = (unsigned long long)chunk * sectorsize;
        for (unsigned long long j = 0ull; j < limit; ++j)
        {
            if (data[j] != 0)
            {
                *datafound = true;
                break;
            }
        }
        delete[] data;
        at += chunk;
    }
    return true;
}

// The throughput line, at most once a second. All three transfer loops report
// the same figure the same way; written out separately they had drifted into
// two spellings of "MB/s".
void MainWindow::showThroughput(unsigned long long sector, unsigned long long total,
                                unsigned long long *lastsector)
{
    if (update_timer.elapsed() < ONE_SEC_IN_MS)
    {
        return;
    }
    const double mbpersec =
        (((double)sectorsize * (sector - *lastsector))
         * ((double)ONE_SEC_IN_MS / update_timer.elapsed())) / 1024.0 / 1024.0;
    statusbar->showMessage(QString("%1 MB/s").arg(mbpersec));
    elapsed_timer->update(sector, total);
    update_timer.start();
    *lastsector = sector;
}

// Every run that stops -- failed, cancelled or done -- ends the same way. It
// was written out at all 25 exits before, so a new one only had to forget a
// line to leave the buttons disabled or the progress bar up.
void MainWindow::endRun(const QString &message)
{
    status = STATUS_IDLE;
    showProgress(false);
    statusbar->showMessage(message);
    bCancel->setEnabled(false);
    setReadWriteButtonState();
}

// The status bar carries a message most of the time, but when it is empty it
// reads as blank space rather than as a part of the window with a job. A shade
// off the window colour and a hairline above it give it an edge to sit in.
//
// The shade is taken from the palette rather than written down, so it follows
// the system theme: a touch darker on a light background, a touch lighter on a
// dark one.
static void shadeStatusBar(QStatusBar *bar)
{
    const QColor window = bar->palette().color(QPalette::Window);
    const bool dark = window.lightness() < 128;
    const QColor fill = dark ? window.lighter(118) : window.darker(106);
    const QColor line = dark ? window.lighter(140) : window.darker(118);
    bar->setStyleSheet(QString("QStatusBar { background: %1; border-top: 1px solid %2; }"
                               "QStatusBar::item { border: none; }")
                           .arg(fill.name(), line.name()));
}

// This style says "pressed" by dimming the button's label. A button whose only
// label is an icon has nothing to dim -- QIcon draws the same pixmap whether the
// button is down or not -- so the browse button looked dead when held. Its
// background does change, from #F6F6F6 to #F5F5F5: one level out of 255, which
// nobody can see. Give it a fill a clear step beyond the one hover uses.
//
// Shaded from the palette like the status bar above, so it follows a dark
// desktop instead of turning into a light patch on one. The rule names the
// button's own class so that it cannot leak into anything else -- an
// unscoped rule here is inherited by the widget's tooltip, which is how two
// other buttons ended up with padded tooltips and no pressed state at all.
static void shadePressedIconButton(QAbstractButton *button)
{
    const QColor base = button->palette().color(QPalette::Button);
    const bool dark = base.lightness() < 128;
    const QColor fill = dark ? base.lighter(128) : base.darker(108);

    button->setStyleSheet(QString("%1:pressed { background-color: %2; }")
                              .arg(QString::fromLatin1(button->metaObject()->className()),
                                   fill.name()));
}

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
{
    setupUi(this);
    wrapLongToolTips(this);
    elapsed_timer = new ElapsedTimer();
    shadeStatusBar(statusbar);
    shadePressedIconButton(tbBrowse);
    statusbar->addPermanentWidget(elapsed_timer);   // "addpermanent" puts it on the RHS of the statusbar
    status = STATUS_IDLE;
    {
        // Hiding a widget normally takes its space with it; this keeps it.
        QSizePolicy sp = progressbar->sizePolicy();
        sp.setRetainSizeWhenHidden(true);
        progressbar->setSizePolicy(sp);
    }
    showProgress(false);
    clipboard = QApplication::clipboard();
    hFile = INVALID_HANDLE_VALUE;
    hRawDisk = INVALID_HANDLE_VALUE;
    if (QCoreApplication::arguments().count() > 1)
    {
        QString fileLocation = QApplication::arguments().at(1);
        QFileInfo fileInfo(fileLocation);
        // Backslashes: Qt hands out '/' whatever the platform, and a Windows
        // user reading their own path expects the separator they type.
        leFile->setText(QDir::toNativeSeparators(fileInfo.absoluteFilePath()));
    }
    // Add supported hash types.
    cboxHashType->addItem("MD5",QVariant(QCryptographicHash::Md5));
    cboxHashType->addItem("SHA1",QVariant(QCryptographicHash::Sha1));
    cboxHashType->addItem("SHA256",QVariant(QCryptographicHash::Sha256));
    connect(this->cboxHashType, SIGNAL(currentIndexChanged(int)), SLOT(on_cboxHashType_IdxChg()));
    // An image named on the command line counts as a selection too. After the
    // list is filled, or there would be nothing to select.
    imageFileChanged();
    sectorData = NULL;
    sectorData2 = NULL;
    sectorsize = 0ul;

    // Nothing is remembered between runs: the program writes no settings
    // anywhere, so a copy of it leaves nothing behind on the machine.
    //
    // Both boxes start off the same way every time, and for the same reason in
    // each case. Unchecking the GPT fix once, for one card, would otherwise
    // leave every later write open to the rewrite this program exists to
    // prevent; and starting with only removable devices listed means a fixed
    // disk is never preselected from a previous session.
    fixGptCheckBox->setChecked(true);
    showAllDevicesCheckBox->setChecked(false);
    // After the "show all devices" state is set, which the filter reads.
    //
    // Deferred rather than called here. The scan asks every disk for its
    // geometry, and a disk that has spun down does not answer until it is
    // turning again -- eleven seconds, measured, on a machine with sleeping
    // platters. From the constructor that wait falls before main() reaches
    // show(), so the program sits there owning no window at all and looks to
    // the user like it failed to start.
    //
    // A zero-millisecond timer fires once the window system's queue has been
    // emptied, which is to say once the window has been shown and painted. The
    // scan still blocks the interface while it runs -- same work, same thread
    // -- but it does it behind a window that is up and says what it is doing.
    QTimer::singleShot(0, this, [this]() { rescanDevices(); });
    // Inserting a card into a reader that presents no volume produces no
    // WM_DEVICECHANGE broadcast, so arrival cannot be left to that alone. The
    // list used to be polled every two seconds, which asked every disk on the
    // machine for its geometry around the clock -- enough to keep spinning
    // disks from ever sleeping, and to do it for a list that is empty by
    // default. Rescanning as the list is opened covers the same case at the
    // one moment the contents have to be right, and leaves the machine alone
    // the rest of the time.
    connect(cboxDevice, &DeviceComboBox::aboutToShowPopup,
            this, &MainWindow::rescanDevices);

    initializeHomeDir();
    myFileType = tr("Disk Images (*.img *.IMG *.img.gz *.img.xz)");
    myFileTypeList << tr("Disk Images (*.img *.IMG *.img.gz *.img.xz)")
                   << tr("Compressed Disk Images (*.img.gz *.img.xz *.gz *.xz)")
                   << "*.*";

    // Last, once every string the window will show has been set: the size the
    // layout needs depends on the words in it, and the words depend on the
    // language.
    //
    // mainwindow.ui is drawn at 520x355 for English, and carries a minimum
    // width of 460 so nobody can drag it narrower than it reads. That minimum
    // is why this is needed: Qt gives a window its layout's minimum size only
    // when the window has no minimum of its own, so the explicit 460 stands in
    // for the real figure, and a translation needing more than 520 is not
    // given it. The layout then takes the space out of the widgets, and a
    // QCheckBox does not elide -- it just draws its label with the end missing,
    // which is what Tamil looked like.
    //
    // expandedTo() so this can only grow the window: English keeps the size it
    // was designed at, and a longer language gets what its layout asks for.
    resize(sizeHint().expandedTo(size()));
}

MainWindow::~MainWindow()
{
    if (hRawDisk != INVALID_HANDLE_VALUE)
    {
        CloseHandle(hRawDisk);
        hRawDisk = INVALID_HANDLE_VALUE;
    }
    if (hFile != INVALID_HANDLE_VALUE)
    {
        CloseHandle(hFile);
        hFile = INVALID_HANDLE_VALUE;
    }
    if (sectorData != NULL)
    {
        delete[] sectorData;
        sectorData = NULL;
    }
    if (sectorData2 != NULL)
    {
        delete[] sectorData2;
        sectorData2 = NULL;
    }
    if (elapsed_timer != NULL)
    {
        delete elapsed_timer;
        elapsed_timer = NULL;
    }
    if (cboxHashType != NULL)
    {
       cboxHashType->clear();
    }
}


void MainWindow::initializeHomeDir()
{
    // Straight to the downloads directory. Nothing reads myHomeDir before the
    // end of this function, so working out a home directory first and then
    // overwriting it unconditionally achieved nothing.
    QString downloadPath = qgetenv("DiskImagesDir");
    if (downloadPath.isEmpty()) {
        PWSTR pPath = NULL;
        static GUID downloads = {0x374de290, 0x123f, 0x4565, {0x91, 0x64, 0x39,
                                 0xc4, 0x92, 0x5e, 0x46, 0x7b}};
        if (SHGetKnownFolderPath(downloads, 0, 0, &pPath) == S_OK) {
            downloadPath = QDir::fromNativeSeparators(QString::fromWCharArray(pPath));
            CoTaskMemFree(pPath);
        }
        // Also when the shell gave no answer at all, not just a stale one.
        if (downloadPath.isEmpty() || !QDir(downloadPath).exists()) {
            downloadPath = QStandardPaths::writableLocation(QStandardPaths::DownloadLocation);
        }
    }
    if (downloadPath.isEmpty())
        downloadPath = QDir::currentPath();
    myHomeDir = downloadPath;
}

void MainWindow::setReadWriteButtonState()
{
    bool fileSelected = !(leFile->text().isEmpty());
    bool deviceSelected = (cboxDevice->count() > 0);
    QFileInfo fi(leFile->text());

    // set read and write buttons according to status of file/device
    bRead->setEnabled(deviceSelected && fileSelected && (fi.exists() ? fi.isWritable() : true));
    bWrite->setEnabled(deviceSelected && fileSelected && fi.isReadable());
    bVerify->setEnabled(deviceSelected && fileSelected && fi.isReadable());
    // This one needs no image: it only looks at the device.
    bCheckGpt->setEnabled(deviceSelected);
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    // The three running states ask the same question about different stakes.
    // The sentences stay whole rather than being assembled from parts: a
    // translator needs the whole sentence, and splitting them would throw away
    // the translations these already have.
    QString atstake;
    if (status == STATUS_READING)
    {
        atstake = tr("Exiting now will result in a corrupt image file.\n"
                     "Are you sure you want to exit?");
    }
    else if (status == STATUS_WRITING)
    {
        atstake = tr("Exiting now will result in a corrupt disk.\n"
                     "Are you sure you want to exit?");
    }
    else if (status == STATUS_VERIFYING)
    {
        atstake = tr("Exiting now will cancel verifying image.\n"
                     "Are you sure you want to exit?");
    }
    else
    {
        return;      // nothing running: let the window close
    }

    if (QMessageBox::warning(this, tr("Exit?"), atstake,
                             QMessageBox::Yes | QMessageBox::No,
                             QMessageBox::No) == QMessageBox::Yes)
    {
        status = STATUS_EXIT;
    }
    event->ignore();
}

void MainWindow::on_tbBrowse_clicked()
{
    // Use the location of already entered file
    QString fileLocation = leFile->text();
    QFileInfo fileinfo(fileLocation);

    // See if there is a user-defined file extension.
    QString fileTypeEnv = qgetenv("DiskImagerFiles");

    QStringList fileTypesList = fileTypeEnv.split(";;", Qt::SkipEmptyParts) + myFileTypeList;
    int index = fileTypesList.indexOf(myFileType);
    if (index != -1) {
        fileTypesList.move(index, 0);
    }

    // create a generic FileDialog
    QFileDialog dialog(this, tr("Select a disk image"));
    dialog.setNameFilters(fileTypesList);
    dialog.setFileMode(QFileDialog::AnyFile);
    dialog.setViewMode(QFileDialog::Detail);
    if (fileinfo.exists())
    {
        dialog.selectFile(fileLocation);
    }
    else
    {
        dialog.setDirectory(myHomeDir);
    }

    if (dialog.exec())
    {
        // selectedFiles returns a QStringList - we just want 1 filename,
        //	so use the zero'th element from that list as the filename
        fileLocation = (dialog.selectedFiles())[0];
        myFileType = dialog.selectedNameFilter();

        if (!fileLocation.isNull())
        {
            leFile->setText(QDir::toNativeSeparators(fileLocation));
            QFileInfo newFileInfo(fileLocation);
            myHomeDir = newFileInfo.absolutePath();
        }
        imageFileChanged();
    }
}

void MainWindow::on_bHashCopy_clicked()
{
    QString hashSum(hashLabel->text());
    if ( !(hashSum.isEmpty()) )
    {
        clipboard->setText(hashSum);
    }
}

// generates the hash
void MainWindow::generateHash(const QString &filename, int hashish)
{
    hashLabel->setText(tr("Generating..."));
    hashLabel->setVisible(true);
    QApplication::processEvents();

    QCryptographicHash filehash((QCryptographicHash::Algorithm)hashish);

    // may take a few secs - display a wait cursor
    QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));

    QFile file(filename);
    if (!file.open(QFile::ReadOnly))
    {
        hashLabel->setText(tr("Error"));
        bHashCopy->setEnabled(false);
        QApplication::restoreOverrideCursor();
        QMessageBox::critical(this, tr("File Error"),
                              tr("Could not open the file to generate a checksum:\n%1").arg(file.errorString()));
        return;
    }
    // A read that stops part-way still returns a well-formed digest -- of the
    // wrong bytes. Reporting that as the file's checksum defeats the point.
    if (!filehash.addData(&file))
    {
        hashLabel->setText(tr("Error"));
        bHashCopy->setEnabled(false);
        QApplication::restoreOverrideCursor();
        QMessageBox::critical(this, tr("File Error"),
                              tr("Could not read the whole file to generate a checksum:\n%1").arg(file.errorString()));
        return;
    }

    hashLabel->setText(filehash.result().toHex());
    bHashCopy->setEnabled(true);
    // redisplay the normal cursor
    QApplication::restoreOverrideCursor();
}


// Tell the user the table is broken and put it right if they say so. `lead`
// opens the sentence, because a verify and a check on its own meet the damage
// in different circumstances. Returns true if the table was repaired.
bool MainWindow::offerGptRepair(HANDLE hDisk, unsigned long long devicesectors,
                                const QString &lead)
{
    const int answer = QMessageBox::warning(this, tr("Partition table damaged"),
        tr("%1 the primary GPT header points at sectors the partition entries "
           "are not in.\n\n"
           "This is what Windows leaves behind when it rescans a card written "
           "without \"Fix GPT after write\". No data has been lost, but the "
           "device will not boot and most tools will refuse the table.\n\n"
           "Repair the partition table now?").arg(lead),
        QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
    if (answer != QMessageBox::Yes)
    {
        return false;
    }

    QString detail;
    if (repairPrimaryGpt(hDisk, sectorsize, devicesectors, &detail))
    {
        return true;
    }
    QMessageBox::critical(this, tr("Repair failed"),
        tr("The partition table could not be repaired: %1").arg(detail));
    return false;
}

// Look at the selected device's partition table on its own, writing and
// comparing nothing. A card Windows has already rescanned still matches its
// image sector for sector, so the table is the only place the damage shows.
void MainWindow::on_bCheckGpt_clicked()
{
    const int deviceID = selectedDeviceID();
    if (deviceID < 0)
    {
        QMessageBox::critical(this, tr("Device Error"), tr("Please select a device."));
        return;
    }

    // Locked for the reason a verify locks: nothing else may write to the disk
    // while the table is being read, still less while it is being repaired.
    LockedVolumes locked;
    if (!locked.lockAll(deviceID))
    {
        statusbar->showMessage(tr("Could not lock the device."));
        return;
    }
    HANDLE hDisk = getHandleOnDevice(deviceID, GENERIC_READ | GENERIC_WRITE);
    if (hDisk == INVALID_HANDLE_VALUE)
    {
        locked.release();
        statusbar->showMessage(tr("Could not open the device."));
        return;
    }
    unsigned long long devicesectors = getNumberOfSectors(hDisk, &sectorsize);
    if (!devicesectors)
    {
        CloseHandle(hDisk);
        locked.release();
        QMessageBox::critical(this, tr("Device Error"),
            tr("The device reports a size of zero. If it is a card reader, "
               "the card may have been removed."));
        return;
    }

    switch (gptPrimaryState(hDisk, sectorsize, devicesectors))
    {
    case GPT_PRIMARY_BROKEN:
        if (offerGptRepair(hDisk, devicesectors,
                tr("This device's partition table is broken:")))
        {
            statusbar->showMessage(tr("Partition table repaired."));
        }
        else
        {
            statusbar->showMessage(tr("Partition table is still damaged."));
        }
        break;
    case GPT_PRIMARY_OK:
        statusbar->showMessage(tr("Partition table is valid."));
        QMessageBox::information(this, tr("Partition table"),
            tr("The GPT on this device is valid: the header and the partition "
               "entries it points at agree."));
        break;
    case GPT_PRIMARY_NO_GPT:
        statusbar->showMessage(tr("No GPT on this device."));
        QMessageBox::information(this, tr("Partition table"),
            tr("This device has no GPT, so it cannot have the damage this "
               "checks for."));
        break;
    default:
        statusbar->showMessage(tr("Could not read the partition table."));
        QMessageBox::warning(this, tr("Partition table"),
            tr("The partition table could not be read, or is damaged in some "
               "way other than the one this repairs."));
        break;
    }

    CloseHandle(hDisk);
    locked.release();
}

// SHA256 is what image publishers overwhelmingly quote, so selecting an image
// arms the checksum most people are about to compare against. Only when the
// image actually changes: the field emits editingFinished whenever it loses
// focus, and resetting the type every time would undo a deliberate choice of
// MD5 or SHA1. Looked up by name rather than index so the order of the list is
// free to change.
void MainWindow::defaultHashTypeForFile()
{
    const QString file = leFile->text();
    if (file.isEmpty() || file == myHashDefaultedFor)
    {
        return;
    }
    myHashDefaultedFor = file;
    const int sha256 = cboxHashType->findText("SHA256");
    if (sha256 >= 0)
    {
        cboxHashType->setCurrentIndex(sha256);
    }
}

void MainWindow::on_leFile_editingFinished()
{
    // A pasted path may use either separator; show it the way the rest of
    // Windows would. Qt and the Win32 API take both, so this is presentation
    // only -- and setText does not re-emit this signal.
    const QString typed = leFile->text();
    const QString native = QDir::toNativeSeparators(typed);
    if (native != typed)
    {
        leFile->setText(native);
    }
    imageFileChanged();
}

void MainWindow::on_bCancel_clicked()
{
    if ( (status == STATUS_READING) || (status == STATUS_WRITING) )
    {
        if (QMessageBox::warning(this, tr("Cancel?"), tr("Canceling now will result in a corrupt destination.\n"
                                                         "Are you sure you want to cancel?"),
                                 QMessageBox::Yes|QMessageBox::No, QMessageBox::No) == QMessageBox::Yes)
        {
            status = STATUS_CANCELED;
        }
    }
    else if (status == STATUS_VERIFYING)
    {
        if (QMessageBox::warning(this, tr("Cancel?"), tr("Cancel Verify.\n"
                                                         "Are you sure you want to cancel?"),
                                 QMessageBox::Yes|QMessageBox::No, QMessageBox::No) == QMessageBox::Yes)
        {
            status = STATUS_CANCELED;
        }

    }
}

void MainWindow::on_bWrite_clicked()
{
    bool passfail = true;
    if (!leFile->text().isEmpty())
    {
        QFileInfo fileinfo(leFile->text());
        if (fileinfo.exists() && fileinfo.isFile() &&
                fileinfo.isReadable() && (fileinfo.size() > 0) )
        {
            if (fileIsOnSelectedDevice(leFile->text()))
            {
                QMessageBox::critical(this, tr("Write Error"), tr("Image file cannot be located on the target device."));
                return;
            }
            int deviceID = selectedDeviceID();
            if (deviceID < 0)
            {
                QMessageBox::critical(this, tr("Write Error"), tr("Please select a target device."));
                return;
            }
            if (QMessageBox::warning(this, tr("Confirm overwrite"), tr("Writing to a physical device can corrupt the device.\n"
                                                                       "(Target Device: %1)\n"
                                                                       "Are you sure you want to continue?").arg(cboxDevice->currentText()),
                                     QMessageBox::Yes | QMessageBox::No, QMessageBox::No) == QMessageBox::No)
            {
                return;
            }
            // A target carrying mounted volumes is the shape of a mistake: a
            // card straight from an imaging tool has no letter Windows can
            // mount, so letters usually mean this is someone's data drive.
            QString targetletters = driveLettersOnDevice((ULONG)deviceID);
            if (!targetletters.isEmpty())
            {
                if (QMessageBox::warning(this, tr("Device has mounted volumes"),
                        tr("%1 is mounted in Windows as %2.\n\n"
                           "Everything on this device, on every one of its partitions, will be "
                           "destroyed and cannot be recovered.\n\n"
                           "Check that %2 is not a drive you meant to keep.\n\n"
                           "Write to this device anyway?")
                            .arg(cboxDevice->currentText()).arg(targetletters),
                        QMessageBox::Yes | QMessageBox::No, QMessageBox::No) == QMessageBox::No)
                {
                    return;
                }
            }
            status = STATUS_WRITING;
            showProgress(true);
            bCancel->setEnabled(true);
            bWrite->setEnabled(false);
            bRead->setEnabled(false);
            bVerify->setEnabled(false);
            unsigned long long i, lasti, availablesectors, numsectors;
            LockedVolumes locked;
            ImageSource image;
            if (!acquireDeviceAndImage(deviceID, locked, image, &availablesectors,
                                       tr("Write Error"), tr("Write failed.")))
            {
                return;
            }
            // gzip only records the uncompressed size modulo 4 GiB, so for any
            // real image it is a lower bound rather than a size. The write then
            // runs until the stream ends, with the device size as the loop
            // bound, and the leftover check below says whether it all fitted.
            const bool sizeisestimate = !image.sizeKnown();
            const unsigned long long imagesectors = image.sizeInSectors();
            numsectors = sizeisestimate ? availablesectors : imagesectors;
            if (!numsectors)
            {
                // The image, not the device: an empty file, or a compressed
                // one whose stream holds nothing. The card-reader comment
                // this used to carry belongs on the device-size check.
                QMessageBox::critical(this, tr("File Error"),
                                      tr("The specified file contains no data."));
                CloseHandle(hRawDisk);
                hRawDisk = INVALID_HANDLE_VALUE;
                locked.release();
                endRun(tr("Write failed."));
                return;
            }
            // An estimated size is only a lower bound, but a lower bound that
            // already exceeds the device is enough to say the image will not
            // fit. Saying so here beats finding out at the end of the card,
            // which is the only other moment it can be detected.
            if (sizeisestimate && imagesectors > availablesectors)
            {
                QString msg = tr("The image is larger than the device:\n"
                                 "  Image: at least %1 sectors\n"
                                 "  Available: %2 sectors\n  Sector Size: %3\n\n"
                                 "The end of the image will not be written, so the device "
                                 "will not hold a complete image.\n\nContinue Anyway?");
                msg = msg.arg(imagesectors).arg(availablesectors).arg(sectorsize);
                if (QMessageBox::warning(this, tr("Not enough available space!"), msg,
                                         QMessageBox::Ok, QMessageBox::Cancel) != QMessageBox::Ok)
                {
                    CloseHandle(hRawDisk);
                    hRawDisk = INVALID_HANDLE_VALUE;
                    locked.release();
                    endRun(tr("Write failed."));
                    return;
                }
            }
            if (numsectors > availablesectors)
            {
                bool datafound = false;
                bool tailchecked = imageTailHasData(image, availablesectors,
                                                    numsectors, &datafound);
                // Built from whole translatable sentences. Assembling the text
                // first and passing it through tr() would look up a string that
                // only exists at runtime, so nothing is ever translated.
                QString msg = (!tailchecked)
                    ? tr("More space required than is available:\n  Required: %1 sectors\n"
                         "  Available: %2 sectors\n  Sector Size: %3\n\n"
                         "The extra space could not be checked for data, because the image "
                         "is compressed\n\nContinue Anyway?")
                    : (datafound)
                        ? tr("More space required than is available:\n  Required: %1 sectors\n"
                             "  Available: %2 sectors\n  Sector Size: %3\n\n"
                             "The extra space DOES appear to contain data\n\nContinue Anyway?")
                        : tr("More space required than is available:\n  Required: %1 sectors\n"
                             "  Available: %2 sectors\n  Sector Size: %3\n\n"
                             "The extra space does not appear to contain data\n\nContinue Anyway?");
                msg = msg.arg(numsectors).arg(availablesectors).arg(sectorsize);
                if(QMessageBox::warning(this, tr("Not enough available space!"),
                                        msg, QMessageBox::Ok, QMessageBox::Cancel) == QMessageBox::Ok)
                {
                    // truncate the image at the device size...
                    numsectors = availablesectors;
                }
                else    // Cancel
                {
                    CloseHandle(hRawDisk);
                    hRawDisk = INVALID_HANDLE_VALUE;
                    locked.release();
                    endRun(tr("Write cancelled."));
                    return;
                }
            }

            // Clear any partition table left over from a previous image before
            // laying down the new one, so no stale backup GPT survives at the
            // end of the device for Windows to reconcile against.
            statusbar->showMessage(tr("Clearing old partition tables..."));
            QCoreApplication::processEvents();
            if (!wipePartitionTables(hRawDisk, sectorsize, availablesectors))
            {
                QMessageBox::critical(this, tr("Write Error"),
                    tr("Could not clear the existing partition tables on the device.")
                    + "\n\n" + tr("The device has been partially written and no longer holds "
                                  "a usable image. Write the image again before using it."));
                CloseHandle(hRawDisk);
                hRawDisk = INVALID_HANDLE_VALUE;
                locked.release();
                endRun(tr("Write failed."));
                return;
            }

            const unsigned long long progresstotal = progressTotalFor(image, numsectors);
            const int progshift = beginProgress(progresstotal, &lasti);
            // Until the first throughput figure a second from now, the status
            // bar would otherwise still read "Clearing old partition tables".
            statusbar->showMessage(tr("Writing..."));
            bool imagetruncated = false;
            for (i = 0ul; i < numsectors && status == STATUS_WRITING; i += TRANSFER_SECTORS)
            {
                unsigned long long chunk = (numsectors - i >= TRANSFER_SECTORS) ? TRANSFER_SECTORS : (numsectors - i);
                unsigned long long got = 0ull;
                sectorData = image.read(i, chunk, &got);
                if (sectorData == NULL)
                {
                    QMessageBox::critical(this, tr("Write Error"),
                        image.errorString()
                        + "\n\n" + tr("The device has been partially written and no longer holds "
                                      "a usable image. Write the image again before using it."));
                    CloseHandle(hRawDisk);
                    hRawDisk = INVALID_HANDLE_VALUE;
                    locked.release();
                    endRun(tr("Write failed."));
                    return;
                }
                if (got == 0ull)
                {
                    // The image ended exactly on the previous chunk.
                    delete[] sectorData;
                    sectorData = NULL;
                    numsectors = i;
                    break;
                }
                if (!writeSectorDataToHandle(hRawDisk, sectorData, i, got, sectorsize))
                {
                    // writeSectorDataToHandle has already reported what went
                    // wrong; this says what it leaves behind on the device.
                    QMessageBox::warning(this, tr("Write Error"),
                        tr("The device has been partially written and no longer holds "
                           "a usable image. Write the image again before using it."));
                    delete[] sectorData;
                    CloseHandle(hRawDisk);
                    locked.release();
                    sectorData = NULL;
                    hRawDisk = INVALID_HANDLE_VALUE;
                    endRun(tr("Write failed."));
                    return;
                }
                delete[] sectorData;
                sectorData = NULL;
                if (got < chunk)
                {
                    // Short read: the image ended inside this chunk.
                    numsectors = i + got;
                    progressbar->setValue(
                        (int)((numsectors > progresstotal ? progresstotal : numsectors) >> progshift));
                    QCoreApplication::processEvents();
                    break;
                }
                QCoreApplication::processEvents();
                showThroughput(i, progresstotal, &lasti);
                // i is where this chunk started; the bar tracks what is done,
                // which is the end of it.
                unsigned long long written = i + chunk;
                progressbar->setValue(
                    (int)((written > progresstotal ? progresstotal : written) >> progshift));
                QCoreApplication::processEvents();
            }
            // Without an exact size the loop bound came from the device, not
            // from the image, so it may have stopped with image still to come.
            // Ask the stream rather than trusting the size that set the bound.
            if (!image.sizeKnown() && status == STATUS_WRITING)
            {
                unsigned long long leftover = 0ull;
                char *extra = image.read(numsectors, 1ull, &leftover);
                delete[] extra;
                imagetruncated = (leftover > 0ull);
            }
            // Order matters. Flush and close the raw disk first: unlocking the
            // volumes lets mountmgr rescan the disk immediately, and a rescan
            // is what triggers Windows' automatic GPT "repair".
            flushDevice(hRawDisk);
            image.close();

            // Make the table consistent with the device before anything can
            // rescan it, so Windows finds nothing to "repair".
            GptFixResult gptfix = GPT_FIX_DISABLED;
            QString gptdetail;
            // Ask before fixing anything: the fix rewrites the very header this
            // reads, so afterwards every image would look unaffected.
            GptRewriteRisk gptrisk = gptRewriteRisk(hRawDisk, sectorsize);
            // Only to word the message: with no GPT, say whether what was
            // written is an MBR image or has no table at all. Read here while
            // the handle is still open.
            bool mbr = deviceHasMbrTable(hRawDisk, sectorsize);
            if (fixGptCheckBox->isChecked() && status != STATUS_CANCELED)
            {
                statusbar->showMessage(tr("Fixing GPT..."));
                QCoreApplication::processEvents();
                gptfix = relocateBackupGPT(hRawDisk, sectorsize, availablesectors, &gptdetail);
                flushDevice(hRawDisk);
            }

            // Take the disk offline before releasing the locks so nothing is
            // remounted, then eject it. The card should be pulled without ever
            // being re-enumerated by Windows.
            bool offline = setDiskOffline(hRawDisk, true);
            bool ejected = ejectDevice(hRawDisk);
            CloseHandle(hRawDisk);
            hRawDisk = INVALID_HANDLE_VALUE;
            locked.release();

            if (imagetruncated && status != STATUS_CANCELED)
            {
                QMessageBox::critical(this, tr("Image truncated"),
                    tr("The image is larger than the device, so the end of it was not "
                       "written and the device does not hold a complete image.\n\n"
                       "This could only be detected once the device was full, because "
                       "the compressed image does not record its uncompressed size."));
                passfail = false;
            }
            else if (status == STATUS_CANCELED){
                passfail = false;
            }
            // No GPT means nothing for Windows to "repair", whether or not the
            // fix was asked for: the write already zeroed the first and last 34
            // sectors, so no stale backup GPT from an earlier image survives.
            // Reporting that as a risk, purely because the checkbox was off,
            // warned about a bug the image cannot have.
            else if (gptfix == GPT_FIX_OK || gptfix == GPT_FIX_NOT_NEEDED
                     || gptfix == GPT_FIX_NO_GPT
                     || (gptrisk == GPT_RISK_NO_GPT && gptfix == GPT_FIX_DISABLED))
            {
                QString msg;
                if (gptfix == GPT_FIX_OK)
                {
                    msg = tr("Write successful.\n\nThe GPT now matches the device (%1), so "
                             "Windows has nothing to repair. Remove the device normally.")
                              .arg(gptdetail);
                }
                else if (gptfix == GPT_FIX_NO_GPT || gptrisk == GPT_RISK_NO_GPT)
                {
                    msg = mbr
                        ? tr("Write successful.\n\nThis image uses an MBR partition table, not "
                             "a GPT, so the Windows GPT rewrite bug cannot affect it. Remove "
                             "the device normally.")
                        : tr("Write successful.\n\nThis image has no partition table, so the "
                             "Windows GPT rewrite bug cannot affect it. Remove the device "
                             "normally.");
                }
                else
                {
                    msg = tr("Write successful.");
                }
                QMessageBox::information(this, tr("Write Successful"), msg);
            }
            else
            {
                QString state = (offline || ejected)
                    ? tr("The device is offline and ejected.")
                    : tr("The device could NOT be taken offline.");
                QString why = (gptfix == GPT_FIX_BAD_GPT)
                    ? tr("The GPT could not be fixed (%1).").arg(
                          gptdetail.isEmpty() ? tr("malformed GPT") : gptdetail)
                    : (gptfix == GPT_FIX_FAILED)
                        ? tr("Fixing the GPT failed (%1).").arg(
                              gptdetail.isEmpty() ? tr("write error") : gptdetail)
                        : tr("\"Fix GPT after write\" is off.");
                QString risk;
                if (gptrisk == GPT_RISK_AFFECTED)
                {
                    risk = tr("This image IS affected: it reserves space ahead of its first "
                              "partition, so a rescan points the primary table at the wrong "
                              "sectors. Windows still accepts the result; Linux does not, and "
                              "the device will not boot.");
                }
                else if (gptrisk == GPT_RISK_SAFE)
                {
                    risk = tr("This image is NOT affected: a rescan still rewrites the table, "
                              "but for this layout it writes the correct values. Removing the "
                              "device now keeps it identical to the image either way.");
                }
                else
                {
                    risk = tr("Whether this image is affected could not be determined. Assume "
                              "it is: a rescan can leave a table that Linux rejects and the "
                              "device will not boot.");
                }
                QMessageBox::warning(this, tr("Remove the device now"),
                    tr("Write successful, but the partition table is at risk.\n\n"
                       "%1 %2\n\n"
                       "%3\n\n"
                       "Remove the device NOW and do not re-insert it here. Put it straight "
                       "into the target hardware.").arg(why).arg(state).arg(risk));
            }
        }
        else if (!fileinfo.exists() || !fileinfo.isFile())
        {
            QMessageBox::critical(this, tr("File Error"), tr("The selected file does not exist."));
            passfail = false;
        }
        else if (!fileinfo.isReadable())
        {
            QMessageBox::critical(this, tr("File Error"), tr("You do not have permission to read the selected file."));
            passfail = false;
        }
        else if (fileinfo.size() == 0)
        {
            QMessageBox::critical(this, tr("File Error"), tr("The specified file contains no data."));
            passfail = false;
        }
        showProgress(false);
        statusbar->showMessage(tr("Done."));
        bCancel->setEnabled(false);
        setReadWriteButtonState();
        if (passfail){
            statusbar->showMessage(tr("Write Successful."));
        }

    }
    else
    {
        QMessageBox::critical(this, tr("File Error"), tr("Please specify an image file to use."));
    }
    if (status == STATUS_EXIT)
    {
        close();
    }
    status = STATUS_IDLE;
    elapsed_timer->stop();
}

// The directory GetDiskFreeSpaceEx should be asked about: the one the image is
// being written into. The first three characters of the path used to stand in
// for it, which assumed a drive letter -- a UNC destination asked about "\\s"
// instead, and the check was skipped with an error nobody could act on.
//
// The directory is known to exist by the time this is called: the image file
// has already been created in it.
static QString volumeDirectoryFor(const QString &file)
{
    QString dir = QDir::toNativeSeparators(QFileInfo(file).absolutePath());
    // A UNC name is refused without a trailing backslash; a drive-letter path
    // accepts one either way, so both get one.
    if (!dir.endsWith(QChar('\\')))
    {
        dir += QChar('\\');
    }
    return dir;
}

void MainWindow::on_bRead_clicked()
{
    QString myFile;
    if (!leFile->text().isEmpty())
    {
        myFile = leFile->text();
        QFileInfo fileinfo(myFile);
        if (fileinfo.path()=="."){
            myFile = QDir::toNativeSeparators(QDir(myHomeDir).filePath(leFile->text()));
            // fileinfo has to follow, or the overwrite prompt below asks about
            // a file in the working directory while getHandleOnFile opens the
            // one in the image directory with CREATE_ALWAYS and truncates it
            // without ever asking.
            fileinfo.setFile(myFile);
        }
        // Reading writes a raw image; compressing on the way out is not
        // supported, and a raw image under a .gz or .xz name would mislead
        // every other tool that opens it.
        if (ImageSource::nameLooksCompressed(myFile))
        {
            QMessageBox::critical(this, tr("Read Error"),
                tr("Images can only be read back uncompressed. Choose a file name "
                   "without a .gz or .xz extension.\n\n"
                   "Compressed images (.img.gz, .img.xz) can be written and verified."));
            return;
        }
        // check whether source and target device is the same...
        if (fileIsOnSelectedDevice(myFile))
        {
            QMessageBox::critical(this, tr("Write Error"), tr("Image file cannot be located on the target device."));
            return;
        }
        int deviceID = selectedDeviceID();
        if (deviceID < 0)
        {
            QMessageBox::critical(this, tr("Read Error"), tr("Please select a source device."));
            return;
        }
        // confirm overwrite if the dest. file already exists
        if (fileinfo.exists())
        {
            if (QMessageBox::warning(this, tr("Confirm Overwrite"), tr("Are you sure you want to overwrite the specified file?"),
                                     QMessageBox::Yes|QMessageBox::No, QMessageBox::No) == QMessageBox::No)
            {
                return;
            }
        }
        bCancel->setEnabled(true);
        bWrite->setEnabled(false);
        bRead->setEnabled(false);
        bVerify->setEnabled(false);
        status = STATUS_READING;
        showProgress(true);
        unsigned long long i, lasti, numsectors, filesize, spaceneeded = 0ull;
        // Lock and dismount every volume on the source disk, so no filesystem
        // driver writes cached metadata into the middle of the image we read.
        // A disk with no mounted volume at all locks nothing and is fine.
        LockedVolumes locked;
        if (!locked.lockAll(deviceID))
        {
            endRun(tr("Read failed."));
            return;
        }
        // The device is opened first, and its size checked, before the image
        // file is touched at all: getHandleOnFile opens for writing with
        // CREATE_ALWAYS, which truncates. Opening it first would empty the
        // file the user already has and only then discover that the device
        // cannot be read, leaving them with neither.
        hRawDisk = getHandleOnDevice(deviceID, GENERIC_READ);
        if (hRawDisk == INVALID_HANDLE_VALUE)
        {
            locked.release();
            endRun(tr("Read failed."));
            return;
        }
        numsectors = getNumberOfSectors(hRawDisk, &sectorsize);
        if (!numsectors)
        {
            // A card reader whose card has been pulled stays present and
            // reports zero. Reading it would write a 0-byte image over
            // whatever was there.
            CloseHandle(hRawDisk);
            hRawDisk = INVALID_HANDLE_VALUE;
            locked.release();
            QMessageBox::critical(this, tr("Device Error"),
                tr("The device reports a size of zero. If it is a card reader, "
                   "the card may have been removed."));
            endRun(tr("Read failed."));
            return;
        }
        hFile = getHandleOnFile(LPCWSTR(myFile.data()), GENERIC_WRITE);
        if (hFile == INVALID_HANDLE_VALUE)
        {
            CloseHandle(hRawDisk);
            hRawDisk = INVALID_HANDLE_VALUE;
            locked.release();
            endRun(tr("Read failed."));
            return;
        }
        filesize = getFileSizeInSectors(hFile, sectorsize);
        if (filesize >= numsectors)
        {
            spaceneeded = 0ull;
        }
        else
        {
            spaceneeded = (unsigned long long)(numsectors - filesize) * (unsigned long long)(sectorsize);
        }
        if (!spaceAvailable(volumeDirectoryFor(myFile), spaceneeded))
        {
            QMessageBox::critical(this, tr("Write Error"), tr("Disk is not large enough for the specified image."));
            CloseHandle(hRawDisk);
            locked.release();
            CloseHandle(hFile);
            sectorData = NULL;
            hRawDisk = INVALID_HANDLE_VALUE;
            hFile = INVALID_HANDLE_VALUE;
            endRun(tr("Read failed."));
            return;
        }
        statusbar->showMessage(tr("Reading..."));
        const int progshift = beginProgress(numsectors, &lasti);
        for (i = 0ul; i < numsectors && status == STATUS_READING; i += TRANSFER_SECTORS)
        {
            sectorData = readSectorDataFromHandle(hRawDisk, i, (numsectors - i >= TRANSFER_SECTORS) ? TRANSFER_SECTORS:(numsectors - i), sectorsize);
            if (sectorData == NULL)
            {
                CloseHandle(hRawDisk);
                locked.release();
                CloseHandle(hFile);
                hRawDisk = INVALID_HANDLE_VALUE;
                hFile = INVALID_HANDLE_VALUE;
                endRun(tr("Read failed."));
                return;
            }
            if (!writeSectorDataToHandle(hFile, sectorData, i, (numsectors - i >= TRANSFER_SECTORS) ? TRANSFER_SECTORS:(numsectors - i), sectorsize))
            {
                delete[] sectorData;
                CloseHandle(hRawDisk);
                locked.release();
                CloseHandle(hFile);
                sectorData = NULL;
                hRawDisk = INVALID_HANDLE_VALUE;
                hFile = INVALID_HANDLE_VALUE;
                endRun(tr("Read failed."));
                return;
            }
            delete[] sectorData;
            sectorData = NULL;
            showThroughput(i, numsectors, &lasti);
            // i is where this chunk started; the bar tracks what is done.
            unsigned long long done = i + TRANSFER_SECTORS;
            progressbar->setValue((int)((done > numsectors ? numsectors : done) >> progshift));
            QCoreApplication::processEvents();
        }
        CloseHandle(hRawDisk);
        locked.release();
        CloseHandle(hFile);
        hRawDisk = INVALID_HANDLE_VALUE;
        hFile = INVALID_HANDLE_VALUE;
        showProgress(false);
        statusbar->showMessage(tr("Done."));
        bCancel->setEnabled(false);
        setReadWriteButtonState();
        if (status == STATUS_CANCELED){
            QMessageBox::information(this, tr("Complete"), tr("Read Canceled."));
        } else {
            QMessageBox::information(this, tr("Complete"), tr("Read Successful."));

        }
        updateHashControls();
    }
    else
    {
        QMessageBox::critical(this, tr("File Info"), tr("Please specify a file to save data to."));
    }
    if (status == STATUS_EXIT)
    {
        close();
    }
    status = STATUS_IDLE;
    elapsed_timer->stop();
}

// Verify image with device
void MainWindow::on_bVerify_clicked()
{
    bool passfail = true;
    bool verifyreported = false;
    if (!leFile->text().isEmpty())
    {
        QFileInfo fileinfo(leFile->text());
        if (fileinfo.exists() && fileinfo.isFile() &&
                fileinfo.isReadable() && (fileinfo.size() > 0) )
        {
            if (fileIsOnSelectedDevice(leFile->text()))
            {
                QMessageBox::critical(this, tr("Verify Error"), tr("Image file cannot be located on the target device."));
                return;
            }
            int deviceID = selectedDeviceID();
            if (deviceID < 0)
            {
                QMessageBox::critical(this, tr("Verify Error"), tr("Please select a device to verify against."));
                return;
            }
            status = STATUS_VERIFYING;
            showProgress(true);
            bCancel->setEnabled(true);
            bWrite->setEnabled(false);
            bRead->setEnabled(false);
            bVerify->setEnabled(false);
            unsigned long long i, lasti, availablesectors, numsectors, result;
            LockedVolumes locked;
            ImageSource image;
            if (!acquireDeviceAndImage(deviceID, locked, image, &availablesectors,
                                       tr("Verify Error"), tr("Verify failed.")))
            {
                return;
            }
            // gzip only records the uncompressed size modulo 4 GiB, so for any
            // real image it is a lower bound; the comparison then runs to the
            // device size and stops when the stream ends.
            const bool sizeisestimate = !image.sizeKnown();
            const unsigned long long imagesectors = image.sizeInSectors();
            numsectors = sizeisestimate ? availablesectors : imagesectors;
            if (!numsectors)
            {
                // The image, not the device: an empty file, or a compressed
                // one whose stream holds nothing. The card-reader comment
                // this used to carry belongs on the device-size check.
                QMessageBox::critical(this, tr("File Error"),
                                      tr("The specified file contains no data."));
                CloseHandle(hRawDisk);
                hRawDisk = INVALID_HANDLE_VALUE;
                locked.release();
                endRun(tr("Verify failed."));
                return;
            }
            // An estimated size is only a lower bound, but a lower bound that
            // already exceeds the device is enough to say the image will not
            // fit. Saying so here beats finding out at the end of the card,
            // which is the only other moment it can be detected.
            if (sizeisestimate && imagesectors > availablesectors)
            {
                QString msg = tr("The image is larger than the device:\n"
                                 "  Image: at least %1 sectors\n"
                                 "  Device: %2 sectors\n  Sector Size: %3\n\n"
                                 "Only the part that fits can be compared.\n\n"
                                 "Continue Anyway?");
                msg = msg.arg(imagesectors).arg(availablesectors).arg(sectorsize);
                if (QMessageBox::warning(this, tr("Size Mismatch!"), msg,
                                         QMessageBox::Ok, QMessageBox::Cancel) != QMessageBox::Ok)
                {
                    CloseHandle(hRawDisk);
                    hRawDisk = INVALID_HANDLE_VALUE;
                    locked.release();
                    endRun(tr("Verify failed."));
                    return;
                }
            }
            if (numsectors > availablesectors)
            {
                bool datafound = false;
                bool tailchecked = imageTailHasData(image, availablesectors,
                                                    numsectors, &datafound);
                QString msg = (!tailchecked)
                    ? tr("Size of image larger than device:\n  Image: %1 sectors\n"
                         "  Device: %2 sectors\n  Sector Size: %3\n\n"
                         "The extra space could not be checked for data, because the image "
                         "is compressed\n\nContinue Anyway?")
                    : (datafound)
                        ? tr("Size of image larger than device:\n  Image: %1 sectors\n"
                             "  Device: %2 sectors\n  Sector Size: %3\n\n"
                             "The extra space DOES appear to contain data\n\nContinue Anyway?")
                        : tr("Size of image larger than device:\n  Image: %1 sectors\n"
                             "  Device: %2 sectors\n  Sector Size: %3\n\n"
                             "The extra space does not appear to contain data\n\nContinue Anyway?");
                msg = msg.arg(numsectors).arg(availablesectors).arg(sectorsize);
                if(QMessageBox::warning(this, tr("Size Mismatch!"),
                                        msg, QMessageBox::Ok, QMessageBox::Cancel) == QMessageBox::Ok)
                {
                    // truncate the image at the device size...
                    numsectors = availablesectors;
                }
                else    // Cancel
                {
                    CloseHandle(hRawDisk);
                    hRawDisk = INVALID_HANDLE_VALUE;
                    locked.release();
                    endRun(tr("Verify cancelled."));
                    return;
                }
            }
            // "Fix GPT after write" deliberately rewrites the protective MBR,
            // the primary header and the entry array, all of which sit inside
            // the image's own range. Those sectors differing is expected, not a
            // bad card, so they are checked against the ranges the fix owns.
            unsigned long long gptfrontend = 0ull, gpttailstart = 0ull;
            bool gptknown = gptOwnedSectors(hRawDisk, sectorsize, availablesectors,
                                            &gptfrontend, &gpttailstart);
            // The fix also zeroes the stale backup GPT the image left mid-device,
            // which is in neither range above. Its location comes from the
            // image's own header, read out of the first chunk below.
            unsigned long long stalefirst = 0ull, stalelast = 0ull;
            bool staleknown = false;
            bool gptonly = false;
            bool gptrepaired = false, gptleftdamaged = false;
            GptPrimaryState gptstate = GPT_PRIMARY_UNKNOWN;

            const unsigned long long progresstotal = progressTotalFor(image, numsectors);
            const int progshift = beginProgress(progresstotal, &lasti);
            statusbar->showMessage(tr("Verifying..."));
            for (i = 0ul; i < numsectors && status == STATUS_VERIFYING; i += TRANSFER_SECTORS)
            {
                unsigned long long got = 0ull;
                sectorData = image.read(i, (numsectors - i >= TRANSFER_SECTORS) ? TRANSFER_SECTORS:(numsectors - i), &got);
                if (sectorData == NULL)
                {
                    QMessageBox::critical(this, tr("Verify Error"), image.errorString());
                    CloseHandle(hRawDisk);
                    hRawDisk = INVALID_HANDLE_VALUE;
                    locked.release();
                    endRun(tr("Verify failed."));
                    return;
                }
                if (got == 0ull)
                {
                    // The image ended on the previous chunk; there is nothing
                    // left to compare.
                    delete[] sectorData;
                    sectorData = NULL;
                    numsectors = i;
                    break;
                }
                sectorData2 = readSectorDataFromHandle(hRawDisk, i, got, sectorsize);
                if (sectorData2 == NULL)
                {
                    // The device could not be read. That is not the image
                    // failing to match, which is what the other message says.
                    QMessageBox::critical(this, tr("Verify Error"),
                        tr("The device could not be read at sector %1.").arg(i));
                    delete[] sectorData;
                    sectorData = NULL;
                    CloseHandle(hRawDisk);
                    hRawDisk = INVALID_HANDLE_VALUE;
                    locked.release();
                    endRun(tr("Verify failed."));
                    return;
                }
                if (i == 0ull && got >= 2ull)
                {
                    staleknown = gptImageBackupRange(
                        (const unsigned char *)(sectorData + sectorsize),
                        sectorsize, &stalefirst, &stalelast);
                }
                unsigned long chunk = (unsigned long)got;
                result = memcmp(sectorData, sectorData2, chunk * sectorsize);
                if (result)
                {
                    // Find the first difference the GPT fix cannot account for.
                    bool bad = false;
                    unsigned long long badsector = i;
                    for (unsigned long s = 0ul; s < chunk && !bad; ++s)
                    {
                        if (memcmp(sectorData + s * sectorsize,
                                   sectorData2 + s * sectorsize, sectorsize) == 0)
                        {
                            continue;
                        }
                        unsigned long long lba = i + s;
                        if (gptknown && (lba < gptfrontend || lba >= gpttailstart))
                        {
                            gptonly = true;
                            continue;
                        }
                        if (staleknown && lba >= stalefirst && lba <= stalelast)
                        {
                            gptonly = true;
                            continue;
                        }
                        badsector = lba;
                        bad = true;
                    }
                    if (bad)
                    {
                        QMessageBox::critical(this, tr("Verify Failure"),
                            tr("Verification failed at sector: %1").arg(badsector));
                        passfail = false;
                        break;
                    }
                }
                showThroughput(i, progresstotal, &lasti);
                delete[] sectorData;
                delete[] sectorData2;
                sectorData = NULL;
                sectorData2 = NULL;
                // i is where this chunk started; the bar tracks what is done.
                unsigned long long checked = i + TRANSFER_SECTORS;
                progressbar->setValue(
                    (int)((checked > progresstotal ? progresstotal : checked) >> progshift));
                QCoreApplication::processEvents();
            }
            // Same reasoning as the write path: without an exact size the loop
            // stops at the end of the device, and comparing only the part that
            // fits is not a successful verify.
            bool imageunchecked = false;
            if (!image.sizeKnown() && status == STATUS_VERIFYING && passfail)
            {
                unsigned long long leftover = 0ull;
                char *extra = image.read(numsectors, 1ull, &leftover);
                delete[] extra;
                imageunchecked = (leftover > 0ull);
            }
            // Every data sector can match the image while the partition table is
            // still ruined: Windows repairs a stranded backup GPT by itself and
            // gets the primary header wrong doing it, and the comparison above
            // forgives GPT sectors by design. So look at the table itself, while
            // the device is still held.
            if (status == STATUS_VERIFYING && passfail)
            {
                gptstate = gptPrimaryState(hRawDisk, sectorsize, availablesectors);
            }
            if (gptstate == GPT_PRIMARY_BROKEN)
            {
                if (offerGptRepair(hRawDisk, availablesectors,
                        tr("The device holds the image correctly, but its partition "
                           "table is broken:")))
                {
                    gptrepaired = true;
                }
                else
                {
                    gptleftdamaged = true;
                }
            }
            // Mirror the write path: take the disk offline and eject it before
            // the volume lock is released, so Windows cannot rescan the card
            // and "repair" a GPT that deliberately is not at the end of the
            // device. Verifying must not undo what the write protected.
            bool offline = setDiskOffline(hRawDisk, true);
            bool ejected = ejectDevice(hRawDisk);
            CloseHandle(hRawDisk);
            locked.release();
            image.close();
            delete[] sectorData;
            delete[] sectorData2;
            sectorData = NULL;
            sectorData2 = NULL;
            hRawDisk = INVALID_HANDLE_VALUE;
            if (status == STATUS_CANCELED){
                passfail = false;
            }
            else if (imageunchecked)
            {
                QMessageBox::critical(this, tr("Image larger than device"),
                    tr("The image is larger than the device, so only the part that fits "
                       "could be compared. Everything compared matched, but the device "
                       "does not hold a complete image.\n\n"
                       "This could only be detected at the end of the device, because "
                       "the compressed image does not record its uncompressed size."));
                passfail = false;
                verifyreported = true;
            }
            else if (gptleftdamaged)
            {
                QMessageBox::warning(this, tr("Partition table damaged"),
                    tr("The device holds the image correctly, but its partition table is "
                       "still broken. Write the image again with \"Fix GPT after write\" "
                       "ticked, or run the verify again and accept the repair."));
                verifyreported = true;
            }
            else if (passfail)
            {
                QString msg = tr("Verify Successful.");
                if (gptrepaired)
                {
                    msg = tr("Verify Successful.\n\nThe device's partition table was "
                             "damaged and has been repaired.");
                }
                else if (gptonly && gptstate == GPT_PRIMARY_OK)
                {
                    msg = tr("Verify Successful.\n\nThe image and the device differ only "
                             "in the GPT, and the GPT on the device is valid.");
                }
                else if (gptonly)
                {
                    // Not broken, but not confirmed good either -- no GPT to
                    // check, or one this cannot judge. Do not claim it is valid.
                    msg = tr("Verify Successful.\n\nThe image and the device differ only "
                             "in the GPT.");
                }
                msg += (offline || ejected)
                    ? tr("\n\nThe device has been ejected. Remove it now.")
                    : tr("\n\nThe device could NOT be taken offline automatically.");
                QMessageBox::information(this, tr("Complete"), msg);
                verifyreported = true;
            }

        }
        else if (!fileinfo.exists() || !fileinfo.isFile())
        {
            QMessageBox::critical(this, tr("File Error"), tr("The selected file does not exist."));
            passfail = false;
        }
        else if (!fileinfo.isReadable())
        {
            QMessageBox::critical(this, tr("File Error"), tr("You do not have permission to read the selected file."));
            passfail = false;
        }
        else if (fileinfo.size() == 0)
        {
            QMessageBox::critical(this, tr("File Error"), tr("The specified file contains no data."));
            passfail = false;
        }
        showProgress(false);
        statusbar->showMessage(tr("Done."));
        bCancel->setEnabled(false);
        setReadWriteButtonState();
        if (passfail && !verifyreported){
            QMessageBox::information(this, tr("Complete"), tr("Verify Successful."));
        }
    }
    else
    {
        QMessageBox::critical(this, tr("File Error"), tr("Please specify an image file to use."));
    }
    if (status == STATUS_EXIT)
    {
        close();
    }
    status = STATUS_IDLE;
    elapsed_timer->stop();
}

static QString formatDeviceSize(unsigned long long bytes)
{
    // Card and stick capacities are quoted in powers of ten, so match that.
    static const char *units[] = { "KB", "MB", "GB", "TB" };
    double value = (double)bytes;
    int unit = -1;
    while (value >= 1000.0 && unit < 3)
    {
        value /= 1000.0;
        ++unit;
    }
    if (unit < 0)
    {
        return QString("%1 B").arg(bytes);
    }
    return QString("%1 %2").arg(value, 0, 'f', (value < 10.0) ? 1 : 0).arg(units[unit]);
}

// How long the interface is left running before a scan started by clicking
// something blocks it. Long enough for the control to finish drawing itself;
// see on_showAllDevicesCheckBox_toggled().
static const int SCAN_SETTLE_MS = 250;

// Rescan behind a status message. Every rescan anyone waits on comes through
// here: the one after the window appears, the one the device list runs as it
// is opened, and the one that follows "Show all devices" being ticked or
// unticked.
//
// The message lasts exactly as long as the scan. A scan of disks that are
// already awake finishes in milliseconds and the message is gone again before
// it can be read, which is the point: there was nothing to wait for.
void MainWindow::rescanDevices()
{
    // Never while something is running. getLogicalDrives() rebuilds the list
    // and finishes by putting the buttons back the way an idle window has
    // them, which in the middle of a write would re-enable the very buttons
    // the write disabled -- and the device list stays clickable throughout a
    // run, so this is reachable. The two-second poll this replaced took the
    // same precaution, and the device-arrival broadcast still does.
    if (status != STATUS_IDLE)
    {
        return;
    }
    statusbar->showMessage(tr("Scanning disks..."));
    // A wait cursor as well as the message. A scan that has to spin a disk up
    // takes twelve seconds, and for every one of them the window is frozen --
    // it does not repaint and does not answer. The message says what is
    // happening; the cursor says it where the pointer already is, over the
    // control that was just clicked.
    QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));

    // Painted before the scan rather than left in the queue behind it. This
    // flushes the message, the cursor, and the repaint still pending on
    // whatever was clicked to get here -- getLogicalDrives() blocks this
    // thread, so anything merely posted would not reach the screen until the
    // wait it explains was already over, and a checkbox would sit there drawn
    // in the state it was clicked in looking like a hung program.
    QCoreApplication::processEvents(QEventLoop::ExcludeUserInputEvents);

    getLogicalDrives();
    QApplication::restoreOverrideCursor();
    // Emptied rather than replaced with something. The status bar says what
    // is happening or what just happened; with nothing running it has nothing
    // to say, and shadeStatusBar() gives it an edge so that an empty one still
    // reads as part of the window rather than as a gap in it.
    statusbar->clearMessage();
}

// getLogicalDrives fills cboxDevice from the physical disks attached to the
// machine. Enumerating physical disks rather than drive letters is what lets a
// card with no Windows-readable filesystem — a written Linux image, or a card
// Windows has not mounted for any other reason — still appear in the list.
void MainWindow::getLogicalDrives()
{
    // Keep the user's selection across a refresh, since device arrival and the
    // "show all devices" toggle both rebuild the list underneath them.
    int previous = selectedDeviceID();

    QList<PhysicalDevice> devices = enumeratePhysicalDevices(showAllDevicesCheckBox->isChecked());

    // Rebuilding the list closes an open dropdown and resets the selection, so
    // the poll below only goes through with it when something actually changed.
    QString signature;
    for (int i = 0; i < devices.size(); ++i)
    {
        signature += QString("%1:%2:%3;").arg(devices.at(i).deviceNumber)
                         .arg(devices.at(i).letters).arg(devices.at(i).sizeBytes);
    }
    if (signature == deviceSignature && cboxDevice->count() == devices.size())
    {
        return;
    }
    deviceSignature = signature;

    cboxDevice->clear();
    for (int i = 0; i < devices.size(); ++i)
    {
        const PhysicalDevice &dev = devices.at(i);
        QString label = dev.letters.isEmpty()
                ? tr("[Disk %1]").arg(dev.deviceNumber)
                : QString("[%1]").arg(dev.letters);
        cboxDevice->addItem(QString("%1 %2 - %3").arg(label)
                                .arg(formatDeviceSize(dev.sizeBytes))
                                .arg(dev.description),
                            (qulonglong)dev.deviceNumber);
    }

    // The popup is otherwise as narrow as the closed box, which elides the very
    // names it exists to tell apart.
    QFontMetrics metrics(cboxDevice->view()->font());
    int widest = 0;
    for (int i = 0; i < cboxDevice->count(); ++i)
    {
        widest = qMax(widest, metrics.horizontalAdvance(cboxDevice->itemText(i)));
    }
    cboxDevice->view()->setMinimumWidth(widest + 2 * cboxDevice->view()->frameWidth()
                                        + QApplication::style()->pixelMetric(QStyle::PM_ScrollBarExtent));

    int restore = (previous >= 0) ? cboxDevice->findData((qulonglong)previous) : -1;
    cboxDevice->setCurrentIndex((restore >= 0) ? restore : 0);
    setReadWriteButtonState();
}

// Physical disk number of the device the user picked, or -1 if the list is empty.
int MainWindow::selectedDeviceID()
{
    if (cboxDevice->currentIndex() < 0)
    {
        return -1;
    }
    bool ok = false;
    int id = cboxDevice->currentData().toInt(&ok);
    return ok ? id : -1;
}

// True if the image file sits on a volume of the target disk, which would mean
// reading or writing it over itself.
bool MainWindow::fileIsOnSelectedDevice(const QString &file)
{
    int deviceID = selectedDeviceID();
    if (deviceID < 0 || file.length() < 2 || file.at(1) != QChar(':'))
    {
        return false;
    }
    QString letters = driveLettersOnDevice((ULONG)deviceID);
    return letters.contains(QString("%1:").arg(file.at(0).toUpper()));
}

void MainWindow::on_showAllDevicesCheckBox_toggled(bool)
{
    // Through rescanDevices(), not straight to getLogicalDrives(). Ticking
    // this box is what asks for the fixed disks, which are the ones that may
    // be spun down -- so of the three places a scan starts, this is the one
    // most likely to take twelve seconds, and it was the one that showed
    // nothing at all while it did.
    //
    // Deferred, and deliberately not by zero. The tick is animated by the
    // Windows style, and an animation advances only while the event loop is
    // running: a single pass of processEvents draws its first frame, which is
    // an empty box. Scanning straight from this slot therefore left the
    // checkbox drawn unticked for the whole wait, next to a status bar saying
    // it was scanning -- which reads as the click having been ignored, and was
    // measured doing exactly that four seconds into a stalled scan. Letting
    // the loop run first costs a quarter second against a wait of twelve.
    QTimer::singleShot(SCAN_SETTLE_MS, this, [this]() { rescanDevices(); });
}

// register to receive notifications when USB devices are inserted or removed
// adapted from http://www.known-issues.net/qt/qt-detect-event-windows.html
bool MainWindow::nativeEvent(const QByteArray &type, void *vMsg, qintptr *result)
{
    Q_UNUSED(type);
    MSG *msg = (MSG*)vMsg;
    if(msg->message == WM_DEVICECHANGE)
    {
        switch(msg->wParam)
        {
        case DBT_DEVICEARRIVAL:
        case DBT_DEVICEREMOVECOMPLETE:
            // The list is rebuilt wholesale rather than patched by drive
            // letter: a device worth listing need not carry a letter at all,
            // and the broadcast only names one when it does.
            if (status == STATUS_IDLE)
            {
                getLogicalDrives();
            }
            break;
        } // skip the rest
    } // end of if msg->message
    *result = 0; //get rid of obnoxious compiler warning
    return false; // let qt handle the rest
}

void MainWindow::updateHashControls()
{
    QFileInfo fileinfo(leFile->text());
    bool validFile = (fileinfo.exists() && fileinfo.isFile() &&
                      fileinfo.isReadable() && (fileinfo.size() >0));

    bHashCopy->setEnabled(false);
    hashLabel->clear();
    // An empty hash line is a blank row; let the group close up until there is
    // something to show. This one does not retain its space: the point is the
    // height it gives back.
    hashLabel->setVisible(false);

    if (cboxHashType->currentIndex() != 0 && !leFile->text().isEmpty() && validFile)
    {
            bHashGen->setEnabled(true);
    }
    else
    {
        bHashGen->setEnabled(false);
    }

    // Copy stays disabled: this function has just cleared the label, so there
    // is nothing to copy. generateHash() enables it once a digest exists.
}

// A different image file has been named -- typed, browsed to, dropped, or given
// on the command line. Everything that depends on which file it is is settled
// here, so a fourth way of naming one cannot end up doing two thirds of it.
void MainWindow::imageFileChanged()
{
    defaultHashTypeForFile();
    setReadWriteButtonState();
    updateHashControls();
}

void MainWindow::on_cboxHashType_IdxChg()
{
    updateHashControls();
}

void MainWindow::on_bHashGen_clicked()
{
    generateHash(leFile->text(), cboxHashType->currentData().toInt());

}
