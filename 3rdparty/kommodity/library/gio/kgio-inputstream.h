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


#ifndef KGIOINPUTSTREAM_H_
#define KGIOINPUTSTREAM_H_

#include <QObject>

#include <Kommodity/GIO/WrappedObject>
#include <Kommodity/GIO/Error>
#include <Kommodity/GIO/Cancellable>

namespace Kommodity {
namespace GIO {

class Q_DECL_EXPORT InputStream : public QObject, public virtual WrappedObject
{
    Q_OBJECT

public:
    InputStream();
    virtual ~InputStream();

    qint64 read(void *buffer, qint64 count, Error & error, const Cancellable * cancellable = 0);

    bool readAll(void *buffer,
            qint64 count,
            qint64 & bytesRead,
            Error & error,
            const Cancellable * cancellable = 0);

    qint64 skip(qint64 count, Error & error, const Cancellable * cancellable = 0);

    bool close(Error & error, const Cancellable * cancellable = 0);

    void readAsync(void *buffer, qint64 count, int ioPriority, const Cancellable * cancellable = 0);
    void skipAsync(qint64 count, int ioPriority, const Cancellable * cancellable = 0);
    void closeAsync(int ioPriority, const Cancellable * cancellable = 0);

    bool isClosed();
    bool hasPending();
    bool setPending(Error & error);
    void clearPending();

    // internal
    void emitReadAsyncReady(InputStream * inputStream, const qint64 count, const Error & error);
    void emitSkipAsyncReady(InputStream * inputStream, const qint64 count, const Error & error);
    void emitCloseAsyncReady(InputStream * inputStream, const Error & error);

public:
Q_SIGNALS:
    void readAsyncReady(InputStream * inputStream, const qint64 count, const Error & error);
    void skipAsyncReady(InputStream * inputStream, const qint64 count, const Error & error);
    void closeAsyncReady(InputStream * inputStream, const Error & error);

private:

    class Private;
    friend class Private;
    Private * d;
};

}}




#endif /*KGIOINPUTSTREAM_H_*/
