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

#ifndef DIRECTORYSIZEJOB_H
#define DIRECTORYSIZEJOB_H

#include <VibeCore/VJob>

class DirectorySizeJob : public VJob
{
    Q_OBJECT
public:
    explicit DirectorySizeJob(const QString &path, QObject *parent = 0);

    quint64 totalSize() const;
    quint64 totalFiles() const;

    void start();

private:
    QString m_path;
    quint64 m_totalSize;
    quint64 m_totalFiles;

    void processDirectory(const QDir &dir);
};

#endif // DIRECTORYSIZEJOB_H
