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

#include <QDir>
#include <QUrl>

#include "filemanager.h"
#include "fileviewcontroller.h"
#include "filecopyjob.h"
#include "filemovejob.h"
#include "directorycopyjob.h"
#include "directorymovejob.h"

FileManager::FileManager(QObject *parent)
    : QObject(parent)
{
}

void FileManager::copy(const QUrl &src, const QUrl &dstDir)
{
    copyOrMove(false, src, dstDir);
}

void FileManager::move(const QUrl &src, const QUrl &dstDir)
{
    copyOrMove(true, src, dstDir);
}

void FileManager::handleResult(VJob *job)
{
}

void FileManager::copyOrMove(bool move, const QUrl &src, const QUrl &dstDir)
{
    // Only local files are allowed
    if (!src.isLocalFile())
        return;

    QString srcFilePath = src.toLocalFile();
    QString srcFileName = QFileInfo(srcFilePath).fileName();
    QUrl dstFilePath = QUrl::fromLocalFile(
                           QDir(dstDir.toLocalFile()).absoluteFilePath(
                               srcFileName));

    VJob *job = 0;
    if (QFileInfo(srcFilePath).isDir()) {
        if (move)
            job = new DirectoryMoveJob(srcFilePath, dstFilePath.toLocalFile());
        else
            job = new DirectoryCopyJob(srcFilePath, dstFilePath.toLocalFile());
    } else {
        if (move)
            job = new FileMoveJob(srcFilePath, dstFilePath.toLocalFile());
        else
            job = new FileCopyJob(srcFilePath, dstFilePath.toLocalFile());
    }
    connect(job, SIGNAL(result(VJob *)), this, SLOT(handleResult(VJob *)));
    job->start();
}

#include "moc_filemanager.cpp"
