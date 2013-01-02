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

#ifndef FILEMANAGERADAPTOR_H
#define FILEMANAGERADAPTOR_H

#include <QObject>
#include <QDBusAbstractAdaptor>

/*
 * This is the org.freedesktop.FileManager1 adaptor that implements
 * the File Manager DBus Interface from Freedesktop.
 *
 * http://www.freedesktop.org/wiki/Specifications/file-manager-interface
 */

class FileManagerAdaptor: public QDBusAbstractAdaptor
{
    Q_OBJECT
    Q_CLASSINFO("D-Bus Interface", "org.freedesktop.FileManager1")
public:
    explicit FileManager1Adaptor(QObject *parent);
    virtual ~FileManager1Adaptor();

public slots:
    void ShowFolders(const QStringList &URIs, const QString &StartupId);
    void ShowItemProperties(const QStringList &URIs, const QString &StartupId);
    void ShowItems(const QStringList &URIs, const QString &StartupId);
};

#endif // FILEMANAGERADAPTOR_H
