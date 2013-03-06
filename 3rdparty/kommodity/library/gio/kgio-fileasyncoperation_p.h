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


#ifndef KGIOFILEASYNCOPERATION_H_
#define KGIOFILEASYNCOPERATION_H_

#include <QtCore/QObject>
#include <QtCore/QString>
#include <QtCore/QByteArray>

#include <Kommodity/GIO/WrappedObject>
#include <Kommodity/GIO/Error>
#include <Kommodity/GIO/FileInputStream>
#include <Kommodity/GIO/FileOutputStream>
#include <Kommodity/GIO/FileEnumerator>
#include <Kommodity/GIO/FileInfo>
#include <Kommodity/GIO/Cancellable>
#include <Kommodity/GIO/File>
#include <Kommodity/GIO/MountOperation>
#include <Kommodity/GIO/Mount>

namespace Kommodity {
namespace GIO {

class Q_DECL_EXPORT FileAsyncOperation : public QObject
{
    Q_OBJECT

public:
    FileAsyncOperation();
    FileAsyncOperation( const File & file );
    FileAsyncOperation( const File & file, const Cancellable * cancellable, bool deleteAfterEmit = false );

    virtual ~FileAsyncOperation();

    void read();
    void appendTo(File::CreateFlags flags);
    void create(File::CreateFlags flags);
    void replace(const QString & etag, bool makeBackup, File::CreateFlags flags);

    void queryInfo(const char *attributes, File::QueryInfoFlags flags);
    void queryFilesystemInfo(const char *attributes);

    void findEnclosingMount();
    void enumerateChildren(const char *attributes, File::QueryInfoFlags flags);
    void setDisplayName(const QString & displayName);

    void copy(const File & destination, File::CopyFlags flags);

    void setAttributes(const FileInfo & info, File::QueryInfoFlags flags);

    void mountMountable(MountOperation & mountOperation, Mount::MountFlags flags = Mount::MountNone);

    void unmountMountable(Mount::UnmountFlags flags);

    void ejectMountable(Mount::UnmountFlags flags);

    void mountEnclosingVolume(MountOperation & mount_operation, Mount::MountFlags flags = Mount::MountNone);

    void loadContentsAsync();

    void loadPartialContents();

    void replaceContents(const QByteArray contents,
                         const QString & etag,
                         bool makeBackup,
                         File::CreateFlags flags);

    File getFile() const;

    bool m_deleteAfterEmit;

Q_SIGNALS:

    void inputStreamReady(const File & file, FileInputStream * inputStream, const Error & error);
    void outputStreamReady(const File & file, FileOutputStream * outputStream, const Error & error);

    void queryInfoReady(const File & file, const FileInfo & fileInfo, const Error & error);
    void queryFilesystemInfoReady(const File & file, const FileInfo & fileInfo, const Error & error);
    void findEnclosingMountReady(const File & file, const Mount & mount, const Error & error);
    void enumerateChildrenReady(const File & file, const FileEnumerator & fileEnumerator, const Error & error);
    void setDisplayNameReady(const File & file, const File& result, const Error & error);
    void copyProgress(qint64 currentNumBytes, qint64 totalNumBytes);
    void copyReady(const File & file, const Error & error);
    void setAttributesReady(const File & file, const FileInfo & fileInfo, const Error & error);
    void mountMountableReady(const File & file, const File& result, const Error & error);
    void unmountMountableReady(const File & file, const Error & error);
    void ejectMountableReady(const File & file, const Error & error);
    void mountEnclosingVolumeReady(const File & file, const Error & error);
    void loadContentsReady(const File & file, const QByteArray & contents, const QString & etag, const Error & error);
    void loadPartialContentsReady(const File & file, const char * content, qint64 size, const QString & etag, const Error & error);
    void loadPartialContentsReadMore(const File & file, const char * content, qint64 currentSize);
    void replaceContentsReady(const File & file, const QString & newEtag, const Error & error);


public: // internal:

    void emitInputStreamReady(const File & file, FileInputStream * inputStream, const Error & error);
    void emitOutputStreamReady(const File & file, FileOutputStream * outputStream, const Error & error);

    void emitQueryInfoReady(const File & file, const FileInfo & fileInfo, const Error & error);
    void emitQueryFilesystemInfoReady(const File & file, const FileInfo & fileInfo, const Error & error);
    void emitFindEnclosingMountReady(const File & file, const Mount & mount, const Error & error);
    void emitEnumerateChildrenReady(const File & file, const FileEnumerator & fileEnumerator, const Error & error);
    void emitSetDisplayNameReady(const File & file, const File& result, const Error & error);
    void emitCopyProgress(qint64 currentNumBytes, qint64 totalNumBytes);
    void emitCopyReady(const File & file, const Error & error);
    void emitSetAttributesReady(const File & file, const FileInfo & fileInfo, const Error & error);
    void emitMountMountableReady(const File & file, const File& result, const Error & error);
    void emitUnmountMountableReady(const File & file, const Error & error);
    void emitEjectMountableReady(const File & file, const Error & error);
    void emitMountEnclosingVolumeReady(const File & file, const Error & error);
    void emitLoadContentsReady(const File & file, const QByteArray & contents, const QString & etag, const Error & error);
    void emitLoadPartialContentsReadMore(const File & file, const char * content, qint64 currentSize);
    void emitLoadPartialContentsReady(const File & file, const char * content, qint64 size, const QString & etag, const Error & error);
    void emitReplaceContentsReady(const File & file, const QString & newEtag, const Error & error);


private:

    class Private;
    friend class Private;
    Private * d;
};

}}

#endif /*KGIOFILEASYNCOPERATION_H_*/
