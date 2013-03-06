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

#include <QtCore/QSharedData>

#include <glib.h>
#include <gio/gio.h>

#include "kgio-sharedwrappedobject.h"

namespace Kommodity {
namespace GIO {


SharedWrappedObject::SharedWrappedObject() : objectPtr(0)
{
}


SharedWrappedObject::SharedWrappedObject(void * _objectPtr) : objectPtr(_objectPtr)
{
}

SharedWrappedObject::SharedWrappedObject(const SharedWrappedObject &other) : objectPtr(other.objectPtr)
{
    if (objectPtr != NULL)
    {
        g_object_ref(objectPtr);
    }
}

SharedWrappedObject &SharedWrappedObject::operator=(const SharedWrappedObject &other)
{
    if (this == &other) return *this;
    
    if (objectPtr != NULL)
    {
        g_object_unref(objectPtr);
    }
    objectPtr = other.objectPtr;
    if (objectPtr != NULL)
    {
        g_object_ref(objectPtr);
    }
    return *this;    
}

SharedWrappedObject::~SharedWrappedObject()
{
    if (objectPtr != NULL)
    {
        g_object_unref(objectPtr);
    }    
}

void * SharedWrappedObject::getObjectPtr() const
{
    return objectPtr;
}

void SharedWrappedObject::setObjectPtr(void * _objectPtr)
{
    if (objectPtr != NULL)
    {
        g_object_unref(objectPtr);
    }    
    objectPtr = _objectPtr;
}

bool SharedWrappedObject::isNull() const
{
    return objectPtr == 0;
}


}}

#include "kgio-wrappedobject.moc"
