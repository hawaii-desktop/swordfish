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

#include <QDebug>
#include <QDir>

#include "directorycopyjob.h"
#include "directorysizejob.h"
#include "filecopyjob.h"

DirectoryCopyJob::DirectoryCopyJob(const QString &source, const QString &destination,
                                   QObject *parent)
    : VCompositeJob(parent)
    , m_isMove(false)
    , m_source(source)
    , m_destination(destination)
{
    // First calculate the source directory size
    VJob *job = new DirectorySizeJob(source);
    connect(job, SIGNAL(result(VJob *)), this, SLOT(sizeCalculated(VJob *)));
    addSubjob(job);
}

QString DirectoryCopyJob::source() const
{
    return m_source;
}

QString DirectoryCopyJob::destination() const
{
    return m_destination;
}

void DirectoryCopyJob::start()
{
    foreach(VJob * job, subjobs())
    job->start();
}

void DirectoryCopyJob::sizeCalculated(VJob *job)
{
    bool isValid = job->error() == VJob::NoError;
    job->deleteLater();

    DirectorySizeJob *dirSizeJob = qobject_cast<DirectorySizeJob *>(job);
    qDebug() << "*****************" << isValid << dirSizeJob->totalSize() << dirSizeJob->totalFiles();

    if (isValid)
        copyDirectory(QDir(m_source), QDir(m_destination));
}

void DirectoryCopyJob::copyDirectory(const QDir &src, const QDir &dst)
{
    foreach(const QFileInfo & info, src.entryInfoList(QDir::AllEntries | QDir::NoDotAndDotDot)) {
        QString sourcePath = info.filePath();
        QString destPath = dst.path() + QDir::separator() + info.fileName();

        if (info.isDir()) {
            // Copy directories recursively
            qDebug() << "Copy directory" << sourcePath << destPath;
            copyDirectory(QDir(sourcePath), QDir(destPath));
        } else {
            // Copy a file
            qDebug() << "Copy file" << sourcePath << destPath;
            VJob *job = new FileCopyJob(sourcePath, destPath);
            addSubjob(job);
        }
    }
}

#include "moc_directorycopyjob.cpp"
