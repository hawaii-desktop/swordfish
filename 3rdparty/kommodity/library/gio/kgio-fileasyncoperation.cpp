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

#include <QtCore/QByteRef>
#include <QtCore/QDate>
#include <QtCore/QVarLengthArray>
#include <QtCore/QCoreApplication>
#include <QtCore/QRegExp>
#include <QtCore/QFile>
#include <QtCore/QString>
#include <QtCore/QPointer>

#include <glib.h>
#include <gio/gio.h>

#include "kgio-error.h"
#include "kgio-file.h"
#include "kgio-fileasyncoperation_p.h"
#include "kgio-inputstream.h"
#include "kgio-outputstream.h"
#include "kgio-wrappedobjectutils_p.h"

namespace Kommodity {
namespace GIO {

/* ------- callbacks -------- */

static void  _async_read_ready_callback(GObject *source_object, GAsyncResult *result, gpointer user_data)
{
    Error error;

    GFileInputStream* gFileInputStream = g_file_read_finish(
            G_FILE(source_object),
            result,
            WO::getGErrorPtr(&error));

    FileInputStream * fileInputStream;
    if (!error.hasError())
    {
        fileInputStream = new FileInputStream;
        WO::setGFileInputStream(fileInputStream, gFileInputStream);
    }

    QPointer<FileAsyncOperation> * fileAsyncOperationQPointer = (QPointer<FileAsyncOperation> *)user_data;
    if (!fileAsyncOperationQPointer->isNull())
    {
        FileAsyncOperation * fileAsyncOperation = fileAsyncOperationQPointer->data();
        fileAsyncOperation->emitInputStreamReady(
            fileAsyncOperation->getFile(),
            fileInputStream,
            error);
    }
    delete fileAsyncOperationQPointer;
}

static void  _emit_file_output_stream_signal(GFileOutputStream* gFileOutputStream, const Error & error, gpointer user_data)
{
    FileOutputStream * fileOutputStream;
    if (!error.hasError())
    {
        fileOutputStream = new FileOutputStream;
        WO::setGFileOutputStream(fileOutputStream, gFileOutputStream);
    }

    QPointer<FileAsyncOperation> * fileAsyncOperationQPointer = (QPointer<FileAsyncOperation> *)user_data;
    if (!fileAsyncOperationQPointer->isNull())
    {
        FileAsyncOperation * fileAsyncOperation = fileAsyncOperationQPointer->data();
        fileAsyncOperation->emitOutputStreamReady(
            fileAsyncOperation->getFile(),
            fileOutputStream,
            error);
    }
    delete fileAsyncOperationQPointer;
}


static void  _append_to_ready_callback(GObject *source_object, GAsyncResult *result, gpointer user_data)
{
    Error error;

    GFileOutputStream* gFileOutputStream = g_file_append_to_finish(
            G_FILE(source_object),
            result,
            WO::getGErrorPtr(&error));

    _emit_file_output_stream_signal(gFileOutputStream, error, user_data);

}

static void  _create_ready_callback(GObject *source_object, GAsyncResult *result, gpointer user_data)
{
    Error error;

    GFileOutputStream* gFileOutputStream = g_file_create_finish(
            G_FILE(source_object),
            result,
            WO::getGErrorPtr(&error));

    _emit_file_output_stream_signal(gFileOutputStream, error, user_data);

}

static void  _replace_ready_callback(GObject *source_object, GAsyncResult *result, gpointer user_data)
{
    Error error;

    GFileOutputStream* gFileOutputStream = g_file_replace_finish(
            G_FILE(source_object),
            result,
            WO::getGErrorPtr(&error));

    _emit_file_output_stream_signal(gFileOutputStream, error, user_data);

}

//// -------------------------------------------

static void  _query_info_async_callback(GObject *source_object, GAsyncResult *result, gpointer user_data)
{
    Error error;

    QPointer<FileAsyncOperation> * fileAsyncOperationQPointer = (QPointer<FileAsyncOperation> *)user_data;
    if (!fileAsyncOperationQPointer->isNull())
    {
        FileAsyncOperation * fileAsyncOperation = fileAsyncOperationQPointer->data();

        FileInfo fileInfo;

        WO::setGFileInfo(&fileInfo, g_file_query_info_finish(
                G_FILE(source_object),
                result,
                WO::getGErrorPtr(&error)));

        fileAsyncOperation->emitQueryInfoReady(
            fileAsyncOperation->getFile(),
            fileInfo,
            error);
    }
    delete fileAsyncOperationQPointer;

}

static void _query_filesystem_info_async_callback(GObject *source_object, GAsyncResult *result, gpointer user_data)
{
    Error error;

    QPointer<FileAsyncOperation> * fileAsyncOperationQPointer = (QPointer<FileAsyncOperation> *)user_data;
    if (!fileAsyncOperationQPointer->isNull())
    {
        FileAsyncOperation * fileAsyncOperation = fileAsyncOperationQPointer->data();

        FileInfo fileInfo;

        WO::setGFileInfo(&fileInfo, g_file_query_filesystem_info_finish(
                G_FILE(source_object),
                result,
                WO::getGErrorPtr(&error)));

        fileAsyncOperation->emitQueryFilesystemInfoReady(
                fileAsyncOperation->getFile(),
                fileInfo,
                error);
    }

    if (fileAsyncOperationQPointer->data()->m_deleteAfterEmit)
    {
        delete fileAsyncOperationQPointer->data();
    }
    delete fileAsyncOperationQPointer;

}



static void _find_enclosing_mount_async_callback(GObject *source_object, GAsyncResult *result, gpointer user_data)
{
    Error error;

    QPointer<FileAsyncOperation> * fileAsyncOperationQPointer = (QPointer<FileAsyncOperation> *)user_data;
    if (!fileAsyncOperationQPointer->isNull())
    {
        FileAsyncOperation * fileAsyncOperation = fileAsyncOperationQPointer->data();

        Mount mount;

        WO::setGMount(&mount, g_file_find_enclosing_mount_finish(
                G_FILE(source_object),
                result,
                WO::getGErrorPtr(&error)));

        fileAsyncOperation->emitFindEnclosingMountReady(
            fileAsyncOperation->getFile(),
            mount,
            error);
    }
    delete fileAsyncOperationQPointer;

}


static void _enumerate_children_async_callback(GObject *source_object, GAsyncResult *result, gpointer user_data)
{
    Error error;

    QPointer<FileAsyncOperation> * fileAsyncOperationQPointer = (QPointer<FileAsyncOperation> *)user_data;
    if (!fileAsyncOperationQPointer->isNull())
    {
        FileAsyncOperation * fileAsyncOperation = fileAsyncOperationQPointer->data();

        FileEnumerator fileEnumerator;
        WO::setGFileEnumerator(&fileEnumerator, g_file_enumerate_children_finish(
                G_FILE(source_object),
                result,
                WO::getGErrorPtr(&error)));

        fileAsyncOperation->emitEnumerateChildrenReady(
                fileAsyncOperation->getFile(),
                fileEnumerator,
                error);
    }
    delete fileAsyncOperationQPointer;

}


static void _set_display_name_async_callback(GObject *source_object, GAsyncResult *result, gpointer user_data)
{
    Error error;

    QPointer<FileAsyncOperation> * fileAsyncOperationQPointer = (QPointer<FileAsyncOperation> *)user_data;
    if (!fileAsyncOperationQPointer->isNull())
    {
        FileAsyncOperation * fileAsyncOperation = fileAsyncOperationQPointer->data();

        File file;

        WO::setGFile(&file, g_file_set_display_name_finish(
                G_FILE(source_object),
                result,
                WO::getGErrorPtr(&error)));

        fileAsyncOperation->emitSetDisplayNameReady(
                fileAsyncOperation->getFile(),
                file,
                error);
    }
    delete fileAsyncOperationQPointer;

}


void _copy_progress_callback(goffset current_num_bytes,
            goffset total_num_bytes,
            gpointer user_data)
{
    QPointer<FileAsyncOperation> * fileAsyncOperationQPointer = (QPointer<FileAsyncOperation> *)user_data;
    if (!fileAsyncOperationQPointer->isNull())
    {
        FileAsyncOperation * fileAsyncOperation = fileAsyncOperationQPointer->data();
        fileAsyncOperation->emitCopyProgress(current_num_bytes, total_num_bytes);
    }
}

static void _copy_async_ready_callback(GObject *source_object, GAsyncResult *result, gpointer user_data)
{
    Error error;
    QPointer<FileAsyncOperation> * fileAsyncOperationQPointer = (QPointer<FileAsyncOperation> *)user_data;
    if (!fileAsyncOperationQPointer->isNull())
    {
        FileAsyncOperation * fileAsyncOperation = fileAsyncOperationQPointer->data();

        gboolean success = g_file_copy_finish(
                G_FILE(source_object),
                result,
                WO::getGErrorPtr(&error));

        fileAsyncOperation->emitCopyReady(
                fileAsyncOperation->getFile(),
                error);
    }
    delete fileAsyncOperationQPointer;

}

static void _set_attributes_async_ready_callback(GObject *source_object, GAsyncResult *result, gpointer user_data)
{
    Error error;
    QPointer<FileAsyncOperation> * fileAsyncOperationQPointer = (QPointer<FileAsyncOperation> *)user_data;
    if (!fileAsyncOperationQPointer->isNull())
    {
        FileAsyncOperation * fileAsyncOperation = fileAsyncOperationQPointer->data();

        GFileInfo * gFileInfo=NULL;
        FileInfo fileInfo;

        gboolean success = g_file_set_attributes_finish(
                G_FILE(source_object),
                result,
                &gFileInfo,
                WO::getGErrorPtr(&error));

        if (success)
        {
            WO::setGFileInfo(&fileInfo, gFileInfo);
        }

        fileAsyncOperation->emitSetAttributesReady(
                fileAsyncOperation->getFile(),
                fileInfo,
                error);

    }
    delete fileAsyncOperationQPointer;

}

static void _mount_mountable_ready_callback(GObject *source_object, GAsyncResult *result, gpointer user_data)
{
    Error error;
    QPointer<FileAsyncOperation> * fileAsyncOperationQPointer = (QPointer<FileAsyncOperation> *)user_data;
    if (!fileAsyncOperationQPointer->isNull())
    {
        FileAsyncOperation * fileAsyncOperation = fileAsyncOperationQPointer->data();

        File file;

        WO::setGFile(&file,
                g_file_mount_mountable_finish(
                        G_FILE(source_object),
                        result,
                        WO::getGErrorPtr(&error)));

        fileAsyncOperation->emitMountMountableReady(
                fileAsyncOperation->getFile(),
                file,
                error);
    }
    delete fileAsyncOperationQPointer;

}

static void _unmount_mountable_ready_callback(GObject *source_object, GAsyncResult *result, gpointer user_data)
{
    Error error;
    QPointer<FileAsyncOperation> * fileAsyncOperationQPointer = (QPointer<FileAsyncOperation> *)user_data;
    if (!fileAsyncOperationQPointer->isNull())
    {
        FileAsyncOperation * fileAsyncOperation = fileAsyncOperationQPointer->data();

        gboolean success = g_file_unmount_mountable_finish(
                G_FILE(source_object),
                result,
                WO::getGErrorPtr(&error));

        fileAsyncOperation->emitUnmountMountableReady(
                fileAsyncOperation->getFile(),
                error);
    }
    delete fileAsyncOperationQPointer;

}

static void _eject_mountable_ready_callback(GObject *source_object, GAsyncResult *result, gpointer user_data)
{
    Error error;
    QPointer<FileAsyncOperation> * fileAsyncOperationQPointer = (QPointer<FileAsyncOperation> *)user_data;
    if (!fileAsyncOperationQPointer->isNull())
    {
        FileAsyncOperation * fileAsyncOperation = fileAsyncOperationQPointer->data();

        gboolean success = g_file_eject_mountable_finish(
                G_FILE(source_object),
                result,
                WO::getGErrorPtr(&error));

        fileAsyncOperation->emitEjectMountableReady(
                fileAsyncOperation->getFile(),
                error);
    }
    delete fileAsyncOperationQPointer;

}

static void _mount_enclosing_volume_ready_callback(GObject *source_object, GAsyncResult *result, gpointer user_data)
{
    Error error;
    QPointer<FileAsyncOperation> * fileAsyncOperationQPointer = (QPointer<FileAsyncOperation> *)user_data;
    if (!fileAsyncOperationQPointer->isNull())
    {
        FileAsyncOperation * fileAsyncOperation = fileAsyncOperationQPointer->data();

        gboolean success = g_file_mount_enclosing_volume_finish(
                G_FILE(source_object),
                result,
                WO::getGErrorPtr(&error));

        fileAsyncOperation->emitMountEnclosingVolumeReady(
                fileAsyncOperation->getFile(),
                error);
    }
    delete fileAsyncOperationQPointer;

}

static void _load_contents_async_ready_callback(GObject *source_object, GAsyncResult *result, gpointer user_data)
{
    Error error;
    QPointer<FileAsyncOperation> * fileAsyncOperationQPointer = (QPointer<FileAsyncOperation> *)user_data;
    if (!fileAsyncOperationQPointer->isNull())
    {
        FileAsyncOperation * fileAsyncOperation = fileAsyncOperationQPointer->data();

        char *contents=NULL;
        gsize length;
        char *etag_out=NULL;

        QByteArray contentsOut;
        QString etagOut;

        gboolean success = g_file_load_contents_finish(
                G_FILE(source_object),
                result,
                &contents,
                &length,
                &etag_out,
                WO::getGErrorPtr(&error));

        if (success)
        {
            contentsOut = QByteArray(contents, length);
            etagOut = QString(etag_out);
        }

        g_free(contents);
        g_free(etag_out);

        fileAsyncOperation->emitLoadContentsReady(
                fileAsyncOperation->getFile(),
                contentsOut,
                etagOut,
                error);
    }
    delete fileAsyncOperationQPointer;
}

static gboolean _load_partial_contents_read_more_callback(
        const char *file_contents,
        goffset file_size,
        gpointer callback_data)
{
    QPointer<FileAsyncOperation> * fileAsyncOperationQPointer = (QPointer<FileAsyncOperation> *)callback_data;
    if (!fileAsyncOperationQPointer->isNull())
    {
        FileAsyncOperation * fileAsyncOperation = fileAsyncOperationQPointer->data();
        fileAsyncOperation->emitLoadPartialContentsReadMore(
                fileAsyncOperation->getFile(),
                file_contents,
                file_size);
        return true;
    }
    return false;
}

static void _load_partial_contents_async_ready_callback(GObject *source_object, GAsyncResult *result, gpointer user_data)
{
    Error error;
    QPointer<FileAsyncOperation> * fileAsyncOperationQPointer = (QPointer<FileAsyncOperation> *)user_data;
    if (!fileAsyncOperationQPointer->isNull())
    {
        FileAsyncOperation * fileAsyncOperation = fileAsyncOperationQPointer->data();

        char *contents;
        gsize length;
        char *etag_out;

        //QByteArray contentsOut;
        QString etagOut;

        gboolean success = g_file_load_partial_contents_finish (
                G_FILE(source_object),
                result,
                &contents,
                &length,
                &etag_out,
                WO::getGErrorPtr(&error));

        if (success)
        {
            //contentsOut = QByteArray(contents, length);
            etagOut = QString(etag_out);
        }

        fileAsyncOperation->emitLoadPartialContentsReady(
                fileAsyncOperation->getFile(),
                contents,
                length,
                etagOut,
                error);
    }
    delete fileAsyncOperationQPointer;

}

static void _replace_contents_async_ready_callback(GObject *source_object, GAsyncResult *result, gpointer user_data)
{
    Error error;
    QPointer<FileAsyncOperation> * fileAsyncOperationQPointer = (QPointer<FileAsyncOperation> *)user_data;
    if (!fileAsyncOperationQPointer->isNull())
    {
        FileAsyncOperation * fileAsyncOperation = fileAsyncOperationQPointer->data();

        char *new_etag;
        QString newETag;

        gboolean success = g_file_replace_contents_finish(
                G_FILE(source_object),
                result,
                &new_etag,
                WO::getGErrorPtr(&error));

        if (success)
        {
            newETag = QString(new_etag);
        }

        fileAsyncOperation->emitReplaceContentsReady(
            fileAsyncOperation->getFile(),
            newETag,
            error);

    }
    delete fileAsyncOperationQPointer;

}




class FileAsyncOperation::Private
{
public:
    Private() : m_ioPriority(G_PRIORITY_DEFAULT)
    {
    };

