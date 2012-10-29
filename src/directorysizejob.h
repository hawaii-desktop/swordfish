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
