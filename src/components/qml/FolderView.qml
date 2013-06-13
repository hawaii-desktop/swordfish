/****************************************************************************
 * This file is part of Swordfish.
 *
 * Copyright (C) 2010-2013 Pier Luigi Fiorini <pierluigi.fiorini@gmail.com>
 *
 * Author(s):
 *    Pier Luigi Fiorini
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
import FluidExtra 1.0 as FluidExtra
import Swordfish 0.1

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

        Item {
            width: gridView.cellWidth
            height: gridView.cellHeight

            MouseArea {
                anchors.fill: parent
                onClicked: gridView.currentIndex = index
            }

            ColumnLayout {
                anchors.fill: parent

                FluidExtra.IconItem {
                    icon: fileIcon
                    width: 48
                    height: 48

                    Layout.alignment: Qt.AlignCenter
                }

                Label {
                    text: model.fileName
                    horizontalAlignment: Qt.AlignHCenter
                    verticalAlignment: Qt.AlignTop
                    wrapMode: Text.Wrap
                    elide: Text.ElideRight

                    Layout.fillWidth: true
                    Layout.fillHeight: true
                }
            }
        }
    }

    ScrollView {
        anchors.fill: parent

        GridView {
            id: gridView
            cellWidth: 100
            cellHeight: 100
            model: FolderModel {}
            delegate: gridDelegate
            highlight: Rectangle {
                radius: 4
                color: palette.highlight
            }
        }
    }
}
