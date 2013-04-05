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

#include <QDebug>

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

    m_volumeMonitor = new VolumeMonitor();

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
    addPlacesBookmark("drive-harddisk", tr("File System"), QUrl("file:///"));
    addPlacesBookmark("user-trash", tr("Trash"), QUrl("trash:///"));
    addPlacesBookmark("computer", tr("Computer"), QUrl("computer:///"));

    // Find all volumes and append to the Devices category
    QList<Volume *> volumes = m_volumeMonitor->getVolumes();
    for (int i = 0; i < volumes.size(); i++) {
        Volume *volume = volumes.at(i);
        VolumeItem *item = new VolumeItem(volume);
        appendRow(item);
    }

    // Network
    addNetworkBookmark("network", tr("Network"), QUrl("network:///"));

    connect(m_volumeMonitor, SIGNAL(volumeAdded(const VolumeMonitor*, const Volume*)),
            this, SLOT(volumeAdded(const VolumeMonitor*, const Volume*)));
    connect(m_volumeMonitor, SIGNAL(volumeChanged(const VolumeMonitor*,const Volume*)),
            this, SLOT(volumeChanged(const VolumeMonitor*, const Volume*)));
    connect(m_volumeMonitor, SIGNAL(volumeRemoved(const VolumeMonitor*, const Volume*)),
            this, SLOT(volumeRemoved(const VolumeMonitor*, const Volume*)));
    connect(m_volumeMonitor, SIGNAL(mountAdded(const VolumeMonitor*, const Mount*)),
            this, SLOT(mountAdded(const VolumeMonitor*, const Mount*)));
    connect(m_volumeMonitor, SIGNAL(mountChanged(const VolumeMonitor*, const Mount*)),
            this, SLOT(mountChanged(const VolumeMonitor*, const Mount*)));
    connect(m_volumeMonitor, SIGNAL(mountRemoved(const VolumeMonitor*, const Mount*)),
            this, SLOT(mountRemoved(const VolumeMonitor*, const Mount*)));
}

void PlacesModel::volumeAdded(const VolumeMonitor *volumeMonitor,
                              const Volume *volume)
{
    Q_UNUSED(volumeMonitor);

    VolumeItem *item = itemFromVolume(const_cast<Volume *>(volume));
    if (!item) {
        // Fine, we don't have an item for this volume so we create an item
        // and append it
        VolumeItem *item = new VolumeItem(const_cast<Volume *>(volume));
        appendRow(item);
    }
}

void PlacesModel::volumeChanged(const VolumeMonitor *volumeMonitor,
                                const Volume *volume)
{
    Q_UNUSED(volumeMonitor);

    VolumeItem *item = itemFromVolume(const_cast<Volume *>(volume));
    if (!item) {
        qWarning() << "No volume item for" << volume->getUuid();
        return;
    }

    item->setVolume(const_cast<Volume *>(volume));
}

void PlacesModel::volumeRemoved(const VolumeMonitor *volumeMonitor,
                                const Volume *volume)
{
    Q_UNUSED(volumeMonitor);

    for (int i = 0; i < m_items.length(); i++) {
        VolumeItem *item = static_cast<VolumeItem *>(m_items.at(i));
        if (!item)
            continue;

        Volume *theVolume = item->volume();
        if (theVolume && theVolume->getUuid() == volume->getUuid()) {
            removeRow(i);
            m_items.removeAt(i);
            return;
        }
    }

    qWarning() << "No volume item for" << volume->getUuid();
}

void PlacesModel::mountAdded(const VolumeMonitor *volumeMonitor,
                             const Mount *mount)
{
    Volume *volume = mount->getVolume();
    if (!volume) {
        qWarning() << "Mount" << mount->getUuid() << "has no volume";
        return;
    }

    volumeAdded(volumeMonitor, volume);
}

void PlacesModel::mountChanged(const VolumeMonitor *volumeMonitor,
                               const Mount *mount)
{
    Volume *volume = mount->getVolume();
    if (!volume) {
        qWarning() << "Mount" << mount->getUuid() << "has no volume";
        return;
    }

    volumeChanged(volumeMonitor, volume);
}

void PlacesModel::mountRemoved(const VolumeMonitor *volumeMonitor,
                               const Mount *mount)
{
    Volume *volume = mount->getVolume();
    if (!volume) {
        qWarning() << "Mount" << mount->getUuid() << "has no volume";
        return;
    }

    volumeRemoved(volumeMonitor, volume);
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

VolumeItem *PlacesModel::itemFromVolume(Volume *volume)
{
    for (int i = 0; i < m_items.length(); i++) {
        VolumeItem *item = static_cast<VolumeItem *>(m_items.at(i));
        if (!item)
            continue;

        Volume *theVolume = item->volume();
        if (theVolume) qDebug() << theVolume << theVolume->getName();
        if (theVolume) qDebug() << theVolume << theVolume->getUuid();
        if (theVolume && theVolume->getUuid() == volume->getUuid())
            return item;
    }

    return 0;
}

VolumeItem *PlacesModel::itemFromMount(Mount *mount)
{
    for (int i = 0; i < m_items.length(); i++) {
        VolumeItem *item = static_cast<VolumeItem *>(m_items.at(i));
        if (!item)
            continue;

        Mount *theMount = item->volume()->getMount();
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
