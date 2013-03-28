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
{
}

PlacesModel::~PlacesModel()
{
}

int PlacesModel::addBookmark(const QString &icon,const QString &text,const QUrl &url)
{
    for(int i=0;i<m_bookmarks.length();i++)
    {
        if(m_bookmarks.at(i)->text()==text && m_bookmarks.at(i)->url()==url)
           return -1;
    }
    m_bookmarks.append(new Bookmark(text,icon,url));
    return 0;
}

void PlacesModel::removeBookmark(const QString &text,const QUrl &url)
{
    for(int i=0;i<m_bookmarks.length();i++)
        if(m_bookmarks.at(i)->text()==text && m_bookmarks.at(i)->url()==url)
            m_bookmarks.removeAt(i);
}
