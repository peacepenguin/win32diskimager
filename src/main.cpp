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
 **********************************************************************/

#ifndef WINVER
#define WINVER 0x0601
#endif

#include <QApplication>
#include <windows.h>
#include "mainwindow.h"


int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    app.setApplicationDisplayName(VER);

    // Qt's own strings -- QMessageBox buttons, the file dialog -- live in
    // translations/ next to the exe, under one of two names depending on how
    // the copy was packaged. tools/deploy-cross.sh assembles the directory by
    // hand and copies Qt's per-module catalogues, so they arrive as
    // qtbase_<lang>.qm. windeployqt, which tools/deploy.sh uses, instead merges
    // every module into a single qt_<lang>.qm and writes no qtbase file at all.
    //
    // Asking only for "qtbase" therefore left every Qt-supplied string in
    // English on a natively built copy, and said nothing about it: the
    // application's own strings are compiled in through translations.qrc and
    // went on being translated, so the window looked right and its dialog
    // buttons did not.
    //
    // qtbase first, because where it exists it is the catalogue these strings
    // actually live in; the merged file is the fallback.
    QTranslator qttranslator;
    const QString qttrdir = QCoreApplication::applicationDirPath() + "/translations";
    if (qttranslator.load(QLocale::system(), "qtbase", "_", qttrdir)
        || qttranslator.load(QLocale::system(), "qt", "_", qttrdir))
        app.installTranslator(&qttranslator);

    // The app's own strings are compiled in by translations.qrc, under :/lang.
    QTranslator translator;
    if (translator.load(QLocale::system(), "diskimager", "_", ":/lang"))
        app.installTranslator(&translator);

    MainWindow* mainwindow = MainWindow::getInstance();
    mainwindow->show();
    return app.exec();
}
