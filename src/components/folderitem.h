#ifndef FOLDERITEM_H
#define FOLDERITEM_H

#include <Kommodity/GIO/FileInfo>
#include <QIcon>
#include <QString>

class FolderItem
{
public:
    FolderItem(const Kommodity::GIO::FileInfo &newFileInfo);
    FolderItem(const FolderItem &next);

    QString displayName;
    QIcon icon;
    Kommodity::GIO::FileInfo *fileInfo;
};

#endif // FOLDERITEM_H
