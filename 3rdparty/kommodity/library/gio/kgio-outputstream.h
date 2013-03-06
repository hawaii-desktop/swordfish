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


#ifndef KGIOOUTPUSTREAM_H_
#define KGIOOUTPUSTREAM_H_

#include <Kommodity/GIO/WrappedObject>
#include <Kommodity/GIO/Cancellable>
#include <Kommodity/GIO/InputStream>

namespace Kommodity {
namespace GIO {

class Q_DECL_EXPORT OutputStream : public QObject, public virtual WrappedObject
{
    Q_OBJECT
    
public:
    OutputStream();
    virtual ~OutputStream();
    
    enum SpliceFlags {
      SpliceNone = 0,
      SpliceCloseSource = 1 << 0,
      SpliceCloseTraget = 1 << 1
    };

    qint64 write(const void *buffer,
            quint64 count,
            Error & error,
            const Cancellable * cancellable = 0);
    
    bool writeAll(const void *buffer,
            quint64 count,
            quint64 &bytesWritten,
            Error &error,
            const Cancellable * cancellable = 0);
    
    qint64 splice(InputStream & source, 
            SpliceFlags flags,
            Error & error,
            const Cancellable * cancellable = 0);
    
    bool flush(Error &error,
            const Cancellable * cancellable = 0);
    
    bool close(Error &error,
            const Cancellable * cancellable = 0);
    
    void writeAsync(const void *buffer,
            quint64 count,
            int io_priority,
            const Cancellable * cancellable = 0);
    
    void spliceAsync(InputStream & source,
            SpliceFlags flags,
            int ioPriority,
            const Cancellable * cancellable = 0);

    void flushAsync(int ioPriority,
            const Cancellable * cancellable = 0);

    void closeAsync(
            int ioPriority,
            const Cancellable * cancellable = 0);

    bool isClosed();
    
    bool hasPending();
    bool setPending(Error & error);
    void clearPending();

    // internal
    void emitWriteAsyncReady(OutputStream * outputStream, const qint64 count, const Error & error);
    void emitSpliceAsyncReady(OutputStream * outputStream, const Error & error);
    void emitFlushAsyncReady(OutputStream * outputStream, const Error & error);
    void emitCloseAsyncReady(OutputStream * outputStream, const Error & error);
    
Q_SIGNALS:    
    void writeAsyncReady(OutputStream * outputStream, const qint64 count, const Error & error);
    void spliceAsyncReady(OutputStream * outputStream, const Error & error);
    void flushAsyncReady(OutputStream * outputStream, const Error & error);
    void closeAsyncReady(OutputStream * outputStream, const Error & error);


private:

    class Private;
    friend class Private;
    Private * d;    
};

}}

#endif /*KGIOOUTPUSTREAM_H_*/
