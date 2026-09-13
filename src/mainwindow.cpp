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

// Qt turns word wrap on for a tooltip only when the text looks like rich text
// (Qt::mightBeRichText). A long plain tooltip therefore becomes one enormous
// line, which Qt then clamps against the edges of the screen: the beginning and
// the end are both cut off. Wrapping the text in a table cell of a fixed width
// turns wrapping on and gives it a column to wrap into.
//
// Applied here rather than in the .ui so the translated strings are wrapped
// too -- a translation is often longer than the English -- and so the strings
// the translators work from stay free of markup.
static void wrapLongToolTips(QWidget *root)
{
    const int wrapAboveChars = 60;
    const int wrapWidthPx = 360;

    const QList<QWidget *> widgets = root->findChildren<QWidget *>();
    for (QWidget *w : widgets)
    {
        const QString tip = w->toolTip();
        // Short tips read better left on one line, and anything already marked
        // up is the author's business.
        if (tip.length() <= wrapAboveChars || Qt::mightBeRichText(tip))
        {
            continue;
        }
        w->setToolTip(QString("<table><tr><td width=\"%1\">%2</td></tr></table>")
                          .arg(wrapWidthPx)
                          .arg(tip.toHtmlEscaped()));
    }
}

// Qt keeps one tooltip label alive and resizes it for the next tooltip instead
// of building a fresh one, and that resize does not always come out right:
// going straight from one tooltip to a wider one leaves the label at the old
// width, with the text clipped at both ends. Let the first tooltip disappear on
// its own first and the next one is correct, because it is then built from
// scratch.
//
// QToolTip::hideText() does not help: it only schedules the hide, so the label
// is still visible when the next tooltip is shown and gets reused anyway -- and
// the hide it scheduled then takes that new tooltip away a second later.
// Hiding the widget is what makes Qt build a new one.
//
// Qt's own class, found by name because it is not public. If that name ever
// changes nothing matches, and tooltips behave as they would without this.
static void dropShowingToolTip()
{
    const QWidgetList tops = QApplication::topLevelWidgets();
    for (QWidget *w : tops)
    {
        if (w->isVisible() && qstrcmp(w->metaObject()->className(), "QTipLabel") == 0)
        {
            w->hide();
        }
    }
}

