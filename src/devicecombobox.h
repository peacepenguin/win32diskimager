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

#ifndef DEVICECOMBOBOX_H
#define DEVICECOMBOBOX_H

#include <QtWidgets>
#include <QComboBox>

// The device list, which rescans as it is opened rather than on a timer.
//
// Scanning asks every disk for its geometry, and a disk that has spun down
// answers only once it is turning again. On a timer that kept the machine's
// disks awake around the clock to maintain a list nobody was looking at; done
// here it happens when somebody is about to read the list, which is the only
// moment it has to be right.
//
// showPopup() rather than a click handler: the list also opens from the
// keyboard, with F4 and Alt+Down, and a handler watching for mouse presses
// would quietly not rescan for anyone who works that way.
class DeviceComboBox : public QComboBox
{
    Q_OBJECT

public:
    DeviceComboBox(QWidget *parent = 0);

    void showPopup() override;

signals:
    // Emitted before the list drops down, while there is still time to change
    // what it contains. Connected slots run to completion first.
    void aboutToShowPopup();
};

#endif // DEVICECOMBOBOX_H
