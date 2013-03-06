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

#include <QtCore/QString>
#include <QtCore/QPointer>

#include <glib.h>
#include <gio/gio.h>

#include "kgio-error.h"
#include "kgio-file.h"
#include "kgio-inputstream.h"
#include "kgio-mount.h"
#include "kgio-wrappedobjectutils_p.h"

namespace Kommodity {
namespace GIO {

/* ------- callbacks -------- */

static void  _close_async_ready_callback(GObject *source_object, GAsyncResult *result, gpointer user_data)
{
    Error error;
    
    gboolean success = g_output_stream_close_finish(
            G_OUTPUT_STREAM(source_object),
            result,
            WO::getGErrorPtr(&error));
    
    QPointer<OutputStream> * outputStreamQPointer = (QPointer<OutputStream> *)user_data;
    if (!outputStreamQPointer->isNull())
    {
        outputStreamQPointer->data()->emitCloseAsyncReady(outputStreamQPointer->data(), error);
    }
    delete outputStreamQPointer;
}
    
    
static void  _write_async_ready_callback(GObject *source_object, GAsyncResult *result, gpointer user_data)
{
    Error error;
    
    gssize count = g_output_stream_write_finish(
            G_OUTPUT_STREAM(source_object),
            result,
            WO::getGErrorPtr(&error));
    
    QPointer<OutputStream> * outputStreamQPointer = (QPointer<OutputStream> *)user_data;
    if (!outputStreamQPointer->isNull())
    {
        outputStreamQPointer->data()->emitWriteAsyncReady(
                outputStreamQPointer->data(), 
                count,
                error);
    }
    delete outputStreamQPointer;
}
 
static void  _splice_async_ready_callback(GObject *source_object, GAsyncResult *result, gpointer user_data)
{
    Error error;
    
    bool success = g_output_stream_splice_finish(
            G_OUTPUT_STREAM(source_object),
            result,
            WO::getGErrorPtr(&error));
    
    QPointer<OutputStream> * outputStreamQPointer = (QPointer<OutputStream> *)user_data;
    if (!outputStreamQPointer->isNull())
    {
        outputStreamQPointer->data()->emitSpliceAsyncReady(
                outputStreamQPointer->data(), 
                error);
    }
    delete outputStreamQPointer;
}


static void  _flush_async_ready_callback(GObject *source_object, GAsyncResult *result, gpointer user_data)
{
    Error error;
    
    bool success = g_output_stream_flush_finish(
            G_OUTPUT_STREAM(source_object),
            result,
            WO::getGErrorPtr(&error));
    
    QPointer<OutputStream> * outputStreamQPointer = (QPointer<OutputStream> *)user_data;
    if (!outputStreamQPointer->isNull())
    {
        outputStreamQPointer->data()->emitFlushAsyncReady(
                outputStreamQPointer->data(), 
                error);
    }
    delete outputStreamQPointer;
}
    
/* -------------------------- */
    
class OutputStream::Private : public QSharedData
{
public:
    Private() 
    {
    };
};

OutputStream::OutputStream() : WrappedObject(), d(0)
{
}

OutputStream::~OutputStream()
{
}

qint64 OutputStream::write(const void *buffer, 
        quint64 count, 
        Error & error,
        const Cancellable * cancellable)
{
    return g_output_stream_write(
            WO::getGOutputStream(this), 
            buffer, 
            count,  
            WO::getGCancellable(cancellable),
            WO::getGErrorPtr(&error)
            );
}

bool OutputStream::writeAll(const void *buffer, 
        quint64 count, 
        quint64 & bytesWritten, 
        Error & error,
        const Cancellable * cancellable)
{
    gsize bytes_written=0;
    bool success = g_output_stream_write_all(
            WO::getGOutputStream(this), 
            buffer, 
            count,  
            &bytes_written,
            WO::getGCancellable(cancellable),
            WO::getGErrorPtr(&error));
    bytesWritten = bytes_written;
    return success;
}

qint64 OutputStream::splice(InputStream & source, 
        SpliceFlags flags,
        Error & error,
        const Cancellable * cancellable)
{
    return  g_output_stream_splice(
            WO::getGOutputStream(this),
            WO::getGInputStream(&source),
            (GOutputStreamSpliceFlags)flags,
            WO::getGCancellable(cancellable),
            WO::getGErrorPtr(&error)
            );
}


bool OutputStream::flush(Error & error, const Cancellable * cancellable)
{
    return  g_output_stream_flush(
            WO::getGOutputStream(this), 
            WO::getGCancellable(cancellable),
            WO::getGErrorPtr(&error)
            );
}

bool OutputStream::close(Error & error, const Cancellable * cancellable)
{
    return  g_output_stream_close(
            WO::getGOutputStream(this), 
            WO::getGCancellable(cancellable),
            WO::getGErrorPtr(&error)
            );
}

void OutputStream::writeAsync(const void *buffer, quint64 count, int io_priority, const Cancellable * cancellable)
{
    void * thisQPointer = new QPointer<OutputStream>(this);
    
    return g_output_stream_write_async(
            WO::getGOutputStream(this),
            buffer,
            count,
            io_priority,
            WO::getGCancellable(cancellable),
            _write_async_ready_callback,
            thisQPointer
    );    
}

void OutputStream::spliceAsync(InputStream & source,
        SpliceFlags flags,
        int ioPriority,
        const Cancellable * cancellable)
{
    void * thisQPointer = new QPointer<OutputStream>(this);
    
    return g_output_stream_splice_async(
            WO::getGOutputStream(this),
            WO::getGInputStream(&source),
            (GOutputStreamSpliceFlags)flags,
            ioPriority,
            WO::getGCancellable(cancellable),
            _splice_async_ready_callback,
            thisQPointer
    );       
}

void OutputStream::flushAsync(int io_priority, const Cancellable * cancellable)
{
    void * thisQPointer = new QPointer<OutputStream>(this);
    
    return g_output_stream_flush_async(
            WO::getGOutputStream(this),
            io_priority,
            WO::getGCancellable(cancellable),
            _flush_async_ready_callback,
            thisQPointer
    );
}

void OutputStream::closeAsync(int io_priority, const Cancellable * cancellable)
{
    void * thisQPointer = new QPointer<OutputStream>(this);
    
    return g_output_stream_close_async(
            WO::getGOutputStream(this),
            io_priority,
            WO::getGCancellable(cancellable),
            _close_async_ready_callback,
            thisQPointer
    );
}

void OutputStream::emitWriteAsyncReady(OutputStream * outputStream, const qint64 count, const Error & error)
{
    emit writeAsyncReady(outputStream, count, error);
}

void OutputStream::emitSpliceAsyncReady(OutputStream * outputStream, const Error & error)
{
    emit spliceAsyncReady(outputStream, error);
}

void OutputStream::emitFlushAsyncReady(OutputStream * outputStream, const Error & error)
{
    emit flushAsyncReady(outputStream, error);
}

void OutputStream::emitCloseAsyncReady(OutputStream * outputStream, const Error & error)
{
    emit closeAsyncReady(outputStream, error);
}


bool OutputStream::isClosed()
{
    return g_output_stream_is_closed(WO::getGOutputStream(this));
}

bool OutputStream::hasPending()
{
    return g_output_stream_has_pending(WO::getGOutputStream(this));
}

bool OutputStream::setPending(Error & error)
{
    return g_output_stream_set_pending(WO::getGOutputStream(this), WO::getGErrorPtr(&error));
}

void OutputStream::clearPending()
{
    g_output_stream_clear_pending(WO::getGOutputStream(this));
}


}}

#include "kgio-outputstream.moc"
