/****************************************************************************
 * This file is part of Swordfish.
 *
 * Copyright (C) 2010-2013 Pier Luigi Fiorini <pierluigi.fiorini@gmail.com>
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

#include "placesmodel.h"

PlacesModel::PlacesModel(QObject* parent)
: QStandardItemModel(parent),
  m_showApplications(true),
  m_showDesktop(true),
  m_showTrash(true)
{   
    PlacesItem *item;
    m_placesRoot = new QStandardItem(tr("Places"));
    m_placesRoot->setEditable(false);
    m_placesRoot->setSelectable(false);
    appendRow(m_placesRoot);

    m_home = new PlacesItem("user-home", QStandardPaths::displayName(QStandardPaths::HomeLocation),
                            QStandardPaths::writableLocation(QStandardPaths::HomeLocation));
    m_home->setEditable(false);
    m_placesRoot->appendRow(m_home);

    m_desktop = new PlacesItem("user-desktop", tr("Desktop"),
                               QStandardPaths::writableLocation(QStandardPaths::HomeLocation));
    m_desktop->setEditable(false);
    m_placesRoot->appendRow(m_desktop);

    m_trashUrl.setUrl("trash:///");
    m_trash = new PlacesItem("user-trash", tr("Trash"), m_trashUrl);
    m_trash->setEditable(false);
    m_placesRoot->appendRow(m_trash);

    m_computerUrl.setUrl("computer:///");
    m_computer = new PlacesItem("computer", tr("Computer"), m_computerUrl);
    m_computer->setEditable(false);
    m_placesRoot->appendRow(m_computer);

    m_application = new PlacesItem("system-software-install", tr("Application"),
                                   QStandardPaths::writableLocation(QStandardPaths::ApplicationsLocation));
    m_application->setEditable(false);
    m_placesRoot->appendRow(m_application);

    m_networkUrl.setUrl("network:///");
    m_network = new PlacesItem("network", tr("Network"), m_networkUrl);
    m_network->setEditable(false);
    m_placesRoot->appendRow(m_network);

    m_deviceRoot = new QStandardItem(tr("Devices"));
    m_deviceRoot->setEditable(false);
    m_deviceRoot->setSelectable(false);
    appendRow(m_deviceRoot);


}

PlacesModel::~PlacesModel()
{
}

int PlacesModel::addBookmark(const QString &icon, const QString &text, const QUrl &url)
{
    for(int i=0;i<m_bookmarks.length();i++)
    {
        if(m_bookmarks.at(i)->text()==text && m_bookmarks.at(i)->url()==url)
           return -1;
    }
    m_bookmarks.append(new Bookmark(text, icon, url));
    return 0;
}

void PlacesModel::removeBookmark(const QString &text, const QUrl &url)
{
    for(int i=0; i<m_bookmarks.length(); i++)
        if(m_bookmarks.at(i)->text()==text && m_bookmarks.at(i)->url()==url)
            m_bookmarks.removeAt(i);
}
