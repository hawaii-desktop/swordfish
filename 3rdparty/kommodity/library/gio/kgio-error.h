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

#ifndef KGIOERROR_H_
#define KGIOERROR_H_


#include <QtCore/QObject>
#include <QtCore/QString>
#include <QtCore/QSharedData>

#include <Kommodity/GIO/WrappedObject>
#include <Kommodity/GIO/IOError>

namespace Kommodity {
namespace GIO {

class Q_DECL_EXPORT Error 
{

public:
    
    Error(const Error&);
    Error();
    virtual ~Error();

    bool hasError() const;
    int getCode() const;
    QString getMessage() const;
    void clear();
    
    bool isIOError(IOError::Codes code);
    
    
private:
    // internal
    Error( void * errorPtr );
    void ** getErrorPtr();
    
    class Private;
    friend class Private;
    QExplicitlySharedDataPointer<Private> d;
    
    friend class WO;
};

}}  // namespace

#endif /*KOGFILE_H_*/