// Only when the pointer reaches a different widget: that is the only time the
// tooltip has to change size. Qt sends this event repeatedly while the pointer
// moves within one widget, and acting on every one of those would throw away
// the tooltip that is already up and correct.
bool MainWindow::eventFilter(QObject *watched, QEvent *event)
{
    if (event->type() == QEvent::ToolTip && watched != myLastToolTipTarget)
    {
        myLastToolTipTarget = watched;
        dropShowingToolTip();
    }
    return QMainWindow::eventFilter(watched, event);
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

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
{
    setupUi(this);
    wrapLongToolTips(this);
    // Application-wide: the filter has to see the event before the widget that
    // is about to show its own tooltip does.
    qApp->installEventFilter(this);
    elapsed_timer = new ElapsedTimer();
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
    statusbar->showMessage(tr("Waiting for a task."));
    hFile = INVALID_HANDLE_VALUE;
    hRawDisk = INVALID_HANDLE_VALUE;
    if (QCoreApplication::arguments().count() > 1)
    {
        QString fileLocation = QApplication::arguments().at(1);
        QFileInfo fileInfo(fileLocation);
        leFile->setText(fileInfo.absoluteFilePath());
    }
    // Add supported hash types.
    cboxHashType->addItem("MD5",QVariant(QCryptographicHash::Md5));
    cboxHashType->addItem("SHA1",QVariant(QCryptographicHash::Sha1));
    cboxHashType->addItem("SHA256",QVariant(QCryptographicHash::Sha256));
    connect(this->cboxHashType, SIGNAL(currentIndexChanged(int)), SLOT(on_cboxHashType_IdxChg()));
    updateHashControls();
    setReadWriteButtonState();
    sectorData = NULL;
    sectorData2 = NULL;
    sectorsize = 0ul;

    loadSettings();
    // After loadSettings, which sets the "show all devices" state the filter reads.
    getLogicalDrives();
    // Inserting a card into a reader that presents no volume produces no
    // WM_DEVICECHANGE broadcast, so the list is also polled. getLogicalDrives
    // only touches the combo box when the set of devices actually changed.
    device_poll_timer = new QTimer(this);
    connect(device_poll_timer, &QTimer::timeout, this, [this]() {
        if (status == STATUS_IDLE)
        {
            getLogicalDrives();
        }
    });
    device_poll_timer->start(2000);

    if (myHomeDir.isEmpty()){
        initializeHomeDir();
    }

    if (myFileType.isEmpty()) {
        myFileType = tr("Disk Images (*.img *.IMG *.img.gz *.img.xz)");
    }
    myFileTypeList << tr("Disk Images (*.img *.IMG *.img.gz *.img.xz)")
                   << tr("Compressed Disk Images (*.img.gz *.img.xz *.gz *.xz)")
                   << "*.*";
}

MainWindow::~MainWindow()
{
    saveSettings();
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


void MainWindow::saveSettings()
{
    QSettings userSettings("HKEY_CURRENT_USER\\Software\\Win32DiskImager", QSettings::NativeFormat);
    userSettings.beginGroup("Settings");
    userSettings.setValue("ImageDir", myHomeDir);
    userSettings.setValue("FileType", myFileType);
    userSettings.setValue("FixGpt", fixGptCheckBox->isChecked());
    userSettings.endGroup();
}

void MainWindow::loadSettings()
{
    QSettings userSettings("HKEY_CURRENT_USER\\Software\\Win32DiskImager", QSettings::NativeFormat);
    userSettings.beginGroup("Settings");
    myHomeDir = userSettings.value("ImageDir").toString();
    myFileType = userSettings.value("FileType").toString();
    fixGptCheckBox->setChecked(userSettings.value("FixGpt", true).toBool());
    // Deliberately not persisted: the app starts with only removable devices
    // listed, so a fixed disk is never preselected from a previous session.
    showAllDevicesCheckBox->setChecked(false);
}

void MainWindow::initializeHomeDir()
{
    myHomeDir = QDir::homePath();
    if (myHomeDir.isNull()){
        myHomeDir = qgetenv("USERPROFILE");
    }
    /* Get Downloads the Windows way */
    QString downloadPath = qgetenv("DiskImagesDir");
    if (downloadPath.isEmpty()) {
        PWSTR pPath = NULL;
        static GUID downloads = {0x374de290, 0x123f, 0x4565, {0x91, 0x64, 0x39,
                                 0xc4, 0x92, 0x5e, 0x46, 0x7b}};
        if (SHGetKnownFolderPath(downloads, 0, 0, &pPath) == S_OK) {
            downloadPath = QDir::fromNativeSeparators(QString::fromWCharArray(pPath));
            LocalFree(pPath);
            if (downloadPath.isEmpty() || !QDir(downloadPath).exists()) {
                downloadPath = QStandardPaths::writableLocation(QStandardPaths::DownloadLocation);
            }
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
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    saveSettings();
    if (status == STATUS_READING)
    {
        if (QMessageBox::warning(this, tr("Exit?"), tr("Exiting now will result in a corrupt image file.\n"
                                                       "Are you sure you want to exit?"),
                                 QMessageBox::Yes|QMessageBox::No, QMessageBox::No) == QMessageBox::Yes)
        {
            status = STATUS_EXIT;
        }
        event->ignore();
    }
    else if (status == STATUS_WRITING)
    {
        if (QMessageBox::warning(this, tr("Exit?"), tr("Exiting now will result in a corrupt disk.\n"
                                                       "Are you sure you want to exit?"),
                                 QMessageBox::Yes|QMessageBox::No, QMessageBox::No) == QMessageBox::Yes)
        {
            status = STATUS_EXIT;
        }
        event->ignore();
    }
    else if (status == STATUS_VERIFYING)
    {
        if (QMessageBox::warning(this, tr("Exit?"), tr("Exiting now will cancel verifying image.\n"
                                                       "Are you sure you want to exit?"),
                                 QMessageBox::Yes|QMessageBox::No, QMessageBox::No) == QMessageBox::Yes)
        {
            status = STATUS_EXIT;
        }
        event->ignore();
    }
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
            leFile->setText(fileLocation);
            QFileInfo newFileInfo(fileLocation);
            myHomeDir = newFileInfo.absolutePath();
        }
        setReadWriteButtonState();
        updateHashControls();
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
    hashLabel->setVisible(true);
        bHashCopy->setEnabled(false);
        QApplication::restoreOverrideCursor();
        QMessageBox::critical(this, tr("File Error"),
                              tr("Could not open the file to generate a checksum:\n%1").arg(file.errorString()));
        return;
    }
    filehash.addData(&file);

    QByteArray hash = filehash.result();

    // display it in the textbox
    hashLabel->setText(hash.toHex());
    hashLabel->setVisible(true);
    bHashCopy->setEnabled(true);
    // redisplay the normal cursor
    QApplication::restoreOverrideCursor();
}


// on an "editingFinished" signal (IE: return press), if the lineedit
// contains a valid file, update the controls
void MainWindow::on_leFile_editingFinished()
{
    setReadWriteButtonState();
    updateHashControls();
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
            double mbpersec;
            unsigned long long i, lasti, availablesectors, numsectors;
            // Lock and dismount every volume on the target disk. Leaving the
            // other partitions mounted lets their filesystem drivers flush
            // cached metadata over the image while it is being written.
            LockedVolumes locked;
            if (!locked.lockAll(deviceID))
            {
                status = STATUS_IDLE;
                showProgress(false);
                statusbar->showMessage(tr("Write failed."));
                bCancel->setEnabled(false);
                setReadWriteButtonState();
                return;
            }
            // The device is opened first: the image reader needs the sector
            // size to hand out whole sectors, compressed or not.
            // Read access is needed as well: the GPT fix reads the table back.
            hRawDisk = getHandleOnDevice(deviceID, GENERIC_READ | GENERIC_WRITE);
            if (hRawDisk == INVALID_HANDLE_VALUE)
            {
                locked.release();
                status = STATUS_IDLE;
                showProgress(false);
                statusbar->showMessage(tr("Write failed."));
                bCancel->setEnabled(false);
                setReadWriteButtonState();
                return;
            }
            availablesectors = getNumberOfSectors(hRawDisk, &sectorsize);
            if (!availablesectors)
            {
                //For external card readers you may not get device change notification when you remove the card/flash.
                //(So no WM_DEVICECHANGE signal). Device stays but size goes to 0. [Is there special event for this on Windows??]
                QMessageBox::critical(this, tr("Device Error"),
                    tr("The device reports a size of zero. If it is a card reader, "
                       "the card may have been removed."));
                locked.release();
                CloseHandle(hRawDisk);
                hRawDisk = INVALID_HANDLE_VALUE;
                passfail = false;
                status = STATUS_IDLE;
                showProgress(false);
                statusbar->showMessage(tr("Write failed."));
                bCancel->setEnabled(false);
                setReadWriteButtonState();
                return;
            }
            // A .img.gz or .img.xz is decompressed on the fly as it is written,
            // so the machine never needs room for the expanded image.
            ImageSource image;
            if (!image.open(leFile->text(), sectorsize))
            {
                QMessageBox::critical(this, tr("Write Error"), image.errorString());
                locked.release();
                CloseHandle(hRawDisk);
                hRawDisk = INVALID_HANDLE_VALUE;
                status = STATUS_IDLE;
                showProgress(false);
                statusbar->showMessage(tr("Write failed."));
                bCancel->setEnabled(false);
                setReadWriteButtonState();
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
                //For external card readers you may not get device change notification when you remove the card/flash.
                //(So no WM_DEVICECHANGE signal). Device stays but size goes to 0. [Is there special event for this on Windows??]
                QMessageBox::critical(this, tr("File Error"),
                                      tr("The specified file contains no data."));
                locked.release();
                CloseHandle(hRawDisk);
                hRawDisk = INVALID_HANDLE_VALUE;
                status = STATUS_IDLE;
                showProgress(false);
                statusbar->showMessage(tr("Write failed."));
                bCancel->setEnabled(false);
                setReadWriteButtonState();
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
                    locked.release();
                    CloseHandle(hRawDisk);
                    status = STATUS_IDLE;
                    hRawDisk = INVALID_HANDLE_VALUE;
                    showProgress(false);
                    statusbar->showMessage(tr("Write failed."));
                    bCancel->setEnabled(false);
                    setReadWriteButtonState();
                    return;
                }
            }
            if (numsectors > availablesectors)
            {
                // A compressed image is read forwards only: scanning its tail
                // would mean decompressing the whole image just to decide
                // whether to start, so the tail is reported as unexamined.
                bool tailchecked = !image.isCompressed();
                bool datafound = false;
                if (tailchecked)
                {
                    i = availablesectors;
                    unsigned long nextchunksize = 0;
                    while ( (i < numsectors) && (datafound == false) )
                    {
                        nextchunksize = ((numsectors - i) >= 1024ul) ? 1024ul : (numsectors - i);
                        sectorData = image.read(i, nextchunksize, NULL);
                        if(sectorData == NULL)
                        {
                            // if there's an error verifying the truncated data, just move on to the
                            //  write, as we don't care about an error in a section that we're not writing...
                            i = numsectors + 1;
                        } else {
                            unsigned int j = 0;
                            unsigned limit = nextchunksize * sectorsize;
                            while ( (datafound == false) && ( j < limit ) )
                            {
                                if(sectorData[j++] != 0)
                                {
                                    datafound = true;
                                }
                            }
                            i += nextchunksize;
                        }
                        // delete the allocated sectorData
                        delete[] sectorData;
                        sectorData = NULL;
                    }
                }
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
                    locked.release();
                    CloseHandle(hRawDisk);
                    status = STATUS_IDLE;
                    hRawDisk = INVALID_HANDLE_VALUE;
                    showProgress(false);
                    statusbar->showMessage(tr("Write cancelled."));
                    bCancel->setEnabled(false);
                    setReadWriteButtonState();
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
                locked.release();
                CloseHandle(hRawDisk);
                status = STATUS_IDLE;
                hRawDisk = INVALID_HANDLE_VALUE;
                showProgress(false);
                statusbar->showMessage(tr("Write failed."));
                bCancel->setEnabled(false);
                setReadWriteButtonState();
                return;
            }

            // The loop runs to the device size when the size is only an
            // estimate, but the bar tracks the estimate: it then describes the
            // image rather than the card.
            unsigned long long progresstotal = numsectors;
            if (!image.sizeKnown() && image.sizeInSectors() > 0ull
                && image.sizeInSectors() < progresstotal)
            {
                progresstotal = image.sizeInSectors();
            }
            const int progshift = progressShift(progresstotal);
            progressbar->setRange(0, (progresstotal == 0ull) ? 100
                                                             : (int)(progresstotal >> progshift));
            lasti = 0ul;
            update_timer.start();
            elapsed_timer->start();
            bool imagetruncated = false;
            for (i = 0ul; i < numsectors && status == STATUS_WRITING; i += 1024ul)
            {
                unsigned long long chunk = (numsectors - i >= 1024ul) ? 1024ul : (numsectors - i);
                unsigned long long got = 0ull;
                sectorData = image.read(i, chunk, &got);
                if (sectorData == NULL)
                {
                    QMessageBox::critical(this, tr("Write Error"),
                        image.errorString()
                        + "\n\n" + tr("The device has been partially written and no longer holds "
                                      "a usable image. Write the image again before using it."));
                    locked.release();
                    CloseHandle(hRawDisk);
                    status = STATUS_IDLE;
                    hRawDisk = INVALID_HANDLE_VALUE;
                    showProgress(false);
                    statusbar->showMessage(tr("Write failed."));
                    bCancel->setEnabled(false);
                    setReadWriteButtonState();
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
                    locked.release();
                    CloseHandle(hRawDisk);
                    status = STATUS_IDLE;
                    sectorData = NULL;
                    hRawDisk = INVALID_HANDLE_VALUE;
                    showProgress(false);
                    statusbar->showMessage(tr("Write failed."));
                    bCancel->setEnabled(false);
                    setReadWriteButtonState();
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
                if (update_timer.elapsed() >= ONE_SEC_IN_MS)
                {
                    mbpersec = (((double)sectorsize * (i - lasti)) * ((float)ONE_SEC_IN_MS / update_timer.elapsed())) / 1024.0 / 1024.0;
                    statusbar->showMessage(QString("%1 MB/s").arg(mbpersec));
                    elapsed_timer->update(i, progresstotal);
                    update_timer.start();
                    lasti = i;
                }
                progressbar->setValue((int)((i > progresstotal ? progresstotal : i) >> progshift));
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

void MainWindow::on_bRead_clicked()
{
    QString myFile;
    if (!leFile->text().isEmpty())
    {
        myFile = leFile->text();
        QFileInfo fileinfo(myFile);
        if (fileinfo.path()=="."){
            myFile=(myHomeDir + "/" + leFile->text());
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
        double mbpersec;
        unsigned long long i, lasti, numsectors, filesize, spaceneeded = 0ull;
        // Lock and dismount every volume on the source disk, so no filesystem
        // driver writes cached metadata into the middle of the image we read.
        // A disk with no mounted volume at all locks nothing and is fine.
        LockedVolumes locked;
        if (!locked.lockAll(deviceID))
        {
            status = STATUS_IDLE;
            showProgress(false);
            statusbar->showMessage(tr("Read failed."));
            bCancel->setEnabled(false);
            setReadWriteButtonState();
            return;
        }
        hFile = getHandleOnFile(LPCWSTR(myFile.data()), GENERIC_WRITE);
        if (hFile == INVALID_HANDLE_VALUE)
        {
            locked.release();
            status = STATUS_IDLE;
            showProgress(false);
            statusbar->showMessage(tr("Read failed."));
            bCancel->setEnabled(false);
            setReadWriteButtonState();
            return;
        }
        hRawDisk = getHandleOnDevice(deviceID, GENERIC_READ);
        if (hRawDisk == INVALID_HANDLE_VALUE)
        {
            locked.release();
            CloseHandle(hFile);
            status = STATUS_IDLE;
            hFile = INVALID_HANDLE_VALUE;
            showProgress(false);
            statusbar->showMessage(tr("Read failed."));
            bCancel->setEnabled(false);
            setReadWriteButtonState();
            return;
        }
        numsectors = getNumberOfSectors(hRawDisk, &sectorsize);
        filesize = getFileSizeInSectors(hFile, sectorsize);
        if (filesize >= numsectors)
        {
            spaceneeded = 0ull;
        }
        else
        {
            spaceneeded = (unsigned long long)(numsectors - filesize) * (unsigned long long)(sectorsize);
        }
        if (!spaceAvailable(myFile.left(3).replace(QChar('/'), QChar('\\')).toLatin1().data(), spaceneeded))
        {
            QMessageBox::critical(this, tr("Write Error"), tr("Disk is not large enough for the specified image."));
            locked.release();
            CloseHandle(hRawDisk);
            CloseHandle(hFile);
            status = STATUS_IDLE;
            sectorData = NULL;
            hRawDisk = INVALID_HANDLE_VALUE;
            hFile = INVALID_HANDLE_VALUE;
            showProgress(false);
            statusbar->showMessage(tr("Read failed."));
            bCancel->setEnabled(false);
            setReadWriteButtonState();
            return;
        }
        const int progshift = progressShift(numsectors);
        if (numsectors == 0ul)
        {
            progressbar->setRange(0, 100);
        }
        else
        {
            progressbar->setRange(0, (int)(numsectors >> progshift));
        }
        lasti = 0ul;
        update_timer.start();
        elapsed_timer->start();
        for (i = 0ul; i < numsectors && status == STATUS_READING; i += 1024ul)
        {
            sectorData = readSectorDataFromHandle(hRawDisk, i, (numsectors - i >= 1024ul) ? 1024ul:(numsectors - i), sectorsize);
            if (sectorData == NULL)
            {
                locked.release();
                CloseHandle(hRawDisk);
                CloseHandle(hFile);
                status = STATUS_IDLE;
                hRawDisk = INVALID_HANDLE_VALUE;
                hFile = INVALID_HANDLE_VALUE;
                showProgress(false);
                statusbar->showMessage(tr("Read failed."));
                bCancel->setEnabled(false);
                setReadWriteButtonState();
                return;
            }
            if (!writeSectorDataToHandle(hFile, sectorData, i, (numsectors - i >= 1024ul) ? 1024ul:(numsectors - i), sectorsize))
            {
                delete[] sectorData;
                locked.release();
                CloseHandle(hRawDisk);
                CloseHandle(hFile);
                status = STATUS_IDLE;
                sectorData = NULL;
                hRawDisk = INVALID_HANDLE_VALUE;
                hFile = INVALID_HANDLE_VALUE;
                showProgress(false);
                statusbar->showMessage(tr("Read failed."));
                bCancel->setEnabled(false);
                setReadWriteButtonState();
                return;
            }
            delete[] sectorData;
            sectorData = NULL;
            if (update_timer.elapsed() >= ONE_SEC_IN_MS)
            {
                mbpersec = (((double)sectorsize * (i - lasti)) * ((float)ONE_SEC_IN_MS / update_timer.elapsed())) / 1024.0 / 1024.0;
                statusbar->showMessage(QString("%1MB/s").arg(mbpersec));
                update_timer.start();
                elapsed_timer->update(i, numsectors);
                lasti = i;
            }
            progressbar->setValue((int)(i >> progshift));
            QCoreApplication::processEvents();
        }
        locked.release();
        CloseHandle(hRawDisk);
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
            double mbpersec;
            unsigned long long i, lasti, availablesectors, numsectors, result;
            // Lock and dismount every volume on the disk being verified, so
            // nothing writes to it while it is compared against the image.
            LockedVolumes locked;
            if (!locked.lockAll(deviceID))
            {
                status = STATUS_IDLE;
                showProgress(false);
                statusbar->showMessage(tr("Verify failed."));
                bCancel->setEnabled(false);
                setReadWriteButtonState();
                return;
            }
            // The device is opened first: the image reader needs the sector
            // size to hand out whole sectors, compressed or not.
            // Read-write: verify never writes, but taking the disk offline
            // afterwards (IOCTL_DISK_SET_DISK_ATTRIBUTES) needs write access.
            hRawDisk = getHandleOnDevice(deviceID, GENERIC_READ | GENERIC_WRITE);
            if (hRawDisk == INVALID_HANDLE_VALUE)
            {
                locked.release();
                status = STATUS_IDLE;
                showProgress(false);
                statusbar->showMessage(tr("Verify failed."));
                bCancel->setEnabled(false);
                setReadWriteButtonState();
                return;
            }
            availablesectors = getNumberOfSectors(hRawDisk, &sectorsize);
            if (!availablesectors)
            {
                //For external card readers you may not get device change notification when you remove the card/flash.
                //(So no WM_DEVICECHANGE signal). Device stays but size goes to 0. [Is there special event for this on Windows??]
                QMessageBox::critical(this, tr("Device Error"),
                    tr("The device reports a size of zero. If it is a card reader, "
                       "the card may have been removed."));
                locked.release();
                CloseHandle(hRawDisk);
                hRawDisk = INVALID_HANDLE_VALUE;
                passfail = false;
                status = STATUS_IDLE;
                showProgress(false);
                statusbar->showMessage(tr("Verify failed."));
                bCancel->setEnabled(false);
                setReadWriteButtonState();
                return;
            }
            // A compressed image is decompressed on the fly and compared as it
            // comes out, exactly like a raw one.
            ImageSource image;
            if (!image.open(leFile->text(), sectorsize))
            {
                QMessageBox::critical(this, tr("Verify Error"), image.errorString());
                locked.release();
                CloseHandle(hRawDisk);
                hRawDisk = INVALID_HANDLE_VALUE;
                status = STATUS_IDLE;
                showProgress(false);
                statusbar->showMessage(tr("Verify failed."));
                bCancel->setEnabled(false);
                setReadWriteButtonState();
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
                //For external card readers you may not get device change notification when you remove the card/flash.
                //(So no WM_DEVICECHANGE signal). Device stays but size goes to 0. [Is there special event for this on Windows??]
                QMessageBox::critical(this, tr("File Error"),
                                      tr("The specified file contains no data."));
                locked.release();
                CloseHandle(hRawDisk);
                hRawDisk = INVALID_HANDLE_VALUE;
                status = STATUS_IDLE;
                showProgress(false);
                statusbar->showMessage(tr("Verify failed."));
                bCancel->setEnabled(false);
                setReadWriteButtonState();
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
                    locked.release();
                    CloseHandle(hRawDisk);
                    status = STATUS_IDLE;
                    hRawDisk = INVALID_HANDLE_VALUE;
                    showProgress(false);
                    statusbar->showMessage(tr("Verify failed."));
                    bCancel->setEnabled(false);
                    setReadWriteButtonState();
                    return;
                }
            }
            if (numsectors > availablesectors)
            {
                // A compressed image is read forwards only, so its tail cannot
                // be examined without decompressing everything first.
                bool tailchecked = !image.isCompressed();
                bool datafound = false;
                if (tailchecked)
                {
                    i = availablesectors;
                    unsigned long nextchunksize = 0;
                    while ( (i < numsectors) && (datafound == false) )
                    {
                        nextchunksize = ((numsectors - i) >= 1024ul) ? 1024ul : (numsectors - i);
                        sectorData = image.read(i, nextchunksize, NULL);
                        if(sectorData == NULL)
                        {
                            // if there's an error verifying the truncated data, just move on to the
                            //  write, as we don't care about an error in a section that we're not writing...
                            i = numsectors + 1;
                        } else {
                            unsigned int j = 0;
                            unsigned limit = nextchunksize * sectorsize;
                            while ( (datafound == false) && ( j < limit ) )
                            {
                                if(sectorData[j++] != 0)
                                {
                                    datafound = true;
                                }
                            }
                            i += nextchunksize;
                        }
                        // delete the allocated sectorData
                        delete[] sectorData;
                        sectorData = NULL;
                    }
                }
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
                    locked.release();
                    CloseHandle(hRawDisk);
                    status = STATUS_IDLE;
                    hRawDisk = INVALID_HANDLE_VALUE;
                    showProgress(false);
                    statusbar->showMessage(tr("Verify cancelled."));
                    bCancel->setEnabled(false);
                    setReadWriteButtonState();
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
            bool gptonly = false;

            unsigned long long progresstotal = numsectors;
            if (!image.sizeKnown() && image.sizeInSectors() > 0ull
                && image.sizeInSectors() < progresstotal)
            {
                progresstotal = image.sizeInSectors();
            }
            const int progshift = progressShift(progresstotal);
            progressbar->setRange(0, (progresstotal == 0ull) ? 100
                                                             : (int)(progresstotal >> progshift));
            update_timer.start();
            elapsed_timer->start();
            lasti = 0ul;
            for (i = 0ul; i < numsectors && status == STATUS_VERIFYING; i += 1024ul)
            {
                unsigned long long got = 0ull;
                sectorData = image.read(i, (numsectors - i >= 1024ul) ? 1024ul:(numsectors - i), &got);
                if (sectorData == NULL)
                {
                    QMessageBox::critical(this, tr("Verify Error"), image.errorString());
                    locked.release();
                    CloseHandle(hRawDisk);
                    status = STATUS_IDLE;
                    hRawDisk = INVALID_HANDLE_VALUE;
                    showProgress(false);
                    statusbar->showMessage(tr("Verify failed."));
                    bCancel->setEnabled(false);
                    setReadWriteButtonState();
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
                    QMessageBox::critical(this, tr("Verify Failure"), tr("Verification failed at sector: %1").arg(i));
                    delete[] sectorData;
                    sectorData = NULL;
                    locked.release();
                    CloseHandle(hRawDisk);
                    status = STATUS_IDLE;
                    hRawDisk = INVALID_HANDLE_VALUE;
                    showProgress(false);
                    statusbar->showMessage(tr("Verify failed."));
                    bCancel->setEnabled(false);
                    setReadWriteButtonState();
                    return;
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
                if (update_timer.elapsed() >= ONE_SEC_IN_MS)
                {
                    mbpersec = (((double)sectorsize * (i - lasti)) * ((float)ONE_SEC_IN_MS / update_timer.elapsed())) / 1024.0 / 1024.0;
                    statusbar->showMessage(QString("%1MB/s").arg(mbpersec));
                    update_timer.start();
                    elapsed_timer->update(i, progresstotal);
                    lasti = i;
                }
                delete[] sectorData;
                delete[] sectorData2;
                sectorData = NULL;
                sectorData2 = NULL;
                progressbar->setValue((int)((i > progresstotal ? progresstotal : i) >> progshift));
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
            else if (passfail)
            {
                QString msg = (gptonly)
                    ? tr("Verify Successful.\n\nThe image and the device differ only in the "
                         "GPT, which the \"Fix GPT after write\" option rewrites by design.")
                    : tr("Verify Successful.");
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
    getLogicalDrives();
}

// register to receive notifications when USB devices are inserted or removed
// adapted from http://www.known-issues.net/qt/qt-detect-event-windows.html
bool MainWindow::nativeEvent(const QByteArray &type, void *vMsg, long long *result)
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

    // if there's a value in the md5 label make the copy button visible
    bool haveHash = !(hashLabel->text().isEmpty());
    bHashCopy->setEnabled(haveHash );
}

void MainWindow::on_cboxHashType_IdxChg()
{
    updateHashControls();
}

void MainWindow::on_bHashGen_clicked()
{
    generateHash(leFile->text(), cboxHashType->currentData().toInt());

}
