#include "folderitem.h"

FolderItem::FolderItem(const Kommodity::GIO::FileInfo &newFileInfo)
{
    fileInfo = new Kommodity::GIO::FileInfo(newFileInfo);
    displayName = fileInfo->getDisplayName();
    icon = fileInfo->getIcon();
}

FolderItem::FolderItem(const FolderItem &next)
{
    fileInfo = new Kommodity::GIO::FileInfo(*next.fileInfo);
    displayName = fileInfo->getDisplayName();
    icon = next.icon;
}
