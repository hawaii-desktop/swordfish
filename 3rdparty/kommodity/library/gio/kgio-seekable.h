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


#ifndef KGIOSEEKABLE_H_
#define KGIOSEEKABLE_H_

#include <Kommodity/GIO/Cancellable>

namespace Kommodity {
namespace GIO {

class Q_DECL_EXPORT Seekable : public virtual WrappedObject
{
protected: 
    Seekable();
    
public:
    
    enum SeekType
    {
      SeekCurrent,
      SeekSet,
      SeekEnd
    };
    
    qint64 tell() const;
    bool canSeek() const;
    bool seek(qint64 offset, SeekType type, Error & error, const Cancellable * cancellable = 0);

    
    bool canTruncate() const;
    bool truncate(qint64 offset, Error & error, const Cancellable * cancellable = 0);

};

}}

#endif /*KGIOSEEKABLE_H_*/
