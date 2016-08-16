/****************************************************************************
 * This file is part of Swordfish.
 *
 * Copyright (C) 2010-2016 Pier Luigi Fiorini
 *
 * Author(s):
 *    Pier Luigi Fiorini <pierluigi.fiorini@gmail.com>
 *
 * $BEGIN_LICENSE:LGPL2.1+$
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 2.1 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * $END_LICENSE$
 ***************************************************************************/

import QtQuick 2.1
import QtQuick.Controls 1.0
import QtQuick.Layouts 1.0
import Qt.labs.controls 1.0
import Swordfish.Folders 1.0

Item {
    id: container

    function setFolder(uri) {
        gridView.model.folder = uri;
    }

    SystemPalette {
        id: palette
    }

    Component {
        id: gridDelegate

        ItemDelegate {
            width: gridView.cellWidth
            height: gridView.cellHeight
            checked: gridView.currentIndex == index
            text: model.filePath
            onClicked: gridView.currentIndex = index
        }
    }

    GridView {
        id: gridView
        cellWidth: 100
        cellHeight: 100
        model: FolderListModel {
            path: "trash://"
        }
        delegate: gridDelegate

        ScrollBar.horizontal: ScrollBar {}
        ScrollBar.vertical: ScrollBar {}
    }
}