    Private(const File & file) : m_file(file), m_ioPriority(G_PRIORITY_DEFAULT)
    {
    };

    Private(const File & file, const Cancellable * cancellable)
            : m_file(file), m_gcancellable(WO::getGCancellable(cancellable))
            ,  m_ioPriority(G_PRIORITY_DEFAULT)
    {
    };

    ~Private()
    {
        if (m_gcancellable)
        {
            g_object_unref(m_gcancellable);
        }
    }


    void emitInputStreamReady(FileAsyncOperation * asyncOp, const File & file, FileInputStream * inputStream, const Error & error)
    {
        emit asyncOp->inputStreamReady(file, inputStream, error);
    }



    const File m_file;
    GCancellable * m_gcancellable;
    int m_ioPriority;

};

FileAsyncOperation::FileAsyncOperation() : d(new Private()), m_deleteAfterEmit(false)
{
}

//FileAsyncOperation::FileAsyncOperation(const FileAsyncOperation &other)
//    : d(other.d)
//{
//}

//FileAsyncOperation &FileAsyncOperation::operator=(const FileAsyncOperation &other)
//{
//    d = other.d;
//    return *this;
//}

FileAsyncOperation::FileAsyncOperation( const File & file ) : d(new Private(file)), m_deleteAfterEmit(false)
{
}

FileAsyncOperation::FileAsyncOperation( const File & file, const Cancellable * cancellable, bool deleteAfterEmit )
    : d(new Private(file, cancellable)) , m_deleteAfterEmit(deleteAfterEmit)
{
}

FileAsyncOperation::~FileAsyncOperation()
{
}

void FileAsyncOperation::read()
{
    void * thisQPointer = new QPointer<FileAsyncOperation>(this);
    File file = getFile();
    Q_ASSERT(WO::getGFile(&file) != 0);
    g_file_read_async(WO::getGFile(&file),
            d->m_ioPriority,
            d->m_gcancellable,
            _async_read_ready_callback,
            thisQPointer);

}

void FileAsyncOperation::appendTo(File::CreateFlags flags)
{
    void * thisQPointer = new QPointer<FileAsyncOperation>(this);
    File file = getFile();
    g_file_append_to_async(WO::getGFile(&file),
            (GFileCreateFlags)flags,
            d->m_ioPriority,
            d->m_gcancellable,
            _append_to_ready_callback,
            thisQPointer);
}

void FileAsyncOperation::create(File::CreateFlags flags)
{
    void * thisQPointer = new QPointer<FileAsyncOperation>(this);
    File file = getFile();
    g_file_create_async(WO::getGFile(&file),
            (GFileCreateFlags)flags,
            d->m_ioPriority,
            d->m_gcancellable,
            _create_ready_callback,
            thisQPointer);
}

void FileAsyncOperation::replace(const QString & etag, bool makeBackup, File::CreateFlags flags)
{
    void * thisQPointer = new QPointer<FileAsyncOperation>(this);
    File file = getFile();
    g_file_replace_async(WO::getGFile(&file),
            etag.toUtf8().data(),
            makeBackup,
            (GFileCreateFlags)flags,
            d->m_ioPriority,
            d->m_gcancellable,
            _replace_ready_callback,
            thisQPointer);
}

void FileAsyncOperation::queryInfo(const char *attributes, File::QueryInfoFlags flags)
{
    void * thisQPointer = new QPointer<FileAsyncOperation>(this);
    File file = getFile();
    g_file_query_info_async(WO::getGFile(&file),
            attributes,
            (GFileQueryInfoFlags)flags,
            d->m_ioPriority,
            d->m_gcancellable,
            _query_info_async_callback,
            thisQPointer);

}

void FileAsyncOperation::queryFilesystemInfo(const char *attributes)
{
    void * thisQPointer = new QPointer<FileAsyncOperation>(this);
    File file = getFile();
    g_file_query_filesystem_info_async(WO::getGFile(&file),
            attributes,
            d->m_ioPriority,
            d->m_gcancellable,
            _query_filesystem_info_async_callback,
            thisQPointer);
}


void FileAsyncOperation::findEnclosingMount()
{
    void * thisQPointer = new QPointer<FileAsyncOperation>(this);
    File file = getFile();
    g_file_find_enclosing_mount_async(WO::getGFile(&file),
            d->m_ioPriority,
            d->m_gcancellable,
            _find_enclosing_mount_async_callback,
            thisQPointer);
}

void FileAsyncOperation::enumerateChildren(const char *attributes, File::QueryInfoFlags flags)
{
    void * thisQPointer = new QPointer<FileAsyncOperation>(this);
    File file = getFile();
    g_file_enumerate_children_async(WO::getGFile(&file),
            attributes,
            (GFileQueryInfoFlags) flags,
            d->m_ioPriority,
            d->m_gcancellable,
            _enumerate_children_async_callback,
            thisQPointer);

}

void FileAsyncOperation::setDisplayName(const QString & displayName)
{
    void * thisQPointer = new QPointer<FileAsyncOperation>(this);
    File file = getFile();
    g_file_set_display_name_async(WO::getGFile(&file),
            displayName.toUtf8().data(),
            d->m_ioPriority,
            d->m_gcancellable,
            _set_display_name_async_callback,
            thisQPointer);

}

void FileAsyncOperation::copy(const File & destination, File::CopyFlags flags)
{
    void * thisQPointer = new QPointer<FileAsyncOperation>(this);
    File file = getFile();
    g_file_copy_async(WO::getGFile(&file),
            WO::getGFile(&destination),
            (GFileCopyFlags) flags,
            d->m_ioPriority,
            d->m_gcancellable,
            _copy_progress_callback,
            thisQPointer,
            _copy_async_ready_callback,
            thisQPointer);
}

void FileAsyncOperation::setAttributes(const FileInfo & info,
             File::QueryInfoFlags flags)
{
    void * thisQPointer = new QPointer<FileAsyncOperation>(this);
    File file = getFile();
    g_file_set_attributes_async(WO::getGFile(&file),
                                WO::getGFileInfo(&info),
                                (GFileQueryInfoFlags) flags,
                                d->m_ioPriority,
                                d->m_gcancellable,
                                _set_attributes_async_ready_callback,
                                thisQPointer);

}

void FileAsyncOperation::mountMountable(MountOperation & mountOperation,
             Mount::MountFlags flags)
{
    void * thisQPointer = new QPointer<FileAsyncOperation>(this);
    File file = getFile();
    g_file_mount_mountable(WO::getGFile(&file),
                          (GMountMountFlags) flags,
                          WO::getGMountOperation(&mountOperation),
                          d->m_gcancellable,
                          _mount_mountable_ready_callback,
                          thisQPointer);

}

void FileAsyncOperation::unmountMountable(Mount::UnmountFlags flags)
{
    void * thisQPointer = new QPointer<FileAsyncOperation>(this);
    File file = getFile();
    g_file_unmount_mountable(WO::getGFile(&file),
                             (GMountUnmountFlags) flags,
                             d->m_gcancellable,
                             _unmount_mountable_ready_callback,
                             thisQPointer);

}

void FileAsyncOperation::ejectMountable(Mount::UnmountFlags flags)
{
    void * thisQPointer = new QPointer<FileAsyncOperation>(this);
    File file = getFile();
    g_file_eject_mountable(WO::getGFile(&file),
                           (GMountUnmountFlags) flags,
                           d->m_gcancellable,
                           _eject_mountable_ready_callback,
                           thisQPointer);

}

void FileAsyncOperation::mountEnclosingVolume(MountOperation & mountOperation,
                Mount::MountFlags flags)
{
    void * thisQPointer = new QPointer<FileAsyncOperation>(this);
    File file = getFile();
    g_file_mount_enclosing_volume(WO::getGFile(&file),
                                  (GMountMountFlags) flags,
                                  WO::getGMountOperation(&mountOperation),
                                  d->m_gcancellable,
                                  _mount_enclosing_volume_ready_callback,
                                  thisQPointer);

}

void FileAsyncOperation::loadContentsAsync()
{
    void * thisQPointer = new QPointer<FileAsyncOperation>(this);
    File file = getFile();
    g_file_load_contents_async(WO::getGFile(&file),
            d->m_gcancellable,
            _load_contents_async_ready_callback,
            thisQPointer);

}

void FileAsyncOperation::loadPartialContents()
{
    void * thisQPointer = new QPointer<FileAsyncOperation>(this);
    File file = getFile();
    g_file_load_partial_contents_async(WO::getGFile(&file),
                d->m_gcancellable,
                _load_partial_contents_read_more_callback,
                _load_partial_contents_async_ready_callback,
                thisQPointer);

}

void FileAsyncOperation::replaceContents(const QByteArray contents,
                     const QString & etag,
                     bool makeBackup,
                     File::CreateFlags flags)
{
    void * thisQPointer = new QPointer<FileAsyncOperation>(this);
    File file = getFile();
    g_file_replace_contents_async(WO::getGFile(&file),
                   contents.data(),
                   contents.size(),
                   etag.toUtf8(),
                   makeBackup,
                   (GFileCreateFlags) flags,
                   d->m_gcancellable,
                   _replace_contents_async_ready_callback,
                   thisQPointer);
}



File FileAsyncOperation::getFile() const
{
    return d->m_file;
}

///// ------------------------------------------

void FileAsyncOperation::emitInputStreamReady(const File & file, FileInputStream * inputStream, const Error & error)
{
    emit inputStreamReady(file, inputStream, error);
}

void FileAsyncOperation::emitOutputStreamReady(const File & file, FileOutputStream * outputStream, const Error & error)
{
    emit outputStreamReady(file, outputStream, error);
}

void FileAsyncOperation::emitQueryInfoReady(const File & file, const FileInfo & fileInfo, const Error & error)
{
    emit queryInfoReady(file, fileInfo, error);
}

void FileAsyncOperation::emitEnumerateChildrenReady(const File & file, const FileEnumerator & fileEnumerator, const Error & error)
{
    emit enumerateChildrenReady(file, fileEnumerator, error);
}

void FileAsyncOperation::emitQueryFilesystemInfoReady(const File & file, const FileInfo & fileInfo, const Error & error)
{
    emit queryFilesystemInfoReady(file, fileInfo, error);
}

void FileAsyncOperation::emitFindEnclosingMountReady(const File & file, const Mount & mount, const Error & error)
{
    emit findEnclosingMountReady(file, mount, error);
}

void FileAsyncOperation::emitSetDisplayNameReady(const File & file, const File& result, const Error & error)
{
    emit setDisplayNameReady(file, result, error);
}

void FileAsyncOperation::emitCopyProgress(qint64 currentNumBytes, qint64 totalNumBytes)
{
    emit copyProgress(currentNumBytes, totalNumBytes);
}

void FileAsyncOperation::emitCopyReady(const File & file, const Error & error)
{
    emit copyReady(file, error);
}

void FileAsyncOperation::emitSetAttributesReady(const File & file, const FileInfo & fileInfo, const Error & error)
{
    emit setAttributesReady(file, fileInfo, error);
}

void FileAsyncOperation::emitMountMountableReady(const File & file, const File& result, const Error & error)
{
    emit mountMountableReady(file, result, error);
}

void FileAsyncOperation::emitUnmountMountableReady(const File & file, const Error & error)
{
    emit unmountMountableReady(file, error);
}

void FileAsyncOperation::emitEjectMountableReady(const File & file, const Error & error)
{
    emit ejectMountableReady(file, error);
}

void FileAsyncOperation::emitMountEnclosingVolumeReady(const File & file, const Error & error)
{
    emit mountEnclosingVolumeReady(file, error);
}

void FileAsyncOperation::emitLoadContentsReady(const File & file, const QByteArray & contents, const QString & etag, const Error & error)
{
    emit loadContentsReady(file, contents, etag, error);
}

void FileAsyncOperation::emitLoadPartialContentsReady(const File & file, const char * content, qint64 size, const QString & etag, const Error & error)
{
    emit loadPartialContentsReady(file, content, size, etag, error);
}

void FileAsyncOperation::emitLoadPartialContentsReadMore(const File & file, const char * buf, qint64 currentSize)
{
    emit loadPartialContentsReadMore(file, buf, currentSize);
}

void FileAsyncOperation::emitReplaceContentsReady(const File & file, const QString & newEtag, const Error & error)
{
    emit replaceContentsReady(file, newEtag, error);
}

}}

#include "moc_kgio-fileasyncoperation_p.cpp"

