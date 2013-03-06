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

    
/* ------- vitamin d --------- */
    
class InputStream::Private : public QSharedData
{
public:
    Private() 
    {
    };
    
Q_SIGNALS:    
    void readAsyncReady(InputStream * inputStream, const qint64 count, const Error & error);
    void skipAsyncReady(InputStream * inputStream, const qint64 count, const Error & error);
    void closeAsyncReady(InputStream * inputStream, const Error & error);
    
};

    
/* ------- callbacks -------- */

static void  _close_async_ready_callback(GObject *source_object, GAsyncResult *result, gpointer user_data)
{
    Error error;
    
    gboolean success = g_input_stream_close_finish(
            G_INPUT_STREAM(source_object),
            result,
            WO::getGErrorPtr(&error));
    
    QPointer<InputStream> * inputStreamQPointer = (QPointer<InputStream> *)user_data;
    if (!inputStreamQPointer->isNull())
    {
        inputStreamQPointer->data()->emitCloseAsyncReady(inputStreamQPointer->data(), error);
    }
    delete inputStreamQPointer;
}
    
    
static void  _read_async_ready_callback(GObject *source_object, GAsyncResult *result, gpointer user_data)
{
    Error error;
    
    gssize count = g_input_stream_read_finish(
            G_INPUT_STREAM(source_object),
            result,
            WO::getGErrorPtr(&error));
    
    QPointer<InputStream> * inputStreamQPointer = (QPointer<InputStream> *)user_data;
    if (!inputStreamQPointer->isNull())
    {
        inputStreamQPointer->data()->emitReadAsyncReady(
                inputStreamQPointer->data(), 
                count,
                error);
    }
    delete inputStreamQPointer;
}
 

static void  _skip_async_ready_callback(GObject *source_object, GAsyncResult *result, gpointer user_data)
{
    Error error;
    
    gssize count = g_input_stream_skip_finish(
            G_INPUT_STREAM(source_object),
            result,
            WO::getGErrorPtr(&error));
    
    QPointer<InputStream> * inputStreamQPointer = (QPointer<InputStream> *)user_data;
    if (!inputStreamQPointer->isNull())
    {
        inputStreamQPointer->data()->emitSkipAsyncReady(
                inputStreamQPointer->data(), 
                count,
                error);
    }
    delete inputStreamQPointer;
}
    
/////

InputStream::InputStream() : d(new Private)
{
}

InputStream::~InputStream()
{
}

qint64 InputStream::read(void *buffer, qint64 count, Error & error, const Cancellable * cancellable)
{
    return g_input_stream_read(
            WO::getGInputStream(this), 
            buffer, 
            count,  
            WO::getGCancellable(cancellable),
            WO::getGErrorPtr(&error));
}

bool InputStream::readAll(void *buffer, 
        qint64 count, 
        qint64 & bytesRead, 
        Error & error,
        const Cancellable * cancellable)
{
    gsize bytes_read=0;
    bool success = g_input_stream_read_all(
            WO::getGInputStream(this), 
            buffer, 
            count,  
            &bytes_read,
            WO::getGCancellable(cancellable),
            WO::getGErrorPtr(&error));
    bytesRead = bytes_read;
    return success;
}

qint64 InputStream::skip(qint64 count, Error & error, const Cancellable * cancellable)
{
    return  g_input_stream_skip(
            WO::getGInputStream(this), 
            count,
            WO::getGCancellable(cancellable),
            WO::getGErrorPtr(&error)
            );
}

bool InputStream::close(Error & error, const Cancellable * cancellable)
{
    return  g_input_stream_close(
            WO::getGInputStream(this), 
            WO::getGCancellable(cancellable),
            WO::getGErrorPtr(&error)
            );
}

void InputStream::readAsync(void *buffer, qint64 count, int io_priority, const Cancellable * cancellable)
{
    void * thisQPointer = new QPointer<InputStream>(this);
    
    return g_input_stream_read_async(
            WO::getGInputStream(this),
            buffer,
            count,
            io_priority,
            WO::getGCancellable(cancellable),
            _read_async_ready_callback,
            thisQPointer
    );    
    
}

void InputStream::skipAsync(qint64 count, int io_priority, const Cancellable * cancellable)
{
    void * thisQPointer = new QPointer<InputStream>(this);
    
    return g_input_stream_skip_async(
            WO::getGInputStream(this),
            count,
            io_priority,
            WO::getGCancellable(cancellable),
            _skip_async_ready_callback,
            thisQPointer
    );       
}
void InputStream::closeAsync(int io_priority, const Cancellable * cancellable)
{
    void * thisQPointer = new QPointer<InputStream>(this);
    
    return g_input_stream_close_async(
            WO::getGInputStream(this),
            io_priority,
            WO::getGCancellable(cancellable),
            _close_async_ready_callback,
            thisQPointer
    );
}

void InputStream::emitReadAsyncReady(InputStream * inputStream, const qint64 count, const Error & error)
{
    emit readAsyncReady(inputStream, count, error);
}

void InputStream::emitSkipAsyncReady(InputStream * inputStream, const qint64 count, const Error & error)
{
    emit skipAsyncReady(inputStream, count, error);
}

void InputStream::emitCloseAsyncReady(InputStream * inputStream, const Error & error)
{
    emit closeAsyncReady(inputStream, error);
}


bool InputStream::isClosed()
{
    return g_input_stream_is_closed(WO::getGInputStream(this));
}

bool InputStream::hasPending()
{
    return g_input_stream_has_pending(WO::getGInputStream(this));
}

bool InputStream::setPending(Error & error)
{
    return g_input_stream_set_pending(WO::getGInputStream(this), WO::getGErrorPtr(&error));
}

void InputStream::clearPending()
{
    g_input_stream_clear_pending(WO::getGInputStream(this));
}


}}

#include "kgio-inputstream.moc"
