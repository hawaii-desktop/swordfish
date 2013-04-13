#include "foldermodel.h"

FolderModel::FolderModel(QObject* parent)
{
}

void FolderModel::insertFiles(const int &row, const QList<Kommodity::GIO::FileInfo *> &fileInfoList)
{
    int files = fileInfoList.length();
    beginInsertRows(QModelIndex(), row, row + files - 1);
    for (int i = 0; i < files; i++) {
        FolderItem newFolderItem(*fileInfoList.at(i));
        m_folderItems.append(newFolderItem);
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

QVariant FolderModel::data(const QModelIndex &index, int role = Qt::DisplayRole) const
{
    if (!index.isValid() || index.row() > m_folderItems.size() || index.column() >= NumOfColumns)
        return QVariant();
    FolderItem *folderitem = itemFromIndex(index);
    Kommodity::GIO::FileInfo *fileinfo = folderitem->fileInfo;

    switch (role) {
    case Qt::ToolTipRole :
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
            //TODO: Show owner name
            break;
        }
        }
    }
    case Qt::DecorationRole : {
        if (index.column() == 0)
            return QVariant(folderitem->icon);
    }

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

Kommodity::GIO::FileInfo *FolderModel::fileInfoFromIndex(const QModelIndex &index) const
{
    FolderItem *folderItem = itemFromIndex(index);
    if (folderItem)
        return folderItem->fileInfo;
    else
        return 0;
}

void FolderModel::removeAll()
{
    if (m_folderItems.empty())
        return;
    beginRemoveRows(QModelIndex(), 0, m_folderItems.size() - 1);
    m_folderItems.clear();
    endRemoveRows();
}

void FolderModel::setFolder(const QDir &newFolder)
{
}

