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


#ifndef KGIOWRAPPEDOBJECT_P_H_
#define KGIOWRAPPEDOBJECT_P_H_

namespace Kommodity {
namespace GIO {

class WrappedObject::Private : public QSharedData
{
public:
    Private() : m_gObject(0)
    {
    };
    GObject * m_gObject;
};

}}

#endif /*KGIOWRAPPEDOBJECT_P_H_*/
