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
import Swordfish 0.1
import Fluid.Extra 1.0 as FluidExtra

Item {
    id: container

    signal selected(url uri)

    SystemPalette {
        id: palette
    }

    ListView {
        id: placesListView
        anchors.fill: parent

        Component {
            id: sectionHeading

            Item {
                width: container.width
                height: childrenRect.height

                Label {
                    text: section
                    font.weight: Font.Bold
                }
            }
        }

        Component {
            id: delegate

            Item {
                width: container.width
                height: childrenRect.height

                property bool selected: placesListView.currentIndex == index

                FluidExtra.IconItem {
                    id: icon
                    anchors {
                        left: parent.left
                        top: parent.top
                        leftMargin: 10
                        topMargin: 5
                    }
                    width: 22
                    height: 22
                    icon: decoration
                    smooth: true
                }

                Label {
                    id: label
                    anchors {
                        left: icon.right
                        leftMargin: 6
                        topMargin: 5
                        verticalCenter: icon.verticalCenter
                    }
                    text: display
                    color: selected ? palette.highlightedText : palette.windowText
                }

                MouseArea {
                    anchors.fill: parent
                    onClicked: {
                        // Select this item and emit a signal
                        placesListView.currentIndex = index;
                        container.selected(url);
                    }
                }
            }
        }

        model: PlacesModel {}
        delegate: delegate
        highlight: Rectangle {
            color: palette.highlight
            opacity: 0.7
        }
        highlightMoveDuration: 0
        spacing: 8
        section.property: "category"
        section.criteria: ViewSection.FullString
        section.delegate: sectionHeading
    }
}
