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

#include <QtCore/QString>
#include <QtCore/QPointer>

#include <glib.h>
#include <gio/gio.h>

#include "kgio-error.h"
#include "kgio-seekable.h"
#include "kgio-file.h"
#include "kgio-mount.h"
#include "kgio-wrappedobjectutils_p.h"

namespace Kommodity {
namespace GIO {

Seekable::Seekable()
{
}
    
qint64 Seekable::tell() const
{
    return g_seekable_tell(WO::getGSeekable(this));
}

bool Seekable::canSeek() const
{
    return g_seekable_can_seek(WO::getGSeekable(this));
}

bool Seekable::seek(qint64 offset, SeekType type, Error & error, const Cancellable * cancellable)
{
    return g_seekable_seek(
            WO::getGSeekable(this),
            offset, 
            (GSeekType)type, 
            WO::getGCancellable(cancellable),
            WO::getGErrorPtr(&error));
}
    
bool Seekable::canTruncate() const
{
    return g_seekable_can_truncate(WO::getGSeekable(this));
}

bool Seekable::truncate(qint64 offset, Error & error, const Cancellable * cancellable)
{
    return g_seekable_truncate(
            WO::getGSeekable(this),
            offset,
            WO::getGCancellable(cancellable),
            WO::getGErrorPtr(&error));
}

}}
