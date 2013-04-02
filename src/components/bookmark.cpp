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

#include "bookmark.h"

Bookmark::~Bookmark()
{
}

Bookmark::Bookmark(const QString &text,const QString &icon,const QUrl &url)
: PlacesItem(icon,text,url)
{
    if(icon.isEmpty())
        m_icon=QIcon::fromTheme("folder");
    else
        m_icon=QIcon::fromTheme(icon);

    if(url.isEmpty())
        m_url.setPath(QDir::homePath());
    else
        m_url.setPath(url.toString());

    if(text.isEmpty())
        m_text = "title";
    else
        m_text = text;
}

void Bookmark::setIcon(const QString &icon)
{
    m_icon = QIcon::fromTheme(icon);
}

void Bookmark::setUrl(const QUrl &url)
{
    m_url = url;
}

void Bookmark::setText(const QString &text)
{
    m_text = text;
}


