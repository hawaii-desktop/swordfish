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

#ifndef PLACESMODEL_H
#define PLACESMODEL_H

#include <QStandardItemModel>
#include <QIcon>
#include <QList>
#include <QDir>
#include <QUrl>
#include <QStandardPaths>
#include <QObject>

#include <Kommodity/GIO/VolumeMonitor>
#include <Kommodity/GIO/FileMonitor>

#include "bookmark.h"
#include "mount.h"
#include "volume.h"
#include "placesitem.h"

class PlacesModel : public QStandardItemModel
{
    Q_OBJECT
public:
    PlacesModel(QObject* parent = 0);

    void addBookmark(const QString &icon, const QString &text, const QUrl &url);
    void removeBookmark(const QString &text, const QUrl &url);

public Q_SLOTS:
    void mountAdded(Kommodity::GIO::VolumeMonitor *volumeMonitor,
                    Kommodity::GIO::Mount *mount, PlacesModel *placesModel);

protected:
    PlacesItem *itemFromPath(const QUrl &url);
    Volume *itemFromVolume(Kommodity::GIO::Volume *volume);
    Mount *itemFromMount(Kommodity::GIO::Mount *mount);
    Bookmark *itemFromBookmark(Bookmark *item);

private:
    Kommodity::GIO::VolumeMonitor *m_volumeMonitor;
    PlacesItem *m_trash;
    PlacesItem *m_desktop;
    PlacesItem *m_home;
    PlacesItem *m_computer;
    PlacesItem *m_network;
    PlacesItem *m_filesystem;
    bool m_showTrash;
    bool m_showDesktop;
    bool m_showApplications;
    QList<PlacesItem *> m_items;
    QUrl m_trashUrl;
    QUrl m_computerUrl;
    QUrl m_filesystemUrl;
    QUrl m_networkUrl;

    void updateBookmarks();
};

#endif // PLACESMODEL_H
