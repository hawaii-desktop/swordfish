/****************************************************************************
 * This file is part of Swordfish
 *
 * Copyright (c) 2010-2012 Pier Luigi Fiorini
 *
 * Author(s):
 *    Pier Luigi Fiorini <pierluigi.fiorini@gmail.com>
 *
 * Swordfish is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Swordfish is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Swordfish.  If not, see <http://www.gnu.org/licenses/>.
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