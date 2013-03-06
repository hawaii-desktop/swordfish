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


#ifndef KGIOLOADABLEICON_H_
#define KGIOLOADABLEICON_H_

#include <QtCore/QObject>
#include <QtCore/QString>
#include <QtCore/QSharedData>

#include <Kommodity/GIO/WrappedObject>
#include <Kommodity/GIO/Cancellable>
#include <Kommodity/GIO/Error>
#include <Kommodity/GIO/Icon>
#include <Kommodity/GIO/InputStream>

namespace Kommodity {
namespace GIO {

class Q_DECL_EXPORT LoadableIcon : public Icon
{
    Q_OBJECT

public:
    
    LoadableIcon();
    virtual ~LoadableIcon();
    
    InputStream * load(
            int size, 
            char **type, 
            Error & error, 
            const Cancellable * cancellable = 0);

    void loadAsync(int size,
            const Cancellable * cancellable = 0);
    
Q_SIGNALS:
    void loadAsyncReady(LoadableIcon * icon, InputStream * inputStream, const QString & type, const Error & error); 
    
private:

    class Private;
    friend class Private;
    Private * d;

public: // internal
    void emitLoadAsyncReady(LoadableIcon * icon, InputStream * inputStream, const QString & type, const Error & error); 
    
};
}}

#endif /*KGIOLOADABLEICON_H_*/
