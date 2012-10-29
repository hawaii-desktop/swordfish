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
