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

#include "kgio-wrappedobject.h"

#include <stdio.h>

namespace Kommodity {
namespace GIO {

WrappedObject::WrappedObject() : objectPtr(0)
{
}


WrappedObject::WrappedObject(void * _objectPtr) : objectPtr(_objectPtr)
{
}

WrappedObject::~WrappedObject()
{
    if (objectPtr != 0)
    {
        //fprintf(stderr, "unref gobject %s\n", G_OBJECT_TYPE_NAME(m_gObject));
        g_object_unref(objectPtr);
    }
}

void * WrappedObject::getObjectPtr() const
{
    return objectPtr;
}

void WrappedObject::setObjectPtr(void * _objectPtr)
{
    if(objectPtr!=0)
    {
        fprintf(stderr, "setObjectPtr problem: %s\n", G_OBJECT_TYPE_NAME(objectPtr));        
    }
    objectPtr = _objectPtr;
}

bool WrappedObject::isNull() const
{
    return objectPtr == 0;
}


}}

#include "moc_kgio-wrappedobject.cpp"
