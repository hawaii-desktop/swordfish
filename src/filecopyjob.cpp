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

#include <QTimer>
#include <QFile>

#include "filecopyjob.h"

FileCopyJob::FileCopyJob(const QString &source, const QString &destination,
                         QObject *parent)
    : VJob(parent)
    , m_isMove(false)
    , m_source(source)
    , m_destination(destination)
{
}

QString FileCopyJob::source() const
{
    return m_source;
}

QString FileCopyJob::destination() const
{
    return m_destination;
}

void FileCopyJob::start()
{
    QTimer::singleShot(0, this, SLOT(copyFile()));
}

void FileCopyJob::copyFile()
{
    // Open the source file for reading
    QFile sourceFile(m_source);
    if (!sourceFile.open(QFile::ReadOnly)) {
        setError(CopySourceError);
        setErrorText(sourceFile.errorString());
        emitResult();
        return;
    }

    // Open destination file for writing
    QFile destinationFile(m_destination);
    if (!destinationFile.open(QFile::WriteOnly)) {
        setError(CopyDestinationError);
        setErrorText(destinationFile.errorString());
        emitResult();
        return;
    }

    qint64 total = sourceFile.size();
    qint64 bytesCopied = 0;

    while (!sourceFile.atEnd()) {
        // Read 8KB from the source file at once
        QByteArray data = sourceFile.read(8192);
        bytesCopied += data.size();

        // Write to the destination
        destinationFile.write(data);

        // Emit the percentage
        emitPercent(bytesCopied, total);
    }

    if (m_isMove) {
        // Remove the source file
        if (!sourceFile.remove()) {
            // The source file could not be removed hence we remove the destination
            // file and trigger an error
            destinationFile.remove();
            setError(MoveSourceError);
            setErrorText(sourceFile.errorString());
            return;
        }
    }
}

#include "moc_filecopyjob.cpp"
