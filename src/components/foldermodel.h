#ifndef FOLDERMODEL_H
#define FOLDERMODEL_H

#include <QObject>
#include <QAbstractListModel>
#include <QDir>
#include <QUrl>
#include <QModelIndex>
#include <Kommodity/GIO/FileInfo>

#include "folderitem.h"

class FolderModel : public QAbstractListModel
{
    Q_OBJECT
public:
    FolderModel(QObject* parent = 0);
    
    enum Role {
        FileInfoRole = Qt::UserRole
    };
    enum ColumnId {
        ColumnFileName,
        ColumnFileType,
        ColumnFileSize,
        ColumnFileMTime,
        ColumnFileOwner,
        NumOfColumns
    };

    QDir *folder() const
    {
        return m_folder;
    }

    QUrl url() const
    {
        if(m_folder->exists())
        return QUrl::fromUserInput(m_folder->absolutePath());
    }

    void setFolder(const QDir &newFolder);
    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    int columnCount(const QModelIndex &parent) const;

    QVariant data(const QModelIndex &index, int role) const;
    QVariant headerData(int section,
                          Qt::Orientation orientation,
                          int role) const;

    QModelIndex index(int row,
                      int column,
                      const QModelIndex &parent = QModelIndex()) const;
    QModelIndex parent(const QModelIndex &index) const;

    Kommodity::GIO::FileInfo *fileInfoFromIndex(const QModelIndex &index) const;

protected:
    void insertFiles(const int &row,const QList<Kommodity::GIO::FileInfo *> &fileInfoList);
    void removeAll();

    QList<FolderItem>::Iterator findItemByUrl(const QUrl &url, const int &row);
    QList<FolderItem>::Iterator findItemByName(const QString &name, const int &row);

    FolderItem *itemFromIndex(const QModelIndex &index) const;

private:
    QDir *m_folder;
    QList<FolderItem> m_folderItems;
    //QHash<FolderItem, int> m_folderItems;
};

#endif // FOLDERMODEL_H
