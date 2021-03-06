/****************************************************************************
 * This file is part of Swordfish.
 *
 * Copyright (C) 2010-2014 Pier Luigi Fiorini <pierluigi.fiorini@gmail.com>
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

#ifndef PLACESITEM_H
#define PLACESITEM_H

#include <QStandardItem>
#include <Kommodity/GIO/FileInfo>
#include <QIcon>
#include <QList>
#include <QUrl>
#include <QVariant>

class PlacesItem : public QStandardItem
{
public:
    enum Roles {
        CategoryRole = Qt::UserRole + 1,
        UrlRole
    };

    PlacesItem();
    PlacesItem(const QUrl &url);
    PlacesItem(const QIcon &icon, const QString &text, const QUrl &url);
    PlacesItem(const QString &icon, const QString &text, const QUrl &url);
    PlacesItem(const QString &text, const QUrl &url);
    ~PlacesItem();

    inline QString category() const {
        return qvariant_cast<QString>(data(CategoryRole));
    }

    void setCategory(const QString &category);

    void setUrl(const QUrl &path);
    void setInfo(const Kommodity::GIO::FileInfo &fileInfo);

    QVariant data (int role = Qt::UserRole + 1) const;

    Kommodity::GIO::FileInfo fileInfo() const {
        return m_fileInfo;
    }

    QUrl url() const {
        return m_url;
    }
    
private:
    QString m_category;
    Kommodity::GIO::FileInfo m_fileInfo;
    QUrl m_url;
};

#endif // PLACESITEM_H
