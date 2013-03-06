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


#ifndef KGIOFILEOUTPUTSTREAM_H_
#define KGIOFILEOUTPUTSTREAM_H_

#include "kgio-outputstream.h"
#include "kgio-seekable.h"

#include <Kommodity/GIO/OutputStream>
#include <Kommodity/GIO/Seekable>
#include <Kommodity/GIO/FileInfo>

namespace Kommodity {
namespace GIO {

class Q_DECL_EXPORT FileOutputStream : public OutputStream, public Seekable
{
    Q_OBJECT
    
public:
    FileOutputStream();
    virtual ~FileOutputStream();
    
    FileInfo queryInfo(char *attributes, 
                    Error & error, 
                    const Cancellable * cancellable = 0);
    
    void queryInfoAsync(char *attributes, 
                    int ioPriority, 
                    const Cancellable * cancellable = 0);

    QString getEtag() const;

Q_SIGNALS:
    void queryInfoReady(const FileOutputStream * fileOutputStream, const FileInfo & fileInfo, const Error & error);
    
private:

    class Private;
    friend class Private;
    Private * d;       

public: // internal
    void emitQueryInfoReady(const FileOutputStream * fileOutputStream, const FileInfo & fileInfo, const Error & error);

    
};

}}

#endif /*KGIOFILEOUTPUTSTREAM_H_*/
