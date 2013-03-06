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


#ifndef KGIOCANCELLABLE_
#define KGIOCANCELLABLE_

#include <QtCore/QObject>
#include <QtCore/QString>
#include <QtCore/QUrl>
#include <QtCore/QSharedData>

#include <Kommodity/GIO/WrappedObject>
#include <Kommodity/GIO/Error>

namespace Kommodity {
namespace GIO {

class Q_DECL_EXPORT Cancellable : public QObject, public virtual WrappedObject
{
    Q_OBJECT

public:
    
    Cancellable();
    virtual ~Cancellable();

    bool isCancelled() const;
    void setErrorIfCancelled(Error & error);
    int getFd() const;
    
    
    static Cancellable getCurrent();
    void popCurrent();
    void pushCurrent();
    
    void reset();
    void cancel();
    
Q_SIGNALS:
    
    void cancelled();

private:

    void * d; // future extension
};

}}  // namespace


#endif /*KGIOGCANCELLABLE_*/
