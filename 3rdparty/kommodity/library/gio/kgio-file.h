/*
   Copyright (C) 2008 Norbert Frese <nf2 at scheinwelt at>

   This library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Library General Public
   License (LGPL) as published by the Free Software Foundation;
   either version 2 of the License, or (at your option) any later
   version.

   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Library General Public License for more details.

   You should have received a copy of the GNU Library General Public License
   along with this library; see the file COPYING.LIB.  If not, write to
   the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.
*/


#ifndef KGIOFILE_H_
#define KGIOFILE_H_


#include <QtCore/QObject>
#include <QtCore/QString>
#include <QtCore/QUrl>
#include <QtCore/QSharedData>

#include <Kommodity/GIO/SharedWrappedObject>
#include <Kommodity/GIO/Error>
#include <Kommodity/GIO/FileInputStream>
#include <Kommodity/GIO/FileOutputStream>
#include <Kommodity/GIO/AppInfo>
#include <Kommodity/GIO/Cancellable>
#include <Kommodity/GIO/FileInfo>
#include <Kommodity/GIO/FileEnumerator>
#include <Kommodity/GIO/FileMonitor>
#include <Kommodity/GIO/MountOperation>
#include <Kommodity/GIO/Mount>
#include <Kommodity/GIO/FileAttribute>

namespace Kommodity {
namespace GIO {

class Mount;

class Q_DECL_EXPORT File : public virtual SharedWrappedObject
{

public:

    File();
    File(const File&);
    File(const QByteArray & path );
    File(const QUrl & url );
    virtual ~File();

    File &operator=(const File &other);

    enum QueryInfoFlags {
        QueryInfoNorm = 0,
        QueryInfoNofollowSymlinks = (1<<0)
    };

    enum CreateFlags {
        CreateNorm = 0,
        CreatePrivate = (1<<0)
    };

    enum CopyFlags {
        CopyNorm = 0,
        CopyOverwrite = (1<<0),
        CopyBackup = (1<<1),
        CopyNofollowSymlinks = (1<<2),
        CopyAllMetaData = (1<<3),
        CopyNoFallBackForMove = (1<<4)
    };

    enum MonitorFlags {
        MonitorNorm = 0,
        MonitorWatchMounts = (1<<0)
    };

    static File fromCommandlineArg(const char *arg);
    static File fromParseName(const QString & parseName);

    QByteArray getBasename() const;
    QByteArray getPath() const;
    QUrl getUri() const;
    QString getParseName() const;

    File getParent() const;
    File getChild(const QByteArray & name ) const;
    File getChildForDisplayName(const QString & name, Error & error) const;

    bool hasPrefix(const File & prefix) const;
    QByteArray getRelativePath(const File & descendant) const;
    File resolveRelativePath(const QByteArray & relativePath) const;
    bool isNative() const;
    bool hasUriScheme(const QString & uriScheme) const;
    QString getUriScheme() const;

    FileInputStream * read(Error & error, const Cancellable * cancellable = 0);

    void readAsync(
            QObject *receiver,
            const char * callbackSlot,
            const Cancellable * cancellable = 0);

    FileOutputStream * create(CreateFlags flags, Error & error, const Cancellable * cancellable = 0);

    void createAsync(File::CreateFlags flags,
            QObject *receiver,
            const char * callbackSlot,
            const Cancellable * cancellable = 0);

    FileOutputStream * appendTo(CreateFlags flags, Error & error, const Cancellable * cancellable = 0);

    void appendToAsync(File::CreateFlags flags,
            QObject *receiver,
            const char * callbackSlot,
            const Cancellable * cancellable = 0);

    FileOutputStream * replace(const QString & etag, bool makeBackup, CreateFlags flags, Error & error, const Cancellable * cancellable = 0);

    void replaceAsync(const QString & etag, bool makeBackup, File::CreateFlags flags,
            QObject *receiver,
            const char * callbackSlot,
            const Cancellable * cancellable = 0);

    AppInfo queryDefaultHandler(Error & error, const Cancellable * cancellable = 0);

    FileInfo queryInfo(const char *attributes,
                      QueryInfoFlags flags,
                      Error & error,
                      const Cancellable * cancellable = 0);

    void queryInfoAsync(const char*attributes,
            QueryInfoFlags flags,
            QObject *receiver,
            const char * callbackSlot,
            const Cancellable * cancellable = 0);

    bool queryExists(const Cancellable * cancellable = 0);

    FileInfo queryFilesystemInfo(const char *attributes,
                      Error & error,
                      const Cancellable * cancellable = 0);

    void queryFilesystemInfoAsync(const char *attributes,
            QObject *receiver,
            const char * callbackSlot,
            const Cancellable * cancellable = 0);

    Mount * findEnclosingMount(Error & error, const Cancellable * cancellable = 0);

    void findEnclosingMountAsync(
            QObject *receiver,
            const char * callbackSlot,
            const Cancellable * cancellable = 0);

    FileEnumerator enumerateChildren(const char *attributes,
                      QueryInfoFlags flags,
                      Error & error,
                      const Cancellable * cancellable = 0);

