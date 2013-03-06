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

#include <glib.h>
#include <gio/gio.h>

#include "kgio-error.h"

namespace Kommodity {
namespace GIO {
    
class Error::Private : public QSharedData
{
public:
    Private() : m_gError(0)
    {
    };
    
    ~Private()
    {
        if (m_gError)
            g_error_free(m_gError);
    }
    ::GError * m_gError;
};

Error::Error() : d(new Private)
{
}


Error::Error( void * error ) : d(new Private)
{
    d->m_gError = (::GError *) error;
}

Error::~Error()
{
}

void ** Error::getErrorPtr()
{
    Q_ASSERT(!hasError());
    return (void**) &d->m_gError;
}

bool Error::hasError() const
{
    return d->m_gError != 0;
}

int Error::getCode() const
{
    if (!hasError()) return 0;
    return d->m_gError->code;
}

QString Error::getMessage() const
{
    if (!hasError()) return QString();
    return QString(d->m_gError->message);
}

void Error::clear()
{
    if (hasError())
    {
        g_error_free(d->m_gError);
        d->m_gError = 0;
    }
}

bool Error::isIOError(IOError::Codes code)
{
    if (d->m_gError->domain != g_io_error_quark()) return false;
    return d->m_gError->code == code;
}

    
}}


