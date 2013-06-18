/****************************************************************************
 * This file is part of Swordfish.
 *
 * Copyright (C) 2010-2013 Pier Luigi Fiorini <pierluigi.fiorini@gmail.com>
 *
 * Author(s):
 *    Pier Luigi Fiorini
 *
 * $BEGIN_LICENSE:LGPL2.1+$
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 2.1 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * $END_LICENSE$
 ***************************************************************************/

#ifndef FOLDERMODEL_H
#define FOLDERMODEL_H

#include <QObject>
#include <QAbstractListModel>
#include <QDir>
#include <QUrl>
#include <QModelIndex>
#include <QDebug>
#include <Kommodity/GIO/FileInfo>
#include <Kommodity/GIO/FileMonitor>
#include <Kommodity/GIO/File>
#include <Kommodity/GIO/FileEnumerator>
#include <Kommodity/GIO/MountOperation>
#include <QStandardPaths>

using namespace Kommodity::GIO;

#include "folderitem.h"

class FolderModel : public QAbstractListModel
{
    Q_OBJECT
    Q_PROPERTY(QUrl folder READ folder WRITE setFolder NOTIFY folderChanged)
public:
    FolderModel(QObject* parent = 0);
    
    enum Role {
        FileNameRole = Qt::UserRole + 1,
        FileInfoRole,
        FileOwnerRole,
        FileIconSourceRole
    };
    enum ColumnId {
        ColumnFileName,
        ColumnFileType,
        ColumnFileSize,
        ColumnFileMTime,
        ColumnFileOwner,
        NumOfColumns
    };

    QUrl folder() const;
    void setFolder(const QUrl &uri);

    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    int columnCount(const QModelIndex &parent) const;

    QHash<int, QByteArray> roleNames() const;

    QVariant data(const QModelIndex &index, int role) const;
    QVariant headerData(int section,
                          Qt::Orientation orientation,
                          int role) const;

    QModelIndex index(int row,
                      int column,
                      const QModelIndex &parent = QModelIndex()) const;
    QModelIndex parent(const QModelIndex &index) const;

    FileInfo *fileInfoFromIndex(const QModelIndex &index) const;

signals:
    void folderChanged();

public Q_SLOTS:
    void changed(FileMonitor *monitor,
                 const File &file,
                 const File &otherFile,
                 FileMonitor::FileMonitorEvent eventType);

protected:
    QList<FolderItem>::Iterator findItemByUrl(const QUrl &url, const int &row);
    QList<FolderItem>::Iterator findItemByName(const QString &name, const int &row);

    FolderItem *itemFromIndex(const QModelIndex &index) const;

private:
    QUrl m_uri;
    FileMonitor *m_folderMonitor;
    MountOperation m_mountOperation;
    QList<FolderItem> m_folderItems;

    void listFolderContents(const File &folder);
    void removeAll();
    void insertFiles(const QList<FileInfo> &fileInfoList);

private Q_SLOTS:
    void askPassword(MountOperation *op, const QString &message,
                     const QString &defaultUser, const QString &defaultDomain,
                     MountOperation::AskPasswordFlags flags);
    void askQuestion(MountOperation *op, const QString &message,
                     const QList<QString> &choices);
    void replied(MountOperation *op, MountOperation::MountOperationResult abort);
    void mountEnclosingVolumeReady(const File &folder, const Error &error);
};

#endif // FOLDERMODEL_H
