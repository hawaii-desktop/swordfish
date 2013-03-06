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


#ifndef KGIOCONTENTTYPE_H_
#define KGIOCONTENTTYPE_H_

#include <QtCore/QString>
#include <QtCore/QList>
#include <QtCore/QSharedData>
#include <Kommodity/GIO/Icon>

namespace Kommodity {
namespace GIO {
    
class Q_DECL_EXPORT ContentType
{
public:
    ContentType(const QByteArray & type);
    virtual ~ContentType();
    ContentType(const ContentType&);
    ContentType &operator=(const ContentType &other);
    
    bool equals(const ContentType & other) const;
    bool isA(const ContentType & supertype) const;
    bool isUnknown() const;
    QString getDescription() const;
    QByteArray getMimeType() const;
    QByteArray getType() const;
    Icon * getIcon() const;
    bool canBeExecutable() const;
    
    static ContentType guess(const QString & filename, const char *data, quint64 size, bool & resultUncertain);
    static QList<ContentType> getRegistered();
    
private:
    QByteArray m_contentType;
    
    class Private;
    friend class Private;
    QSharedDataPointer<Private> d;    
    
};
    
}}

#endif /*KGIOCONTENTTYPE_H_*/
