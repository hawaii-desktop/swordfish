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

#include "kgio-fileattributematcher.h"
#include "kgio-wrappedobjectutils_p.h"

namespace Kommodity {
namespace GIO {
    
   
class FileAttributeMatcher::Private : public QSharedData
{
public:
    Private() : m_gFileAttributeMatcher(0)
    {
    };
    
    ~Private()
    {
        if (m_gFileAttributeMatcher)
            g_file_attribute_matcher_unref(m_gFileAttributeMatcher);
    };
    GFileAttributeMatcher* m_gFileAttributeMatcher;
    
};


FileAttributeMatcher::FileAttributeMatcher(const char *attributes) : d(new Private)
{
    d->m_gFileAttributeMatcher = g_file_attribute_matcher_new (attributes);
}


FileAttributeMatcher::~FileAttributeMatcher()
{
}

FileAttributeMatcher::FileAttributeMatcher( const FileAttributeMatcher& other) : d(other.d)
{
}

FileAttributeMatcher &FileAttributeMatcher::operator=(const FileAttributeMatcher &other)
{
    d = other.d;
    return *this;
}

void * FileAttributeMatcher::getObjectPtr() const
{
    return d->m_gFileAttributeMatcher;
}

void FileAttributeMatcher::setObjectPtr(void * objectPtr)
{
    d->m_gFileAttributeMatcher = (GFileAttributeMatcher*)objectPtr;
}

bool FileAttributeMatcher::matches(const char *attribute)
{
    return g_file_attribute_matcher_matches(d->m_gFileAttributeMatcher, attribute);
}

bool FileAttributeMatcher::matchesOnly(const char *attribute)
{
    return g_file_attribute_matcher_matches_only(d->m_gFileAttributeMatcher, attribute);
}

bool FileAttributeMatcher::enumerateNamespace(const char *ns)
{
    return g_file_attribute_matcher_enumerate_namespace(d->m_gFileAttributeMatcher, ns);
}

QByteArray FileAttributeMatcher::enumerateNext()
{
    return QByteArray(g_file_attribute_matcher_enumerate_next(d->m_gFileAttributeMatcher));
}

}}

#include "moc_kgio-fileattributematcher.cpp"

