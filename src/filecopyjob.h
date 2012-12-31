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

#ifndef FILECOPYJOB_H
#define FILECOPYJOB_H

#include <VibeCore/VJob>

class FileCopyJob : public VJob
{
    Q_OBJECT
public:
    enum {
        CopySourceError = UserDefinedError,
        CopyDestinationError,
        MoveSourceError
    };

    explicit FileCopyJob(const QString &source, const QString &destination,
                         QObject *parent = 0);

    QString source() const;
    QString destination() const;

    void start();

protected:
    bool m_isMove;

private slots:
    void copyFile();

private:
    QString m_source;
    QString m_destination;
};

#endif // FILECOPYJOB_H
