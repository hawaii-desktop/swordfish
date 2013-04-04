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

#include "placesmodel.h"

PlacesModel::PlacesModel(QObject* parent)
    : QStandardItemModel(parent),
      m_showApplications(true),
      m_showDesktop(true),
      m_showTrash(true)
{   
    // Set role names
    QHash<int, QByteArray> roles;
    roles[Qt::DisplayRole] = "display";
    roles[Qt::DecorationRole] = "decoration";
    roles[PlacesItem::CategoryRole] = "category";
    roles[PlacesItem::UrlRole] = "url";
    setItemRoleNames(roles);

    m_volumeMonitor = new Kommodity::GIO::VolumeMonitor();

    // Bookmarks
    addBookmark("folder-documents-symbolic",
                QStandardPaths::displayName(QStandardPaths::DocumentsLocation),
                QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation),
                false);
    addBookmark("folder-music-symbolic",
                QStandardPaths::displayName(QStandardPaths::MusicLocation),
                QStandardPaths::writableLocation(QStandardPaths::MusicLocation),
                false);
    addBookmark("folder-pictures-symbolic",
                QStandardPaths::displayName(QStandardPaths::PicturesLocation),
                QStandardPaths::writableLocation(QStandardPaths::PicturesLocation),
                false);
    addBookmark("folder-videos-symbolic",
                QStandardPaths::displayName(QStandardPaths::MoviesLocation),
                QStandardPaths::writableLocation(QStandardPaths::MoviesLocation),
                false);
    addBookmark("folder-download-symbolic",
                QStandardPaths::displayName(QStandardPaths::DownloadLocation),
                QStandardPaths::writableLocation(QStandardPaths::DownloadLocation),
                false);

    // Places
    addPlacesBookmark("user-home",
                      QStandardPaths::displayName(QStandardPaths::HomeLocation),
                      QStandardPaths::writableLocation(QStandardPaths::HomeLocation));
    if (m_showDesktop)
        addPlacesBookmark("user-desktop",
                          QStandardPaths::displayName(QStandardPaths::DesktopLocation),
                          QStandardPaths::writableLocation(QStandardPaths::DesktopLocation));
    addPlacesBookmark("volume", tr("File System"), QUrl("file:///"));
    addPlacesBookmark("user-trash", tr("Trash"), QUrl("trash:///"));
    addPlacesBookmark("computer", tr("Computer"), QUrl("computer:///"));

    // Network
    addNetworkBookmark("network", tr("Network"), QUrl("network:///"));

    PlacesItem *item;

    // Find all volumes and append to the Devices category
    QList<Kommodity::GIO::Volume *> volumes = m_volumeMonitor->getVolumes();

    for (int i = 0; i < volumes.size(); ++i) {
        Kommodity::GIO::Volume *volume = volumes.at(i);
        item = new VolumeItem(volume);
        appendRow(item);
    }

    connect(m_volumeMonitor, SIGNAL(mountAdded(VolumeMonitor *, MountItem *)),
            this, SLOT(mountAdded(Kommodity::GIO::VolumeMonitor *, Kommodity::GIO::Mount *)));
}

void PlacesModel::mountAdded(Kommodity::GIO::VolumeMonitor *volumeMonitor,
                             Kommodity::GIO::Mount *mount)
{
    Q_UNUSED(volumeMonitor);

    Kommodity::GIO::Volume *volume = mount->getVolume();
    if (!volume)
        return;

    VolumeItem *item = new VolumeItem(volume);
    appendRow(item);
}

void PlacesModel::addBookmark(const QString &icon, const QString &text,
                              const QUrl &url, bool editable)
{
    BookmarkItem *item = new BookmarkItem(text, icon, url);
    item->setEditable(editable);
    m_items.append(item);
    appendRow(item);
}

void PlacesModel::addPlacesBookmark(const QString &icon, const QString &text,
                                    const QUrl &url)
{
    BookmarkItem *item = new BookmarkItem(text, icon, url);
    item->setCategory(tr("Places"));
    item->setEditable(false);
    m_items.append(item);
    appendRow(item);
}

void PlacesModel::addNetworkBookmark(const QString &icon, const QString &text,
                                     const QUrl &url)
{
    BookmarkItem *item = new BookmarkItem(text, icon, url);
    item->setCategory(tr("Network"));
    item->setEditable(false);
    m_items.append(item);
    appendRow(item);
}

void PlacesModel::removeBookmark(const QString &text, const QUrl &url)
{
    for(int i = 0; i < m_items.length(); i++) {
        BookmarkItem *item = static_cast<BookmarkItem *>(m_items.at(i));
        if (!item)
            continue;

        if (item->text() == text && item->url() == url)
            m_items.removeAt(i);
    }
}

VolumeItem *PlacesModel::itemFromVolume(Kommodity::GIO::Volume *volume)
{
    for (int i = 0; i < m_items.length(); i++) {
        VolumeItem *item = static_cast<VolumeItem *>(m_items.at(i));
        if (!item)
            continue;

        Kommodity::GIO::Volume *theVolume = item->volume();
        if (theVolume && theVolume->getUuid() == volume->getUuid())
            return item;
    }

    return 0;
}

VolumeItem *PlacesModel::itemFromMount(Kommodity::GIO::Mount *mount)
{
    for (int i = 0; i < m_items.length(); i++) {
        VolumeItem *item = static_cast<VolumeItem *>(m_items.at(i));
        if (!item)
            continue;

        Kommodity::GIO::Mount *theMount = item->volume()->getMount();
        if (theMount && theMount->getUuid() == mount->getUuid())
            return item;
    }

    return 0;
}

BookmarkItem *PlacesModel::itemFromBookmark(BookmarkItem *item)
{
    return 0;
}

#include "moc_placesmodel.cpp"
