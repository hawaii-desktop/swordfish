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
#include <QtCore/QFile>
#include <QtCore/QString>

#include <glib.h>
#include <gio/gio.h>


#import "kgio-fileoutputstream.h"
#include "kgio-wrappedobjectutils_p.h"


namespace Kommodity {
namespace GIO {


class FileOutputStream::Private : public QSharedData
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

static void  _file_output_stream_query_info_async_callback(GObject *source_object, GAsyncResult *result, gpointer user_data)
{
    Error error;
    
    QPointer<FileOutputStream> * p = (QPointer<FileOutputStream> *)user_data;
    if (!p->isNull())
    {
        FileInfo fileInfo;
        
        GFileInfo* gFileInfo = g_file_output_stream_query_info_finish(
            G_FILE_OUTPUT_STREAM(source_object),
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


FileOutputStream::FileOutputStream() : OutputStream(), d(0)
{
}

FileOutputStream::~FileOutputStream()
{
}

/////


FileInfo FileOutputStream::queryInfo(char *attributes, Error & error, const Cancellable * cancellable)
{
    FileInfo fileInfo;
    WO::setGFileInfo(&fileInfo, g_file_output_stream_query_info(WO::getGFileOutputStream(this),
                                                             attributes,
                                                             WO::getGCancellable(cancellable),
                                                             WO::getGErrorPtr(&error)));
    return fileInfo;
}

void FileOutputStream::queryInfoAsync(char *attributes, int ioPriority, const Cancellable * cancellable)
{
    void * thisQPointer = new QPointer<FileOutputStream>(this);
    g_file_output_stream_query_info_async(WO::getGFileOutputStream(this),
             attributes,
             ioPriority,
             WO::getGCancellable(cancellable),
             _file_output_stream_query_info_async_callback,
             thisQPointer);
     
}

QString FileOutputStream::getEtag() const
{
    return QString(g_file_output_stream_get_etag(WO::getGFileOutputStream(this)));
}


//////

void FileOutputStream::emitQueryInfoReady(const FileOutputStream * fileOutputStream, const FileInfo & fileInfo, const Error & error)
{
    emit queryInfoReady(fileOutputStream, fileInfo, error);
}



// TODO

}}

#include "kgio-fileoutputstream.moc"
