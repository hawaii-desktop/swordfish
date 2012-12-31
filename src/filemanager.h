/****************************************************************************
 * This file is part of Swordfish.
 *
 * Copyright (C) 2010-2012 Pier Luigi Fiorini <pierluigi.fiorini@gmail.com>
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

#ifndef FILEMANAGER_H
#define FILEMANAGER_H

#include <QObject>

#include <VibeCore/VJob>

class QUrl;

class FileManager : public QObject
{
    Q_OBJECT
public:
    FileManager(QObject *parent = 0);

    void copy(const QUrl &src, const QUrl &dstDir);
    void move(const QUrl &src, const QUrl &dstDir);

signals:
    void copyFinished(const QUrl &from, const QUrl &to);
    void moveFinished(const QUrl &from, const QUrl &to);
    void copyError(const QUrl &from, const QUrl &to, const QString &error);
    void moveError(const QUrl &from, const QUrl &to, const QString &error);

private slots:
    void handleResult(VJob *job);

private:
    void copyOrMove(bool move, const QUrl &src, const QUrl &dstDir);
};

#endif // FILEMANAGER_H
