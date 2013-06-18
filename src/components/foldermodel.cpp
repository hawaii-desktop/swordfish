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

#include "foldermodel.h"

FolderModel::FolderModel(QObject* parent)
    : QAbstractListModel(parent)
{
    setFolder(QStandardPaths::writableLocation(QStandardPaths::HomeLocation));
}

void FolderModel::insertFiles(const int &row, const QList<FileInfo> &fileInfoList)
{
    int files = fileInfoList.length();
    beginInsertRows(QModelIndex(), row, row + files - 1);
    for (int i = 0; i < files; i++) {
        FolderItem item(fileInfoList.at(i));
        m_folderItems.append(item);
    }
    endInsertRows();
}

int FolderModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;
    return m_folderItems.size();
}

int FolderModel::columnCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;
    return NumOfColumns;
}

FolderItem *FolderModel::itemFromIndex(const QModelIndex &index) const
{
    return reinterpret_cast<FolderItem *>(index.internalPointer());
}

QModelIndex FolderModel::index(int row, int column, const QModelIndex &parent) const
{
    if (row < 0 || row >= m_folderItems.size() || column < 0 || column >= NumOfColumns)
        return QModelIndex();
    const FolderItem& folderItem = m_folderItems.at(row);
    return createIndex(row, column, (void *)&folderItem);

}

QHash<int, QByteArray> FolderModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[FileNameRole] = "fileName";
    roles[FileInfoRole] = "fileInfo";
    roles[FileIconRole] = "fileIcon";
    return roles;
}

QVariant FolderModel::data(const QModelIndex &index, int role = Qt::DisplayRole) const
{
    if (!index.isValid() || index.row() > m_folderItems.size() || index.column() >= NumOfColumns)
        return QVariant();
    FolderItem *folderitem = itemFromIndex(index);
    FileInfo *fileinfo = folderitem->fileInfo;

    switch (role) {

    /*case Qt::ToolTipRole :
        return QVariant(folderitem->displayName);

    case Qt::DisplayRole : {

        switch (index.column()) {
        case ColumnFileName :
            return QVariant(folderitem->displayName);

        case ColumnFileType :
            return QVariant(fileinfo->getContentType());

        case ColumnFileMTime :
            return QVariant(fileinfo->getModificationTime().toString());

        case ColumnFileSize : {
            return QVariant(fileinfo->getSize());
            break;
        }

        case ColumnFileOwner : {
            return QVariant(fileinfo->getAttributeAsString("owner::user"));
            break;
        }
        }
    }
    case Qt::DecorationRole : {
        if (index.column() == 0)
            return QVariant(folderitem->icon);
    }*/

    case FileIconRole:
        return QVariant(folderitem->icon);

    case FileOwnerRole:
        return QVariant(fileinfo->getAttributeAsString("owner::user"));

    case FileNameRole:
        return QVariant(folderitem->displayName);

    case FileInfoRole :
        return qVariantFromValue((void *)fileinfo);

    }
    return QVariant();
}

QModelIndex FolderModel::parent(const QModelIndex &index) const
{
    return QModelIndex();
}

QVariant FolderModel::headerData(int section,
                                 Qt::Orientation orientation,
                                 int role) const
{
    if (role == Qt::DisplayRole) {
        if (orientation == Qt::Horizontal) {
            QString text;
            switch (section) {
            case ColumnFileName :
                text = tr("Name");
                break;

            case ColumnFileType :
                text = tr("Type");
                break;
            case ColumnFileSize :
                text = tr("Size");
                break;
            case ColumnFileMTime :
                text = tr("Modified");
                break;
            case ColumnFileOwner:
                text = tr("Owner");
                break;
            }
            return QVariant(text);
        }
    }
    return QVariant();
}

FileInfo *FolderModel::fileInfoFromIndex(const QModelIndex &index) const
{
    FolderItem *folderItem = itemFromIndex(index);
    if (folderItem)
        return folderItem->fileInfo;
    return 0;
}

void FolderModel::removeAll()
{
    if (m_folderItems.size() == 0)
        return;

    beginRemoveRows(QModelIndex(), 0, m_folderItems.size() - 1);
    m_folderItems.clear();
    endRemoveRows();
}

QUrl FolderModel::folder() const
{
    return m_uri;
}

void FolderModel::setFolder(const QUrl &uri)
{
    // Just return if we already have this URI open
    if (uri == m_uri)
        return;

    Error error;

    // Open folder
    File folder(uri);
    FileInfo fileInfo = folder.queryInfo("*", File::QueryInfoNorm, error);
    if (error.hasError() && !error.isIOError(IOError::NotMounted)) {
        qWarning("Error opening %s: %s",
                 qPrintable(uri.toString(QUrl::FullyEncoded)),
                 qPrintable(error.getMessage()));
        return;
    }

    // Enumerate folder contents
    FileEnumerator fileEnumerator = folder->enumerateChildren(
        "*", File::QueryInfoNorm, error);
    if (error.hasError()) {
        qWarning("Couldn't enumerate %s: %s",
                 qPrintable(uri.toString(QUrl::FullyEncoded)),
                 qPrintable(error.getMessage()));
        return;
    }

    // Save FileInfo objects
    QList<FileInfo> fileInfoList;
    for (;;) {
        fileInfo = fileEnumerator.nextFile(error);
        if (error.hasError()) {
            qWarning("Couldn't advance enumerator %s: %s",
                     qPrintable(uri.toString(QUrl::FullyEncoded)),
                     qPrintable(error.getMessage()));
            return;
        }

        if (fileInfo.isNull())
            break;

        fileInfoList.append(fileInfo);
    }

    // Close enumerator
    fileEnumerator.close(error);
    if (error.hasError())
        qWarning("Couldn't close enumerator on %s: %s",
                 qPrintable(uri.toString(QUrl::FullyEncoded)),
                 qPrintable(error.getMessage()));
        return;
    }

    // Populate model
    removeAll();
    insertFiles(fileInfoList);

    m_folderMonitor = new FileMonitor(folder.monitorDirectory(folder.MonitorNorm, error, 0));
    if (error.hasError()) {
        qWarning("Failed to monitor %s: %s",
                 qPrintable(uri.toString(QUrl::FullyEncoded)),
                 qPrintable(error.getMessage()));
        return;
    }

    connect(m_folderMonitor, SIGNAL(changed(FileMonitor *, File, File, FileMonitorEvent)),
            this, SLOT(changed(FileMonitor *, File, File, FileMonitor::FileMonitorEvent)));

    // Change URI
    m_uri = uri;
    emit folderChanged();
}

void changed(FileMonitor *monitor,
                  const File &file,
                  const File &otherFile,
                  FileMonitor::FileMonitorEvent eventType)
{
    if (eventType == monitor->Changed) {
        //Do something
    }
    else if (eventType == monitor->Deleted) {
        //Do Something Else
    }
    else if (eventType == monitor->AttributeChanged) {
        //Do Something Else
    }
}

#include "moc_foldermodel.cpp"
