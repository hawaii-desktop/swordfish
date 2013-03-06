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


#ifndef KGIOFILEENUMERATOR_H_
#define KGIOFILEENUMERATOR_H_

#include <Kommodity/GIO/Error>
#include <Kommodity/GIO/File>

namespace Kommodity {
namespace GIO {
    
class Q_DECL_EXPORT FileEnumerator : public QObject, public virtual WrappedObject
{
    Q_OBJECT

public:    
    FileEnumerator();
    FileEnumerator( const FileEnumerator & fileInfo );
    virtual ~FileEnumerator();
    
    FileEnumerator &operator=(const FileEnumerator &other);
    
    FileInfo nextFile(Error &error, const Cancellable * cancellable = 0);
    
    bool close(Error & error, const Cancellable * cancellable = 0);
    
    void nextFilesAsync(int numFiles, int ioPriority,
            const Cancellable * cancellable = 0);
    
    void closeAsync(int ioPriority,
            const Cancellable * cancellable = 0);

    bool isClosed();
    bool hasPending();
    void setPending(bool pending);

    // internal
    void emitNextFilesAsyncReady(FileEnumerator * fileEnumerator, const QList<FileInfo> & fileInfos, const Error & error);
    void emitCloseAsyncReady(FileEnumerator * fileEnumerator, const Error & error);
    
Q_SIGNALS:     
    void nextFilesAsyncReady(FileEnumerator * fileEnumerator, const QList<FileInfo> & fileInfos, const Error & error);
    void closeAsyncReady(FileEnumerator * fileEnumerator, const Error & error);
    
    
private:

    class Private;
    friend class Private;
    QSharedDataPointer<Private> d;
};

}}


#endif /*KGIOFILEENUMERATOR_H_*/
