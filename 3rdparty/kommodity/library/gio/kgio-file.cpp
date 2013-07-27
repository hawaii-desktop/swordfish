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


#include <QtCore/QDate>
#include <QtCore/QFile>
#include <QtCore/QString>

#include <glib.h>
#include <gio/gio.h>

#include "kgio-file.h"
#include "kgio-mount.h"
#include "kgio-drive.h"
#include "kgio-volume.h"
#include "kgio-wrappedobjectutils_p.h"
#include "kgio-fileasyncoperation_p.h"

namespace Kommodity {
namespace GIO {

class File::Private : public QSharedData
{
public:
    Private()
    {
    };
};


File::File() : d(0)
{
}

File::File( const File& other) : SharedWrappedObject(other), d(0)
{
}

File::File( const QByteArray & path ) : d(0)
{
    WO::setGFile(this, g_file_new_for_path(path.data()));
}

File::File( const QUrl & url ) : d(0)
{
    WO::set<File,GFile>(this, g_file_new_for_uri(url.toString().toUtf8().data()));
}

File::~File()
{
}

File &File::operator=(const File &other)
{
    SharedWrappedObject::operator=(other);
    d = other.d;
    return *this;
}

File File::fromCommandlineArg(const char *arg)
{
    File file;
    WO::setGFile(&file, g_file_new_for_commandline_arg(arg));
    return file;
}

File File::fromParseName(const QString & parseName)
{
    File file;
    WO::set<File,GFile>(&file, g_file_parse_name(parseName.toUtf8().data()));
    return file;
}

QByteArray File::getBasename() const
{
    char * basename = g_file_get_basename(WO::get<File,GFile>(this));
    if (!basename) return QByteArray();
    QByteArray result = QByteArray(basename);
    g_free(basename);
    return result;
}

QByteArray File::getPath() const
{
    char * path = g_file_get_path(WO::getGFile(this));
    if (!path) return QByteArray();
    QByteArray result = QByteArray(path);
    g_free(path);
    return result;
}

QUrl File::getUri() const
{
    char * uri = g_file_get_uri(WO::getGFile(this));
    if (!uri) return QUrl();
    QUrl result = QUrl(QString::fromUtf8(uri));
    g_free(uri);
    return result;
}

QString File::getParseName() const
{
    char * parse_name = g_file_get_parse_name(WO::getGFile(this));
    if (!parse_name) return QString();
    QString result = QString::fromUtf8(parse_name);
    g_free(parse_name);
    return result;
}

File File::getParent() const
{
    File result;
    WO::setGFile(&result, g_file_get_parent (WO::getGFile(this)));
    return result;
}

File File::getChild(const QByteArray & name) const
{
    File result;
    WO::setGFile(&result, g_file_get_child (WO::getGFile(this), name.data()));
    return result;
}


File File::getChildForDisplayName(const QString & name, Error & error) const
{
    File result;

    WO::setGFile(&result, g_file_get_child_for_display_name(WO::getGFile(this),
            name.toUtf8().data(),
            WO::getGErrorPtr(&error) ));
    return result;
}

bool File::hasPrefix(const File & prefix) const
{
    return g_file_has_prefix(
            WO::getGFile(this),
            WO::getGFile(&prefix));
}

QByteArray File::getRelativePath(const File & descendant) const
{
    return QByteArray(g_file_get_relative_path(
            WO::getGFile(this),
            WO::getGFile(&descendant)));
}

File File::resolveRelativePath(const QByteArray & relativePath) const
{
    File file;
    WO::setGFile(&file, g_file_resolve_relative_path(
            WO::getGFile(this),
            relativePath.data()));
    return file;
}

bool File::isNative() const
{
    return g_file_is_native(WO::getGFile(this));
}

bool File::hasUriScheme(const QString & uriScheme) const
{
    return g_file_has_uri_scheme(
            WO::getGFile(this),
            uriScheme.toUtf8().data());
}

QString File::getUriScheme() const
{
    return QString::fromUtf8(g_file_get_uri_scheme(WO::getGFile(this)));
}

FileInputStream * File::read(Error & error, const Cancellable * cancellable)
{

    FileInputStream * fileInputStream = new FileInputStream;

    GFileInputStream* gFileInputStream = g_file_read(
            WO::getGFile(this),
            WO::getGCancellable(cancellable),
            WO::getGErrorPtr(&error) );

    WO::setGFileInputStream(fileInputStream, gFileInputStream);
    return fileInputStream;
}

void File::readAsync(
        QObject *receiver,
        const char * callbackSlot,
        const Cancellable * cancellable)
{
    FileAsyncOperation * asyncOperation
        = new FileAsyncOperation(*this, cancellable, true);

    asyncOperation->connect(
            asyncOperation, SIGNAL(inputStreamReady(const File &, FileInputStream *, const Error &)),
            receiver, callbackSlot);

    asyncOperation->read();
}

FileOutputStream * File::create(CreateFlags flags, Error & error, const Cancellable * cancellable)
{
    FileOutputStream * fileOutputStream = new FileOutputStream;

    GFileOutputStream* gFileOutputStream = g_file_create(
            WO::getGFile(this),
            (GFileCreateFlags) flags,
            WO::getGCancellable(cancellable),
            WO::getGErrorPtr(&error) );

    WO::setGFileOutputStream(fileOutputStream, gFileOutputStream);
    return fileOutputStream;
}

void File::createAsync(File::CreateFlags flags,
        QObject *receiver,
        const char * callbackSlot,
        const Cancellable * cancellable)
{
    FileAsyncOperation * asyncOperation
        = new FileAsyncOperation(*this, cancellable, true);

    asyncOperation->connect(
            asyncOperation, SIGNAL(outputStreamReady(const File &, FileOutputStream *, const Error &)),
            receiver, callbackSlot);

    asyncOperation->create(flags);
}

FileOutputStream * File::appendTo(CreateFlags flags, Error & error, const Cancellable * cancellable)
{
    FileOutputStream * fileOutputStream = new FileOutputStream;

    GFileOutputStream* gFileOutputStream = g_file_append_to(
            WO::getGFile(this),
            (GFileCreateFlags) flags,
            WO::getGCancellable(cancellable),
            WO::getGErrorPtr(&error) );

    WO::setGFileOutputStream(fileOutputStream, gFileOutputStream);
    return fileOutputStream;

}

void File::appendToAsync(File::CreateFlags flags,
         QObject *receiver,
         const char * callbackSlot,
         const Cancellable * cancellable)
{
    FileAsyncOperation * asyncOperation
        = new FileAsyncOperation(*this, cancellable, true);

    asyncOperation->connect(
            asyncOperation, SIGNAL(outputStreamReady(const File &, FileOutputStream *, const Error &)),
            receiver, callbackSlot);

    asyncOperation->appendTo(flags);
}

FileOutputStream * File::replace(const QString & etag, bool makeBackup, CreateFlags flags, Error & error, const Cancellable * cancellable)
{
    FileOutputStream * fileOutputStream = new FileOutputStream;

    GFileOutputStream* gFileOutputStream = g_file_replace(
            WO::getGFile(this),
            etag.toUtf8().data(),
            makeBackup,
            (GFileCreateFlags) flags,
            WO::getGCancellable(cancellable),
            WO::getGErrorPtr(&error) );

    WO::setGFileOutputStream(fileOutputStream, gFileOutputStream);
    return fileOutputStream;

}

void File::replaceAsync(const QString & etag, bool makeBackup, File::CreateFlags flags,
        QObject *receiver,
        const char * callbackSlot,
        const Cancellable * cancellable)
{
    FileAsyncOperation * asyncOperation
        = new FileAsyncOperation(*this, cancellable, true);

    asyncOperation->connect(
            asyncOperation, SIGNAL(outputStreamReady(const File &, FileOutputStream *, const Error &)),
            receiver, callbackSlot);

    asyncOperation->replace(etag, makeBackup, flags);
}

AppInfo File::queryDefaultHandler(Error & error, const Cancellable * cancellable)
{
    AppInfo appInfo;
    WO::setGAppInfo(&appInfo, g_file_query_default_handler(
            WO::getGFile(this),
            WO::getGCancellable(cancellable),
            WO::getGErrorPtr(&error)));
    return appInfo;
}


FileInfo File::queryInfo(const char *attributes,
                   QueryInfoFlags flags,
                   Error & error,
                   const Cancellable * cancellable)
{
    FileInfo fileInfo;
    WO::setGFileInfo(&fileInfo,
            g_file_query_info(WO::getGFile(this),
                              attributes,
                              (GFileQueryInfoFlags)flags,
                              WO::getGCancellable(cancellable),
                              WO::getGErrorPtr(&error)));
    return fileInfo;
}

void File::queryInfoAsync(const char*attributes,
        QueryInfoFlags flags,
        QObject *receiver,
        const char * callbackSlot,
        const Cancellable * cancellable)
{
    FileAsyncOperation * asyncOperation
        = new FileAsyncOperation(*this, cancellable, true);

    asyncOperation->connect(
            asyncOperation, SIGNAL(queryInfoReady(const File &, const FileInfo &, const Error &)),
            receiver, callbackSlot);

    asyncOperation->queryInfo(attributes, flags);
}

bool File::queryExists(const Cancellable * cancellable)
{
    return g_file_query_exists(WO::getGFile(this), WO::getGCancellable(cancellable));
}

FileInfo File::queryFilesystemInfo(const char *attributes,
                   Error & error,
                   const Cancellable * cancellable)
{
    FileInfo fileInfo;
    WO::setGFileInfo(&fileInfo,
            g_file_query_filesystem_info(WO::getGFile(this),
                                         attributes,
                                         WO::getGCancellable(cancellable),
                                         WO::getGErrorPtr(&error)));
    return fileInfo;
}

void File::queryFilesystemInfoAsync(const char *attributes,
        QObject *receiver,
        const char * callbackSlot,
        const Cancellable * cancellable)
{
    FileAsyncOperation * asyncOperation
        = new FileAsyncOperation(*this, cancellable, true);

    asyncOperation->connect(
            asyncOperation, SIGNAL(queryFilesystemInfoReady(const File &, const FileInfo &, const Error &)),
            receiver, callbackSlot);

    asyncOperation->queryFilesystemInfo(attributes);
}

Mount * File::findEnclosingMount(Error & error, const Cancellable * cancellable)
{
    GMount * gMount = g_file_find_enclosing_mount(
            WO::getGFile(this),
            WO::getGCancellable(cancellable),
            WO::getGErrorPtr(&error));
    if (!gMount) return 0;

    Mount * mount = new Mount;
    WO::setGMount(mount, gMount);
    return mount;
}

void File::findEnclosingMountAsync(
        QObject *receiver,
        const char * callbackSlot,
        const Cancellable * cancellable)
{
    FileAsyncOperation * asyncOperation
        = new FileAsyncOperation(*this, cancellable, true);

    asyncOperation->connect(
            asyncOperation, SIGNAL(findEnclosingMountReady(const File & , const Mount & , const Error & )),
            receiver, callbackSlot);

    asyncOperation->findEnclosingMount();
}

FileEnumerator File::enumerateChildren(const char *attributes,
                   QueryInfoFlags flags,
                   Error & error,
                   const Cancellable * cancellable)
{
    FileEnumerator fileEnumerator;
    WO::setGFileEnumerator(&fileEnumerator,
            g_file_enumerate_children(
                    WO::getGFile(this),
                    attributes,
                    (GFileQueryInfoFlags)flags,
                    WO::getGCancellable(cancellable),
                    WO::getGErrorPtr(&error)));
    return fileEnumerator;
}

void File::enumerateChildrenAsync(const char *attributes,
        File::QueryInfoFlags flags,
        QObject *receiver,
        const char * callbackSlot,
        const Cancellable * cancellable)
{
    FileAsyncOperation * asyncOperation
        = new FileAsyncOperation(*this, cancellable, true);

    asyncOperation->connect(
            asyncOperation, SIGNAL(enumerateChildrenReady(const File & , const FileEnumerator & , const Error & )),
            receiver, callbackSlot);

    asyncOperation->enumerateChildren(attributes, flags);
}

File File::setDisplayName(const QString & displayName,
                   Error & error,
                   const Cancellable * cancellable)
{
    File result;
    WO::setGFile(&result,
         g_file_set_display_name(
                 WO::getGFile(this),
                 displayName.toUtf8().data(),
                 WO::getGCancellable(cancellable),
                 WO::getGErrorPtr(&error)));
    return result;
}

void File::setDisplayNameAsync(const QString & displayName,
        QObject *receiver,
        const char * callbackSlot,
        const Cancellable * cancellable)
{
    FileAsyncOperation * asyncOperation
        = new FileAsyncOperation(*this, cancellable, true);

    asyncOperation->connect(
            asyncOperation, SIGNAL(setDisplayNameReady(const File & , const File& , const Error & )),
            receiver, callbackSlot);

    asyncOperation->setDisplayName(displayName);
}

bool File::deleteFile(Error &error, const Cancellable * cancellable)
{
    return g_file_delete(WO::getGFile(this),
            WO::getGCancellable(cancellable),
            WO::getGErrorPtr(&error));
}

bool File::trash(Error &error, const Cancellable * cancellable)
{
    return g_file_trash(WO::getGFile(this),
            WO::getGCancellable(cancellable),
            WO::getGErrorPtr(&error));
}

bool File::copy(const File & destination,
          CopyFlags flags,
          Error & error,
          const Cancellable * cancellable)
{
    return g_file_copy(WO::getGFile(this),
             WO::getGFile(&destination),
             (GFileCopyFlags)flags,
             WO::getGCancellable(cancellable),
             NULL /* GFileProgressCallback progress_callback */,
             NULL /* gpointer progress_callback_data */,
             WO::getGErrorPtr(&error));
}

void File::copyAsync(const File & destination,
        File::CopyFlags flags,
        QObject *progressReceiver,
        const char * progressSlot,
        QObject *receiver,
        const char * callbackSlot,
        const Cancellable * cancellable)
{
    FileAsyncOperation * asyncOperation
        = new FileAsyncOperation(*this, cancellable, true);

    asyncOperation->connect(
            asyncOperation, SIGNAL(copyProgress(qint64 , qint64 )),
            progressReceiver, progressSlot);

    asyncOperation->connect(
            asyncOperation, SIGNAL(copyReady(const File & , const Error & )),
            receiver, callbackSlot);

    asyncOperation->copy(destination, flags);

}

bool File::move(const File & destination,
          CopyFlags flags,
          Error & error,
          const Cancellable * cancellable)
{
    return g_file_move(WO::getGFile(this),
            WO::getGFile(&destination),
            (GFileCopyFlags)flags,
            WO::getGCancellable(cancellable),
            NULL /* GFileProgressCallback progress_callback */,
            NULL /* gpointer progress_callback_data */,
            WO::getGErrorPtr(&error));
}

bool File::makeDirectory(Error & error, const Cancellable * cancellable)
{
    return g_file_make_directory(WO::getGFile(this),
            WO::getGCancellable(cancellable),
            WO::getGErrorPtr(&error));
}

bool File::makeSymbolicLink(const QString & symlinkValue,
            Error & error,
            const Cancellable * cancellable)
{
    return g_file_make_symbolic_link(
            WO::getGFile(this),
            symlinkValue.toUtf8().data(),
            WO::getGCancellable(cancellable),
            WO::getGErrorPtr(&error));
}

QList<FileAttributeInfo> File::querySettableAttributes(
                  Error & error,
                  const Cancellable * cancellable)
{
    QList<FileAttributeInfo> result;

    GFileAttributeInfoList* attributeInfoList = g_file_query_settable_attributes
            (WO::getGFile(this),
             WO::getGCancellable(cancellable),
             WO::getGErrorPtr(&error));

    for(int i=0; i<attributeInfoList->n_infos; i++)
    {
        GFileAttributeInfo * info = &attributeInfoList->infos[i];
        FileAttributeInfo attributeInfo;
        attributeInfo.name = QString(info->name);
        attributeInfo.type = (FileAttributeType)info->type;
        attributeInfo.flags = (FileAttributeInfoFlags)info->flags;
        result.append(attributeInfo);
    }
    return result;
}

QList<FileAttributeInfo> File::queryWritableNamespaces(
                  Error & error,
                  const Cancellable * cancellable)
{
    QList<FileAttributeInfo> result;

    GFileAttributeInfoList* attributeInfoList = g_file_query_writable_namespaces
                (WO::getGFile(this),
                WO::getGCancellable(cancellable),
                WO::getGErrorPtr(&error));

    for(int i=0; i<attributeInfoList->n_infos; i++)
    {
        GFileAttributeInfo * info = &attributeInfoList->infos[i];
        FileAttributeInfo attributeInfo;
        attributeInfo.name = QString(info->name);
        attributeInfo.type = (FileAttributeType)info->type;
        attributeInfo.flags = (FileAttributeInfoFlags)info->flags;
        result.append(attributeInfo);
    }
    return result;

}

//    bool setAttribute(const char *attribute,
//                      FileAttributeType type,
//                      gpointer value_p,
//                      GFileQueryInfoFlags flags,
//                      Cancellable & cancellable,
//                      Error & error);

bool File::setAttributesFromInfo(FileInfo & info,
                           QueryInfoFlags flags,
                           Error & error,
                           const Cancellable * cancellable)
{
    return g_file_set_attributes_from_info(WO::getGFile(this),
                           WO::getGFileInfo(&info),
                           (GFileQueryInfoFlags)flags,
                           WO::getGCancellable(cancellable),
                           WO::getGErrorPtr(&error));
}

bool File::setAttributeString(const char *attribute,
                        const QString & value,
                        QueryInfoFlags flags,
                        Error & error,
                        const Cancellable * cancellable)
{
    return g_file_set_attribute_string(WO::getGFile(this),
                           attribute,
                           value.toUtf8().data(),
                           (GFileQueryInfoFlags)flags,
                           WO::getGCancellable(cancellable),
                           WO::getGErrorPtr(&error));
}

bool File::setAttributeByteString(const char *attribute,
                            const QByteArray & value,
                            QueryInfoFlags flags,
                            Error & error,
                            const Cancellable *cancellable)
{

    QByteArray byteArray(value);
    byteArray.append('\0'); // must be \0 terminated

    return  g_file_set_attribute_byte_string(
            WO::getGFile(this),
            attribute,
            byteArray.data(),
            (GFileQueryInfoFlags)flags,
            WO::getGCancellable(cancellable),
            WO::getGErrorPtr(&error));
}

bool File::setAttributeUInt32(const char *attribute,
                        quint32 value,
                        QueryInfoFlags flags,
                        Error & error,
                        const Cancellable *cancellable)
{
    return g_file_set_attribute_uint32(WO::getGFile(this),
            attribute,
            (guint32)value,
            (GFileQueryInfoFlags)flags,
            WO::getGCancellable(cancellable),
            WO::getGErrorPtr(&error));
}

bool File::setAttributeInt32(const char *attribute,
                       qint32 value,
                       QueryInfoFlags flags,
                       Error & error,
                       const Cancellable * cancellable)
{
    return g_file_set_attribute_int32(WO::getGFile(this),
            attribute,
            (gint32)value,
            (GFileQueryInfoFlags)flags,
            WO::getGCancellable(cancellable),
            WO::getGErrorPtr(&error));
}

bool File::setAttributeUint64(const char *attribute,
                           quint64 value,
                           QueryInfoFlags flags,
                           Error & error,
                           const Cancellable * cancellable)
{
    return g_file_set_attribute_uint64(WO::getGFile(this),
            attribute,
            (guint64)value,
            (GFileQueryInfoFlags)flags,
            WO::getGCancellable(cancellable),
            WO::getGErrorPtr(&error));

}

bool File::setAttributeInt64(const char *attribute,
                           qint64 value,
                           QueryInfoFlags flags,
                           Error & error,
                           const Cancellable * cancellable)
{
    return g_file_set_attribute_int64(WO::getGFile(this),
            attribute,
            (gint32)value,
            (GFileQueryInfoFlags)flags,
            WO::getGCancellable(cancellable),
            WO::getGErrorPtr(&error));
}

FileMonitor File::monitorDirectory(MonitorFlags flags,
                             Error & error,
                             const Cancellable * cancellable)
{
    FileMonitor fileMonitor;
    WO::setGFileMonitor(&fileMonitor,
           g_file_monitor_directory(WO::getGFile(this),
                   (GFileMonitorFlags) flags,
                   WO::getGCancellable(cancellable),
                   WO::getGErrorPtr(&error)));
    return fileMonitor;
}

FileMonitor File::monitorFile(MonitorFlags flags,
                        Error & error,
                        const Cancellable * cancellable)
{
    FileMonitor fileMonitor;
    WO::setGFileMonitor(&fileMonitor,
            g_file_monitor_file(WO::getGFile(this),
                    (GFileMonitorFlags) flags,
                    WO::getGCancellable(cancellable),
                    WO::getGErrorPtr(&error)));
    return fileMonitor;
}

QByteArray File::loadContents(
                        QString &etagOut,
                        Error & error,
                        const Cancellable * cancellable) const
{
    char *contents = NULL;
    gsize length=0;
    char *etag_out = NULL;

    bool success = g_file_load_contents(WO::getGFile(this),
            WO::getGCancellable(cancellable),
            &contents,
            &length,
            &etag_out,
            WO::getGErrorPtr(&error));

    if (!success) return QByteArray();

    QByteArray contentsByteArray(contents, length);
    g_free(contents);

    if (etag_out)
    {
        etagOut = QString(etag_out);
        g_free(etag_out);
    }
    return contentsByteArray;
}

bool File::replaceContents(const QByteArray & contents,
                     const QString & etag,
                     bool makeBackup,
                     CreateFlags flags,
                     QString & newEtag,
                     Error &error,
                     const Cancellable * cancellable)
{
    char *new_etag =  NULL;
    gboolean success = g_file_replace_contents(WO::getGFile(this),
                             contents.data(),
                             contents.size(),
                             etag.toUtf8().data(),
                             makeBackup,
                             (GFileCreateFlags)flags,
                             &new_etag,
                             WO::getGCancellable(cancellable),
                             WO::getGErrorPtr(&error));
    if (new_etag)
    {
        newEtag = QString(new_etag);
        g_free(new_etag);
    }
    return success;
}

bool File::copyAttributes(File & destination,
                    CopyFlags flags,
                    Error & error,
                    const Cancellable * cancellable)
{
    return g_file_copy_attributes(WO::getGFile(this),
            WO::getGFile(&destination),
            (GFileCopyFlags) flags,
            WO::getGCancellable(cancellable),
            WO::getGErrorPtr(&error));
}

void File::setAttributesAsync(const FileInfo & info, File::QueryInfoFlags flags,
        QObject *receiver,
        const char * callbackSlot,
        const Cancellable * cancellable)
{
    FileAsyncOperation * asyncOperation
        = new FileAsyncOperation(*this, cancellable, true);

    asyncOperation->connect(
            asyncOperation, SIGNAL(setAttributesReady(const File & , const FileInfo & , const Error & )),
            receiver, callbackSlot);

    asyncOperation->setAttributes(info, flags);
}

void File::mountMountableAsync(MountOperation & mountOperation, Mount::MountFlags flags,
        QObject *receiver,
        const char * callbackSlot,
        const Cancellable * cancellable)
{
    FileAsyncOperation * asyncOperation
        = new FileAsyncOperation(*this, cancellable, true);

    asyncOperation->connect(
            asyncOperation, SIGNAL(mountMountableReady(const File & , const File& , const Error & )),
            receiver, callbackSlot);

    asyncOperation->mountMountable(mountOperation, flags);
}

void File::unmountMountableAsync(Mount::UnmountFlags flags,
        QObject *receiver,
        const char * callbackSlot,
        const Cancellable * cancellable)
{
    FileAsyncOperation * asyncOperation
        = new FileAsyncOperation(*this, cancellable, true);

    asyncOperation->connect(
            asyncOperation, SIGNAL(unmountMountableReady(const File & , const Error & )),
            receiver, callbackSlot);

    asyncOperation->unmountMountable(flags);
}

void File::ejectMountableAsync(Mount::UnmountFlags flags,
        QObject *receiver,
        const char * callbackSlot,
        const Cancellable * cancellable)
{
    FileAsyncOperation * asyncOperation
        = new FileAsyncOperation(*this, cancellable, true);

    asyncOperation->connect(
            asyncOperation, SIGNAL(ejectMountableReady(const File & , const Error & )),
            receiver, callbackSlot);

    asyncOperation->ejectMountable(flags);
}

void File::mountEnclosingVolumeAsync(MountOperation & mount_operation,
        Mount::MountFlags flags,
        QObject *receiver,
        const char * callbackSlot,
        const Cancellable * cancellable)
{
    FileAsyncOperation * asyncOperation
        = new FileAsyncOperation(*this, cancellable, true);

    asyncOperation->connect(
            asyncOperation, SIGNAL(mountEnclosingVolumeReady(const File &, const Error &)),
            receiver, callbackSlot);

    asyncOperation->mountEnclosingVolume(mount_operation,flags);
}

void File::loadContentsAsync(QObject *receiver,
        const char * callbackSlot,
        const Cancellable * cancellable)
{
    FileAsyncOperation * asyncOperation
        = new FileAsyncOperation(*this, cancellable, true);

    asyncOperation->connect(
            asyncOperation, SIGNAL(loadContentsReady(const File &, const QByteArray &, const QString &, const Error & )),
            receiver, callbackSlot);

    asyncOperation->loadContentsAsync();
}

//void File::loadPartialContentsAsync(QObject *receiver,
//        const char * callbackSlot,
//        const Cancellable * cancellable)
//{
//    FileAsyncOperation * asyncOperation
//        = new FileAsyncOperation(*this, cancellable, true);
//
//    asyncOperation->connect(
//            asyncOperation, SIGNAL(loadPartialContentsReady(const File & file, const char * content, qint64 size, const QString & etag, const Error & error)),
//            receiver, callbackSlot);
//
//    asyncOperation->loadPartialContents();
//}

void File::replaceContentsAsync(const QByteArray contents,
        const QString & etag,
        bool makeBackup,
        File::CreateFlags flags,
        QObject *receiver,
        const char * callbackSlot,
        const Cancellable * cancellable)
{
    FileAsyncOperation * asyncOperation
        = new FileAsyncOperation(*this, cancellable, true);

    asyncOperation->connect(
            asyncOperation, SIGNAL(replaceContentsReady(const File & , const QString & , const Error & )),
            receiver, callbackSlot);

    asyncOperation->replaceContents(contents, etag, makeBackup, flags);
}


}}

//#include "moc_kgio-file.cpp"

