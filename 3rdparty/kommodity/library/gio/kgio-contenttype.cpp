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

#include <QtCore/QPointer>
#include <QtCore/QString>

#include <glib.h>
#include <gio/gio.h>


#include "kgio-contenttype.h"
#include "kgio-wrappedobjectutils_p.h"

namespace Kommodity {
namespace GIO {


class ContentType::Private : public QSharedData
{
public:
    Private() 
    {
    };
};

ContentType::ContentType(const QByteArray & type) : m_contentType(type), d(0)
{
}

ContentType::ContentType(const ContentType& other) 
    : d(0), m_contentType(other.m_contentType)
{
}

ContentType &ContentType::operator=(const ContentType &other)
{
    m_contentType = other.m_contentType;
    d = other.d;
    return *this;
}

ContentType::~ContentType()
{
}

bool ContentType::equals(const ContentType & other) const
{
    return g_content_type_equals(m_contentType.data(),
            other.m_contentType.data());
}

bool ContentType::isA(const ContentType & supertype) const
{
    return g_content_type_is_a(m_contentType.data(),
            supertype.m_contentType.data());
}

bool ContentType::isUnknown() const
{
    return g_content_type_is_unknown(m_contentType.data());
}

QString ContentType::getDescription() const
{
    return QString(g_content_type_get_description(m_contentType.data()));

}

QByteArray ContentType::getMimeType() const
{
    return QByteArray(g_content_type_get_mime_type(m_contentType.data()));

}

QByteArray ContentType::getType() const
{
    return m_contentType.data();
}

Icon * ContentType::getIcon() const
{
    GIcon * gIcon = g_content_type_get_icon(m_contentType.data());
    if (!gIcon) return 0;
    
    Icon * icon = new Icon;
    WO::setGIcon(icon, gIcon);
    return icon;
}

bool ContentType::canBeExecutable() const
{
    return g_content_type_can_be_executable(m_contentType.data());

}

ContentType ContentType::guess(const QString & filename, const char *data, quint64 size, bool & resultUncertain)
{
    gboolean result_uncertain = 0;
    ContentType result = ContentType(QByteArray(g_content_type_guess(filename.toUtf8().data(),
                                                             (const guchar*)data,
                                                             size,
                                                             &result_uncertain)));
    resultUncertain = result_uncertain;
    return result;
}

QList<ContentType> ContentType::getRegistered()
{
    QList<ContentType> result;
    
    GList* gList = g_content_types_get_registered();
    GList* next = gList;
    while (next)
    {
        ContentType contentType = ContentType(QByteArray((char*)next->data));
        result.append(contentType);
        next=next->next;
    }
    
    g_list_foreach(gList, (GFunc)g_free, NULL);
    g_list_free(gList);
    return result;
}


}}

