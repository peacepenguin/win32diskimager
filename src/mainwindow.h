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
 *  Copyright (C) 2009-2014 ImageWriter developers                    *
 *                 https://sourceforge.net/projects/win32diskimager/  *
 *  ---                                                               *
 *  Modified 2026 by peacepenguin (fork not affiliated      *
 *  with the upstream ImageWriter project):                           *
 *  GPT repair, device list, dialogs, hashing                         *
 *  https://github.com/peacepenguin/windiskimager                   *
 **********************************************************************/

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#ifndef WINVER
#define WINVER 0x0601
#endif

#include <QtWidgets>
#include <QClipboard>
#include <windows.h>
#include <memory>
#include "ui_mainwindow.h"
#include "elapsedtimer.h"

class ImageSource;
class LockedVolumes;

class MainWindow : public QMainWindow, public Ui::MainWindow
{
    Q_OBJECT
    public:
        static MainWindow* getInstance() {
            // !NOT thread safe  - first call from main only
            if (!instance)
                instance = new MainWindow();
            return instance;
        }
        static MainWindow* getInstanceIfAvailable() {
            // getInstance crashes if invoked during init to get MessageBox-parent
            // thus, we simply return instance (NULL, if not yet created) to avoid app crash.
            return instance;
        }

        ~MainWindow();
        void closeEvent(QCloseEvent *event);
        enum Status {STATUS_IDLE=0, STATUS_READING, STATUS_WRITING, STATUS_VERIFYING, STATUS_EXIT, STATUS_CANCELED};
        bool nativeEvent(const QByteArray &type, void *vMsg, qintptr *result) override;
    protected slots:
        void on_tbBrowse_clicked();
        void on_bCancel_clicked();
        void on_bCheckGpt_clicked();
        void on_bWrite_clicked();
        void on_bRead_clicked();
        void on_bVerify_clicked();
        void on_leFile_editingFinished();
        void on_bHashCopy_clicked();
        void on_showAllDevicesCheckBox_toggled(bool checked);
        // Compressing to .gz and to .xz are mutually exclusive; each unchecks
        // the other. See mainwindow.cpp.
        void on_readGzCheckBox_toggled(bool checked);
        void on_readXzCheckBox_toggled(bool checked);
private slots:
        void on_cboxHashType_IdxChg();
        void on_bHashGen_clicked();
protected:
        MainWindow(QWidget* = NULL);
private:
        static MainWindow* instance;
        // find attached devices
        void getLogicalDrives();
        // Rescans behind a status message; see mainwindow.cpp.
        void rescanDevices();
        int selectedDeviceID();
        bool fileIsOnSelectedDevice(const QString &file);
        void setReadWriteButtonState();
        void initializeHomeDir();
        void updateHashControls();
        // Everything that follows from a different image file; see mainwindow.cpp.
        void imageFileChanged();
        // Picks a default hash type when a different image is chosen.
        void defaultHashTypeForFile();
        // Sets the bar up for a run and starts the clocks; see mainwindow.cpp.
        int beginProgress(unsigned long long total, unsigned long long *lastsector);
        // Shows or hides the progress group; see mainwindow.cpp.
        void showProgress(bool show);
        // Shows the transfer rate, at most once a second; see mainwindow.cpp.
        void showThroughput(unsigned long long sector, unsigned long long total,
                            unsigned long long *lastsector);
        // Puts the window back to idle and says why; see mainwindow.cpp.
        void endRun(const QString &message);
        // Offers to repair a primary GPT found broken; see mainwindow.cpp.
        bool offerGptRepair(HANDLE hDisk, unsigned long long devicesectors,
                            const QString &lead);
        // Locks the device, opens it and the image; see mainwindow.cpp.
        bool acquireDeviceAndImage(int deviceID, LockedVolumes &locked,
                                   ImageSource &image,
                                   unsigned long long *devicesectors,
                                   const QString &errorTitle,
                                   const QString &failedMessage);
        // Scans the part of an image that will not fit; see mainwindow.cpp.
        bool imageTailHasData(ImageSource &image, unsigned long long from,
                              unsigned long long to, bool *datafound);

        HANDLE hFile;
        HANDLE hRawDisk;
        static const unsigned short ONE_SEC_IN_MS = 1000;
        unsigned long long sectorsize;
        int status;
        char *sectorData;
        char *sectorData2; //for verify
        QElapsedTimer update_timer;
        ElapsedTimer *elapsed_timer = NULL;
        QClipboard *clipboard;
        void generateHash(const QString &filename, int hashish);
        QString deviceSignature;
        QString myHomeDir;
        QString myFileType;
        // The image the hash type was last defaulted for, so that choosing a
        // type by hand is not undone on every pass through the field.
        QString myHashDefaultedFor;
        QStringList myFileTypeList;
};

#endif // MAINWINDOW_H
