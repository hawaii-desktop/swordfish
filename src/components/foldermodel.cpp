#include "foldermodel.h"

FolderModel::FolderModel()
{
}

void FolderModel::insertFiles(const int &row, const QList<Kommodity::GIO::FileInfo *> &fileInfoList)
{
    int files = fileInfoList.length();
    beginInsertRows(QModelIndex(), row, row + files - 1);
    endInsertRows();
}
