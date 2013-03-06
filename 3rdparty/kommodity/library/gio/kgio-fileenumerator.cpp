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

#include "kgio-file.h"
#include "kgio-fileinfo.h"
#include "kgio-wrappedobjectutils_p.h"

namespace Kommodity {
namespace GIO {
    
/* ------- callbacks -------- */

static void  _close_async_ready_callback(GObject *source_object, GAsyncResult *result, gpointer user_data)
{
    Error error;
    
    gboolean success = g_file_enumerator_close_finish(
            G_FILE_ENUMERATOR(source_object),
            result,
            WO::getGErrorPtr(&error));
    
    QPointer<FileEnumerator> * fileEnumeratorQPointer = (QPointer<FileEnumerator> *)user_data;
    if (!fileEnumeratorQPointer->isNull())
    {
        fileEnumeratorQPointer->data()->emitCloseAsyncReady(fileEnumeratorQPointer->data(), error);
    }
    delete fileEnumeratorQPointer;
}
    
    
static void  _next_files_async_ready_callback(GObject *source_object, GAsyncResult *result, gpointer user_data)
{
    Error error;
    
    gssize count = g_input_stream_read_finish(
            G_INPUT_STREAM(source_object),
            result,
            WO::getGErrorPtr(&error));
    
    GList* next_files = g_file_enumerator_next_files_finish(
            G_FILE_ENUMERATOR(source_object),
            result,
            WO::getGErrorPtr(&error));
    
    QList<FileInfo> fileInfos;
    
    while(next_files)
    {
        FileInfo fileInfo;
        WO::setGFileInfo(&fileInfo, (GFileInfo*) next_files->data);
        fileInfos.append(fileInfo);

        next_files = next_files->next;
    }
    
    
    QPointer<FileEnumerator> * fileEnumeratorQPointer = (QPointer<FileEnumerator> *)user_data;
    if (!fileEnumeratorQPointer->isNull())
    {
        fileEnumeratorQPointer->data()->emitNextFilesAsyncReady(
                fileEnumeratorQPointer->data(), 
                fileInfos,
                error);
    }
    delete fileEnumeratorQPointer;
}
    
    
class FileEnumerator::Private : public QSharedData
{
public:
    Private() 
    {
    };
};


FileEnumerator::FileEnumerator() : d(0)
{
    WO::initGIO();
}


FileEnumerator::~FileEnumerator()
{
}

FileEnumerator::FileEnumerator( const FileEnumerator& other) : WrappedObject(other), d(0)
{
}

FileEnumerator &FileEnumerator::operator=(const FileEnumerator &other)
{
    WrappedObject::operator=(other);
    d = other.d;
    return *this;
}

FileInfo FileEnumerator::nextFile(Error &error,const Cancellable * cancellable)
{
    FileInfo fileInfo;
    WO::setGFileInfo(&fileInfo, g_file_enumerator_next_file(
            WO::getGFileEnumerator(this),
            WO::getGCancellable(cancellable),
            WO::getGErrorPtr(&error)));
    return fileInfo;
}

bool FileEnumerator::close(Error & error,const Cancellable * cancellable)
{
    return g_file_enumerator_close(WO::getGFileEnumerator(this),
                                   WO::getGCancellable(cancellable),
                                   WO::getGErrorPtr(&error));
}

void FileEnumerator::nextFilesAsync(int numFiles, int ioPriority,
                   const Cancellable * cancellable)
{
    void * thisQPointer = new QPointer<FileEnumerator>(this);
    
    g_file_enumerator_next_files_async  (
            WO::getGFileEnumerator(this),
            numFiles,
            ioPriority,
            WO::getGCancellable(cancellable),
            _next_files_async_ready_callback,
            thisQPointer);    
}

void FileEnumerator::closeAsync(int ioPriority,
               const Cancellable * cancellable)
{
    void * thisQPointer = new QPointer<FileEnumerator>(this);
    
    g_file_enumerator_close_async(
            WO::getGFileEnumerator(this),
            ioPriority,
            WO::getGCancellable(cancellable),
            _close_async_ready_callback,
            thisQPointer);    
}

bool FileEnumerator::isClosed()
{
    return g_file_enumerator_is_closed(WO::getGFileEnumerator(this));    
}

bool FileEnumerator::hasPending()
{
    return g_file_enumerator_has_pending(WO::getGFileEnumerator(this));    
}

void FileEnumerator::setPending(bool pending)
{
    g_file_enumerator_set_pending(WO::getGFileEnumerator(this), pending);    
}

void FileEnumerator::emitNextFilesAsyncReady(FileEnumerator * fileEnumerator, const QList<FileInfo> & fileInfos, const Error & error)
{
    emit nextFilesAsyncReady(fileEnumerator, fileInfos, error);
}

void FileEnumerator::emitCloseAsyncReady(FileEnumerator * fileEnumerator, const Error & error)
{
    emit closeAsyncReady(fileEnumerator, error);
}



}}

#include "kgio-fileenumerator.moc"
