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

#include <QDir>
#include <QFileInfo>

#include "directorysizejob.h"

DirectorySizeJob::DirectorySizeJob(const QString &path, QObject *parent)
    : VJob(parent)
    , m_path(path)
    , m_totalSize(0)
    , m_totalFiles(0)
{
}

quint64 DirectorySizeJob::totalSize() const
{
    return m_totalSize;
}

quint64 DirectorySizeJob::totalFiles() const
{
    return m_totalFiles;
}

void DirectorySizeJob::start()
{
    processDirectory(QDir(m_path));
    emitResult();
}

void DirectorySizeJob::processDirectory(const QDir &dir)
{
    foreach(QFileInfo info, dir.entryInfoList(QDir::AllEntries | QDir::NoDotAndDotDot)) {
        // Recursive call when we encounter a directory
        if (info.isDir())
            processDirectory(QDir(info.filePath()));
        else {
            // Don't sum file size when it's a symlink but we want to count it as a file
            if (!info.isSymLink())
                m_totalSize += info.size();

            m_totalFiles++;
        }
    }
}

#include "moc_directorysizejob.cpp"
