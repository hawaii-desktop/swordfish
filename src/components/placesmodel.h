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

#include "bookmarkitem.h"
#include "mountitem.h"
#include "volumeitem.h"
#include "placesitem.h"

class PlacesModel : public QStandardItemModel
{
    Q_OBJECT
public:
    PlacesModel(QObject* parent = 0);

    void addBookmark(const QString &icon, const QString &text,
                     const QUrl &url, bool editable = true);
    void addPlacesBookmark(const QString &icon, const QString &text,
                           const QUrl &url);
    void addNetworkBookmark(const QString &icon, const QString &text,
                            const QUrl &url);
    void removeBookmark(const QString &text, const QUrl &url);

public Q_SLOTS:
    void mountAdded(Kommodity::GIO::VolumeMonitor *volumeMonitor,
                    Kommodity::GIO::Mount *mount);

protected:
    PlacesItem *itemFromPath(const QUrl &url);
    VolumeItem *itemFromVolume(Kommodity::GIO::Volume *volume);
    MountItem *itemFromMount(Kommodity::GIO::Mount *mount);
    BookmarkItem *itemFromBookmark(BookmarkItem *item);

private:
    Kommodity::GIO::VolumeMonitor *m_volumeMonitor;
    bool m_showApplications;
    bool m_showDesktop;
    bool m_showTrash;
    QList<PlacesItem *> m_items;

    void updateBookmarks();
};

#endif // PLACESMODEL_H
