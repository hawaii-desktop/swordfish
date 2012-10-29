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
