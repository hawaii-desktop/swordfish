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

#include <QtCore/QFile>
#include <QtCore/QString>
#include <QtCore/QPointer>

#include <glib.h>
#include <gio/gio.h>

#include "kgio-mount.h"
#include "kgio-drive.h"
#include "kgio-volume.h"
#include "kgio-file.h"
#include "kgio-wrappedobjectutils_p.h"

namespace Kommodity {
namespace GIO {
    
class Icon::Private : public QSharedData
{
public:
    Private() 
    {
    };
};


Icon::Icon() : d(0)
{
    WO::initGIO();
}


Icon::~Icon()
{
}


}}

#include "moc_kgio-icon.cpp"
