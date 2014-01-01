/****************************************************************************
 * This file is part of Swordfish.
 *
 * Copyright (C) 2013-2014 Pier Luigi Fiorini <pierluigi.fiorini@gmail.com>
 *
 * Author(s):
 *    Pier Luigi Fiorini
 *
 * $BEGIN_LICENSE:GPL2+$
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * $END_LICENSE$
 ***************************************************************************/

import QtQuick 2.1
import QtQuick.Controls 1.0
import QtQuick.Layouts 1.0
import Swordfish 0.1

ApplicationWindow {
    title: "Swordfish"
    width: 640
    height: 480
    minimumWidth: 400
    minimumHeight: 400
    color: palette.window
    toolBar: ToolBar {
        id: toolBar

        RowLayout {
            spacing: 2
            anchors.verticalCenter: parent.verticalCenter

            ToolButton {
                action: actionBack
            }

            ToolButton {
                action: actionForward
            }
        }
    }

    SystemPalette {
        id: palette
    }

    Action {
        id: actionBack
        text: qsTr("&Back")
        tooltip: qsTr("Back")
        shortcut: "Alt+Left"
        iconName: "go-previous"
        enabled: false
    }

    Action {
        id: actionForward
        text: qsTr("&Forward")
        tooltip: qsTr("Forward")
        shortcut: "Alt+Right"
        iconName: "go-next"
        enabled: false
    }

    SplitView {
        anchors.fill: parent

        PlacesView {
            id: placesView

            Layout.minimumWidth: 200
            Layout.maximumWidth: 250

            onSelected: folderView.setFolder(uri)
        }

        FolderView {
            id: folderView
        }
    }
}
