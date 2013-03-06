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


#ifndef KGIOFILEDEVICE_H_
#define KGIOFILEDEVICE_H_

#include <QtCore/QObject>
#include <QtCore/QString>
#include <QtCore/QByteArray>

#include <Kommodity/GIO/File>
#include <Kommodity/GIO/FileInputStream>
#include <Kommodity/GIO/FileOutputStream>
#include <Kommodity/GIO/FileEnumerator>
#include <Kommodity/GIO/FileInfo>
#include <Kommodity/GIO/Cancellable>



namespace Kommodity {
namespace GIO {

    
class Q_DECL_EXPORT FileDevice : public QIODevice
{
    Q_OBJECT

public:    
    FileDevice( const File & file );
    virtual ~FileDevice();

    virtual bool open ( OpenMode mode );    
    
    virtual bool atEnd () const;
    virtual qint64 bytesAvailable () const;
    virtual qint64 bytesToWrite () const;
    virtual bool canReadLine () const;
    virtual void close ();
    virtual bool isSequential () const;
    virtual qint64 pos () const;
    virtual bool reset ();
    virtual bool seek ( qint64 pos );
    virtual qint64 size () const;
    virtual bool waitForBytesWritten ( int msecs );
    virtual bool waitForReadyRead ( int msecs );

protected:
    virtual qint64 readData ( char * data, qint64 maxSize );
    virtual qint64 readLineData ( char * data, qint64 maxSize );
    virtual qint64 writeData ( const char * data, qint64 maxSize );
    
Q_SIGNALS:

    
private:

    class Private;
    friend class Private;
    QSharedDataPointer<Private> d;
};    

}}


#endif /*KGIOFILEDEVICE_H_*/
