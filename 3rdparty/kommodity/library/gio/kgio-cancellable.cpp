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

#include <QtCore/QByteRef>
#include <QtCore/QDate>
#include <QtCore/QVarLengthArray>
#include <QtCore/QCoreApplication>
#include <QtCore/QRegExp>
#include <QtCore/QFile>
#include <QtCore/QString>

#include <glib.h>
#include <gio/gio.h>

#include "kgio-file.h"
#include "kgio-cancellable.h"
#include "kgio-mount.h"
#include "kgio-wrappedobjectutils_p.h"

namespace Kommodity {
namespace GIO {

Cancellable::Cancellable() : d(0)
{
    WO::initGIO();
    WO::setGCancellable( this, g_cancellable_new  ());
}

Cancellable::~Cancellable()
{
}

void Cancellable::reset()
{
    g_cancellable_reset(WO::getGCancellable(this));
}
void Cancellable::cancel()
{
    g_cancellable_cancel(WO::getGCancellable(this));
}

}}

#include "kgio-cancellable.moc"
