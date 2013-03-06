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

#include <QtCore/QPointer>
#include <QtCore/QString>

#include <glib.h>
#include <gio/gio.h>


#import "kgio-fileinputstream.h"
#include "kgio-wrappedobjectutils_p.h"

namespace Kommodity {
namespace GIO {


class FileInputStream::Private : public QSharedData
{
public:
    Private() 
    {
    };
    
    ~Private()
    {
    }
};

/////

static void  _file_input_stream_query_info_async_callback(GObject *source_object, GAsyncResult *result, gpointer user_data)
{
    Error error;
    
    QPointer<FileInputStream> * p = (QPointer<FileInputStream> *)user_data;
    if (!p->isNull())
    {
        FileInfo fileInfo;
        
        GFileInfo* gFileInfo = g_file_input_stream_query_info_finish(
            G_FILE_INPUT_STREAM(source_object),
            result,
            WO::getGErrorPtr(&error));

        WO::setGFileInfo(&fileInfo, gFileInfo);
        
        
        p->data()->emitQueryInfoReady(
            p->data(),
            fileInfo,
            error);
    }
    delete p;
    
}

/////

FileInputStream::FileInputStream() : d(0)
{
}

FileInputStream::~FileInputStream()
{
}

FileInfo FileInputStream::queryInfo(char *attributes, Error & error, const Cancellable * cancellable)
{
    FileInfo fileInfo;
    WO::setGFileInfo(&fileInfo, g_file_input_stream_query_info(WO::getGFileInputStream(this),
                                                             attributes,
                                                             WO::getGCancellable(cancellable),
                                                             WO::getGErrorPtr(&error)));
    return fileInfo;
}

void FileInputStream::queryInfoAsync(char *attributes, int ioPriority, const Cancellable * cancellable)
{
    void * thisQPointer = new QPointer<FileInputStream>(this);
    g_file_input_stream_query_info_async(WO::getGFileInputStream(this),
             attributes,
             ioPriority,
             WO::getGCancellable(cancellable),
             _file_input_stream_query_info_async_callback,
             thisQPointer);
     
}


//////

void FileInputStream::emitQueryInfoReady(const FileInputStream * fileInputStream, const FileInfo & fileInfo, const Error & error)
{
    emit queryInfoReady(fileInputStream, fileInfo, error);
}


}}

#include "kgio-fileinputstream.moc"
