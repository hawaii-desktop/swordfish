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

#include "filemanageradaptor.h"
#include "mainwindow.h"

FileManagerAdaptor::FileManagerAdaptor(QObject *parent)
    : QDBusAbstractAdaptor(parent)
{
    setAutoRelaySignals(true);
}

FileManagerAdaptor::~FileManagerAdaptor()
{
}

void FileManagerAdaptor::ShowFolders(const QStringList &URIs, const QString &StartupId)
{
    for (int i = 0; i < URIs.length(); i++) {
        MainWindow *window = new MainWindow();
        window->show();
    }
}

void FileManagerAdaptor::ShowItemProperties(const QStringList &URIs, const QString &StartupId)
{
}

void FileManagerAdaptor::ShowItems(const QStringList &URIs, const QString &StartupId)
{
}

#include "moc_filemanageradaptor.cpp"
