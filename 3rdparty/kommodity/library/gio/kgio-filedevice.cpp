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

#include "kgio-filedevice.h"

namespace Kommodity {
namespace GIO {
    
class FileDevice::Private : public QSharedData
{
public:
    Private() 
    {
    };
    
    File file;
    mutable FileInputStream * fileInputStream;
};



FileDevice::FileDevice(const File & file) : d(new Private)
{
    d->file = file;
}


FileDevice::~FileDevice()
{
}

bool FileDevice::open ( QIODevice::OpenMode mode )
{
   
    Error error;
    if (mode & QIODevice::ReadOnly)
    {
        
        FileInputStream * in = d->file.read(error);
        if (error.hasError())
        {
            setErrorString (error.getMessage());
            return false;
        }
        d->fileInputStream=in;
    }
    else if (mode & QIODevice::WriteOnly)
    {
        setErrorString ("unsupported");
        return false;
    }
    else
    {
        setErrorString ("unsupported");
        return false;
    }
    setOpenMode(mode);
    return true;
}

bool FileDevice::atEnd () const
{
    // TODO
    return false;
}

qint64 FileDevice::bytesAvailable () const
{
    // TODO
    return 0;
}

qint64 FileDevice::bytesToWrite () const
{
    // TODO
    return -1;
}

bool FileDevice::canReadLine () const
{
    return false;
}

void FileDevice::close ()
{
    Error error;
    d->fileInputStream->close(error);
    if (error.hasError())
    {
        setErrorString (error.getMessage());    
    }
}

bool FileDevice::isSequential () const
{
    return true;
//    const bool canSeek = d->fileInputStream.canSeek();
//    return !canSeek;
}

qint64 FileDevice::pos () const
{
    
   return d->fileInputStream->tell();
}

bool FileDevice::reset ()
{
    Error error;
    d->fileInputStream->seek(0, Seekable::SeekSet, error);
    if (error.hasError())
    {
        setErrorString (error.getMessage());
        return false;
    }
    return true;
}

bool FileDevice::seek ( qint64 pos )
{
    Error error;
    d->fileInputStream->seek(pos, Seekable::SeekSet, error);
    if (error.hasError())
    {
        setErrorString (error.getMessage());
        return false;
    }
    return true;
}

qint64 FileDevice::size () const
{
    Error error;
    FileInfo fileInfo = d->fileInputStream->queryInfo((char*)"*", error);
    if (!error.hasError())
    {
        return fileInfo.getSize();
    }
    return -1;
}

bool FileDevice::waitForBytesWritten ( int msecs )
{
    return true;
}

bool FileDevice::waitForReadyRead ( int msecs )
{
    return true;
}

qint64 FileDevice::readData ( char * data, qint64 maxSize )
{
    if (d->fileInputStream->isNull())
    {
        setErrorString ("not in read mode");
        return -1;
    }
    
    Error error;
    qint64 n = d->fileInputStream->read(data, maxSize, error);
    if (error.hasError())
    {
        setErrorString (error.getMessage());
        return -1;
    }
    //fwrite (data, 1, n, stdout);
    
    return n;
}

qint64 FileDevice::readLineData ( char * data, qint64 maxSize )
{
    return readData(data, maxSize);
}

qint64 FileDevice::writeData ( const char * data, qint64 maxSize )
{
    return -1; // TODO
}


}}

#include "moc_kgio-filedevice.cpp"