    void enumerateChildrenAsync(const char *attributes,
            File::QueryInfoFlags flags,
            QObject *receiver,
            const char * callbackSlot,
            const Cancellable * cancellable = 0);

    File setDisplayName(const QString & display_name,
                      Error & error,
                      const Cancellable * cancellable = 0);

    void setDisplayNameAsync(const QString & displayName,
            QObject *receiver,
            const char * callbackSlot,
            const Cancellable * cancellable = 0);

    bool deleteFile(Error &error, const Cancellable * cancellable = 0);

    bool trash(Error &error, const Cancellable * cancellable = 0);

    bool copy(const File & destination,
                      CopyFlags flags,
                      Error & error,
                      const Cancellable * cancellable = 0);

    void copyAsync(const File & destination, File::CopyFlags flags,
            QObject *progressReceiver,
            const char * progressSlot,
            QObject *receiver,
            const char * callbackSlot,
            const Cancellable * cancellable = 0);

    bool move(const File & destination,
                      CopyFlags flags,
                      Error & error,
                      const Cancellable * cancellable = 0);

    bool makeDirectory(Error & error, const Cancellable * cancellable = 0);

    bool makeSymbolicLink(
                      const QString & symlinkValue,
                      Error & error,
                      const Cancellable * cancellable = 0);

    QList<FileAttributeInfo> querySettableAttributes(
                      Error & error,
                      const Cancellable * cancellable = 0);

    QList<FileAttributeInfo> queryWritableNamespaces(
                      Error & error,
                      const Cancellable * cancellable = 0);

    bool setAttributesFromInfo(FileInfo & info,
                      QueryInfoFlags flags,
                      Error & error,
                      const Cancellable * cancellable = 0);

    bool setAttributeString(const char *attribute,
                      const QString & value,
                      QueryInfoFlags flags,
                      Error & error,
                      const Cancellable * cancellable = 0);

    bool setAttributeByteString(const char *attribute,
                      const QByteArray & value,
                      QueryInfoFlags flags,
                      Error & error,
                      const Cancellable * cancellable = 0);

    bool setAttributeUInt32(const char *attribute,
                      quint32 value,
                      QueryInfoFlags flags,
                      Error & error,
                      const Cancellable * cancellable = 0);

    bool setAttributeInt32(const char *attribute,
                      qint32 value,
                      QueryInfoFlags flags,
                      Error & error,
                      const Cancellable * cancellable = 0);

    bool setAttributeUint64(const char *attribute,
                      quint64 value,
                      QueryInfoFlags flags,
                      Error & error,
                      const Cancellable * cancellable = 0);

    bool setAttributeInt64(const char *attribute,
                      qint64 value,
                      QueryInfoFlags flags,
                      Error & error,
                      const Cancellable * cancellable = 0);

    FileMonitor monitorDirectory(MonitorFlags flags,
                      Error & error,
                      const Cancellable * cancellable = 0);

    FileMonitor monitorFile(MonitorFlags flags,
                      Error & error,
                      const Cancellable * cancellable = 0);

    QByteArray loadContents(QString &etagOut,
                      Error & error,
                      const Cancellable * cancellable = 0) const;

    bool replaceContents(const QByteArray & contents,
                      const QString & etag,
                      bool makeBackup,
                      CreateFlags flags,
                      QString & newEtag,
                      Error &error,
                      const Cancellable * cancellable = 0);

    bool copyAttributes(File & destination,
                      CopyFlags flags,
                      Error & error,
                      const Cancellable * cancellable = 0);

    void setAttributesAsync(const FileInfo & info, File::QueryInfoFlags flags,
            QObject *receiver,
            const char * callbackSlot,
            const Cancellable * cancellable = 0);

    void mountMountableAsync(MountOperation & mountOperation, Mount::MountFlags flags,
            QObject *receiver,
            const char * callbackSlot,
            const Cancellable * cancellable = 0);

    void unmountMountableAsync(Mount::UnmountFlags flags,
            QObject *receiver,
            const char * callbackSlot,
            const Cancellable * cancellable = 0);

    void ejectMountableAsync(Mount::UnmountFlags flags,
            QObject *receiver,
            const char * callbackSlot,
            const Cancellable * cancellable = 0);

    void mountEnclosingVolumeAsync(MountOperation & mount_operation,
            Mount::MountFlags flags,
            QObject *receiver,
            const char * callbackSlot,
            const Cancellable * cancellable = 0);

    void loadContentsAsync(QObject *receiver,
            const char * callbackSlot,
            const Cancellable * cancellable = 0);

//    void loadPartialContentsAsync(QObject *receiver,
//            const char * callbackSlot,
//            const Cancellable * cancellable = 0);

    void replaceContentsAsync(const QByteArray contents,
            const QString & etag,
            bool makeBackup,
            File::CreateFlags flags,
            QObject *receiver,
            const char * callbackSlot,
            const Cancellable * cancellable = 0);

private:

    class Private;
    friend class Private;
    QSharedDataPointer<Private> d;
};

}}  // namespace

#endif /*KGIOFILE_H_*/
