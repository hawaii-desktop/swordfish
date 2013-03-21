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

#include "placesitem.h"

PlacesItem::PlacesItem(const QUrl &url)
: QStandardItem()
{
    m_url = url;
}

PlacesItem::PlacesItem(const QString &icon,const QString &text,const QUrl &url)
: QStandardItem(text)
{
    m_url = url;
    setIcon(QIcon::fromTheme(icon));
}
PlacesItem::PlacesItem(const QIcon &icon,const QString &text,const QUrl &url)
:  QStandardItem(icon,text)
{
    m_url = url;
}

PlacesItem::PlacesItem(const QString &text,const QUrl &url)
: QStandardItem(text)
{
    m_url = url;
}

PlacesItem::~PlacesItem()
{

}

void PlacesItem::setUrl(const QUrl &url)
{
    m_url = url;
}

QVariant PlacesItem::data(int role) const {
    return QStandardItem::data(role);
}

void PlacesItem::setInfo(const Kommodity::GIO::FileInfo &fileInfo)
{
    m_fileInfo = fileInfo;
}


