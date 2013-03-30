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

#ifndef PLACESMODEL_H
#define PLACESMODEL_H

#include <QStandardItemModel>
#include <Kommodity/GIO/VolumeMonitor>
#include <Kommodity/GIO/FileMonitor>
#include <QIcon>
#include <QList>
#include <QDir>
#include <QUrl>
#include <QStandardPaths>
#include <QObject>

#include "bookmark.h"
#include "mount.h"
#include "volume.h"
#include "placesitem.h"

class PlacesModel : public QStandardItemModel
{
    Q_OBJECT
public:
    PlacesModel(QObject* parent = 0);

    ~PlacesModel();


    int addBookmark(const QString &icon,const QString &text,const QUrl &url);
    void removeBookmark(const QString &text, const QUrl &url);


protected:
    PlacesItem *itemFromPath(const QUrl &url);
    Volume *itemFromVolume(Kommodity::GIO::Volume *volume);
    Mount *itemFromMount(Kommodity::GIO::Mount *mount);

private:
    Kommodity::GIO::VolumeMonitor *m_volumeMonitor;
    PlacesItem *m_trash;
    PlacesItem *m_desktop;
    PlacesItem *m_home;
    PlacesItem *m_computer;
    PlacesItem *m_network;
    PlacesItem *m_filesystem;
    QStandardItem *m_placesRoot;
    QStandardItem *m_deviceRoot;
    QStandardItem *m_networkRoot;
    QStandardItem *m_bookmarksRoot;
    bool m_showTrash;
    bool m_showDesktop;
    bool m_showApplications;
    QList<Bookmark *> m_bookmarks;
    QUrl m_trashUrl;
    QUrl m_computerUrl;
    QUrl m_filesystemUrl;
    QUrl m_networkUrl;
};

#endif // PLACESMODEL_H


