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

import QtQuick 2.0
import Swordfish 0.1
import FluidExtra 1.0 as FluidExtra

Rectangle {
    id: container
    width: 400
    height: 400
    color: sysPal.window

    SystemPalette {
        id: sysPal
    }

    ListView {
        anchors.fill: parent

        Component {
            id: sectionHeading

            Rectangle {
                width: container.width
                height: childrenRect.height
                color: "lightsteelblue"

                Text {
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

                FluidExtra.IconItem {
                    id: icon
                    anchors {
                        left: parent.left
                        top: parent.top
                    }
                    width: 22
                    height: 22
                    icon: decoration
                    smooth: true
                }

                Text {
                    id: label
                    anchors {
                        left: icon.right
                        verticalCenter: icon.verticalCenter
                    }
                    text: display
                }
            }
        }

        model: PlacesModel {}
        delegate: delegate
        spacing: 8
        section.property: "category"
        section.criteria: ViewSection.FullString
        section.delegate: sectionHeading
    }
}
