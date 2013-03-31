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
    //Initializing some variables;
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

    m_filesystemUrl.setUrl("file:///");
    m_filesystem = new PlacesItem("volume", tr("File system"), m_filesystemUrl);
    m_filesystem->setEditable(false);
    m_placesRoot->appendRow(m_filesystem);

    m_trashUrl.setUrl("trash:///");
    m_trash = new PlacesItem("user-trash", tr("Trash"), m_trashUrl);
    m_trash->setEditable(false);
    m_placesRoot->appendRow(m_trash);

    m_computerUrl.setUrl("computer:///");
    m_computer = new PlacesItem("computer", tr("Computer"), m_computerUrl);
    m_computer->setEditable(false);
    m_placesRoot->appendRow(m_computer);


    m_networkRoot = new QStandardItem(tr("Network"));
    m_networkRoot->setEditable(false);
    m_networkRoot->setSelectable(false);
    m_networkUrl.setUrl("network:///");
    m_network = new PlacesItem("network", tr("Network"), m_networkUrl);
    m_network->setEditable(false);
    m_placesRoot->appendRow(m_network);

    m_devicesRoot = new QStandardItem(tr("Devices"));
    m_devicesRoot->setEditable(false);
    m_devicesRoot->setSelectable(false);
    appendRow(m_devicesRoot);

    //Find all volumes and append them to m_devicesRoot
    QList<Kommodity::GIO::Volume *> volumes = m_volumeMonitor->getVolumes();

        for (int i = 0; i < volumes.size(); ++i) {
            Kommodity::GIO::Volume *volume = volumes.at(i);
            item = new Volume(*volume);
            m_devicesRoot->appendRow(item);
        }
    //Find all  mounts and append them to m_devicesRoot
    QList<Kommodity::GIO::Mount *> mounts = m_volumeMonitor->getMounts();

        for (int i = 0; i < mounts.size(); ++i) {
            Kommodity::GIO::Mount *mount = mounts.at(i);
            Kommodity::GIO::Volume *volume = mount->getVolume();

            if(!volume) {
            item = new Mount(*mount);
            m_devicesRoot->appendRow(item);
            }
        }

    m_bookmarksRoot = new QStandardItem(tr("Bookmakrs"));
    m_bookmarksRoot->setEditable(false);
    m_bookmarksRoot->setSelectable(false);
    appendRow(m_bookmarksRoot);

    addBookmark("folder-documents-symbolic",tr("Documents"),
                QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation));
    addBookmark("folder-music-symbolic",tr("Music"),
                QStandardPaths::writableLocation(QStandardPaths::MusicLocation));
    addBookmark("folder-pictures-symbolic",tr("Pictures"),
                QStandardPaths::writableLocation(QStandardPaths::PicturesLocation));
    addBookmark("folder-videos-symbolic",tr("Videos"),
                QStandardPaths::writableLocation(QStandardPaths::MoviesLocation));
    addBookmark("folder-download-symbolic",tr("Downloads"),
                QStandardPaths::writableLocation(QStandardPaths::DownloadLocation));



    for(int i = 0; i < m_bookmarks.size(); ++i) {
        m_bookmarksRoot->appendRow(m_bookmarks.at(i));
    }



    connect(m_volumeMonitor, SIGNAL(mountAdded(Kommodity::GIO::VolumeMonitor *, Kommodity::GIO::Mount *)),
               this, SLOT(mountAdded(Kommodity::GIO::VolumeMonitor *, Kommodity::GIO::Mount *, PlacesModel *)));


}

void PlacesModel::mountAdded(Kommodity::GIO::VolumeMonitor *volumeMonitor, Kommodity::GIO::Mount *mount, PlacesModel *placesModel)
{
    Kommodity::GIO::Volume *volume = mount->getVolume();
    if(volume) {
        Volume *volumeItem = placesModel->itemFromVolume(volume);

        if (volumeItem && volumeItem->url().isEmpty()) {
            Kommodity::GIO::File file = mount->getRoot();
            QUrl murl = file.getUri();
            volumeItem->setUrl(murl);
        }
        else {
            Mount *mountItem = placesModel->itemFromMount(mount);
            if(!mountItem) {
                mountItem = new Mount(*mount);
                placesModel->m_devicesRoot->appendRow(mountItem);
            }
        }
    }
}


PlacesModel::~PlacesModel()
{
}

int PlacesModel::addBookmark(const QString &icon, const QString &text, const QUrl &url)
{
    for(int i=0;i<m_bookmarks.length();i++)
    {
        if(m_bookmarks.at(i)->text()==text && m_bookmarks.at(i)->url() == url)
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

Volume *PlacesModel::itemFromVolume(Kommodity::GIO::Volume *volume)
{
    int rowCount = m_devicesRoot->rowCount();
    for (int i = 0; i < rowCount; ++i) {
        PlacesItem *item = static_cast<PlacesItem *>(m_devicesRoot->child(i, 0));
        if (item->type() == VOLUME)
            if (static_cast<Volume *>(item)->volume() == volume) {
                Volume *volumeItem = static_cast<Volume *>(item);
                return volumeItem;
            }


    }
}

Mount *PlacesModel::itemFromMount(Kommodity::GIO::Mount *mount)
{
    int rowCount = m_devicesRoot->rowCount();
    for (int i = 0; i < rowCount; ++i) {
        PlacesItem *item = static_cast<PlacesItem *>(m_devicesRoot->child(i, 0));
        if(item->type() == MOUNT)
        if (static_cast<Mount *>(item)->mount() == mount) {
            Mount *mountItem = static_cast<Mount *>(item);
            return mountItem;
        }
    }
}

Bookmark *PlacesModel::itemFromBookmark(Bookmark *item)
{

}

#include "moc_placesmodel.cpp"
