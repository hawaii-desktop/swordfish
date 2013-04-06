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

#ifndef KGIOFILEINFO_H_
#define KGIOFILEINFO_H_

#include <QtCore/QObject>
#include <QtCore/QString>
#include <QtCore/QVariant>
#include <QtCore/QTime>
#include <QtCore/QSharedData>
#include <QtGui/QIcon>

#include <Kommodity/GIO/SharedWrappedObject>
#include <Kommodity/GIO/Error>
#include <Kommodity/GIO/Icon>
#include <Kommodity/GIO/FileAttribute>
#include <Kommodity/GIO/FileAttributeMatcher>

namespace Kommodity {
namespace GIO {
    
class Q_DECL_EXPORT FileInfo : public virtual SharedWrappedObject
{

public:    
    FileInfo();
    FileInfo( const FileInfo & fileInfo );
    virtual ~FileInfo();
    
    FileInfo &operator=(const FileInfo &other);

    
    FileInfo dup();
    
    void copyInto(FileInfo & destInfo);
    
    bool hasAttribute(const char *attribute);
    
    QList<QByteArray> listAttributes(const char *name_space);
    
    FileAttributeType  getAttributeType(const char *attribute);
    
    void removeAttribute(const char *attribute);
    
    QString getAttributeAsString (const char *attribute);
    
    bool getAttributeData(const char *attribute,
           FileAttributeType *type,
           void ** value_pp,
           FileAttributeStatus *status);
    
    FileAttributeStatus getAttributeStatus(const char *attribute);
    
    QString getAttributeString(const char *attribute);
    
    QByteArray getAttributeByteString(const char *attribute);
    
    bool getAttributeBoolean(const char *attribute);
    
    quint32 getAttributeUInt32(const char *attribute);
    
    qint32 getAttributeInt32(const char *attribute);
    
    quint64 getAttributeUInt64(const char *attribute);
    
    qint64 getAttributeInt64(const char *attribute);
    
    QVariant getAttributeObject(const char *attribute);
    
    void setAttribute(const char *attribute, FileAttributeType type, QVariant value);
    
    void setAttributeString(const char *attribute, const QString & value);
    
    void setAttributeByteString(const char *attribute, const QByteArray & value);
    
    void setAttributeBoolean(const char *attribute, bool value);
    
    void setAttributeUInt32(const char *attribute, quint32 value);
    
    void setAttributeInt32(const char *attribute, qint32 value);
    
    void setAttributeUInt64(const char *attribute, quint64 value);
    
    void setAttributeInt64(const char *attribute, qint64 value);
    
    void clearStatus();
    
    FileType getFileType();
    
    bool isHidden();
    
    bool isBackup();
    
    bool isSymlink();
    
    QString getName() const;
    
    QString getDisplayName() const;
    
    QString getEditName() const;
    
    QIcon getIcon() const;
    
    QString getContentType() const;
    
    quint64 getSize();
    
    QDateTime getModificationTime() const;
    
    QString getSymlinkTarget() const;
    
    QString getEtag() const;
    
    qint32 getSortOrder();
    
    void setAttributeMask(const FileAttributeMatcher & mask);
    
    void unsetAttributeMask();
    
    void setFileType(FileType type);
    
    void setIsHidden(bool isHidden);
    
    void setIsSymlink(bool isSymlink);
    
    void setName(const QString & name);
    
    void setDisplayName(const QString & displayName);
    
    void setEditName(const QString & editName);
    
    void setIcon(const Icon & icon);
    
    void setContentType(const QString & contentType);
    
    void setSize(quint64 size);
    
    void setModificationTime(const QDateTime & time);
    
    void setSymlinkTarget(const QString & symlink_target);
    
    void setSortOrder(qint32 sortOrder);
    
private:

    class Private;
    friend class Private;
    QSharedDataPointer<Private> d;
};    

}}


#endif /*KGIOFILEINFO_H_*/
