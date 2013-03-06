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


#include <QtCore/QDate>
#include <QtCore/QString>

#include <glib.h>
#include <gio/gio.h>

#include "kgio-file.h"
#include "kgio-fileinfo.h"
#include "kgio-mount.h"
#include "kgio-wrappedobjectutils_p.h"
#include "kgio-themedicon.h"

namespace Kommodity {
namespace GIO {
    
class FileInfo::Private : public QSharedData
{
public:
    Private() 
    {
    };
};


FileInfo::FileInfo() : d(0)
{
    WO::initGIO();
    WO::setGFileInfo(this, g_file_info_new());
    
}


FileInfo::~FileInfo()
{
}

FileInfo::FileInfo( const FileInfo& other) : SharedWrappedObject(other), d(0)
{
}

FileInfo &FileInfo::operator=(const FileInfo &other)
{
    SharedWrappedObject::operator=(other);
    d = other.d;
    return *this;
}

FileInfo FileInfo::dup()
{
    FileInfo result;
    WO::setGFileInfo(&result, g_file_info_dup(WO::getGFileInfo(this)));
    return result;
}

void FileInfo::copyInto(FileInfo & destInfo)
{
    g_file_info_copy_into(WO::getGFileInfo(this),
            WO::getGFileInfo(&destInfo));
}

bool FileInfo::hasAttribute(const char *attribute)
{
    return g_file_info_has_attribute(WO::getGFileInfo(this),
            attribute);
}

QList<QByteArray> FileInfo::listAttributes(const char *name_space)
{
    return WO::qByteArrayListFromCharVector(
            g_file_info_list_attributes(WO::getGFileInfo(this),
                    name_space),
            true);
}

FileAttributeType  FileInfo::getAttributeType(const char *attribute)
{
    return (FileAttributeType) g_file_info_get_attribute_type(WO::getGFileInfo(this),
            attribute);
}

void FileInfo::removeAttribute(const char *attribute)
{
    g_file_info_remove_attribute(WO::getGFileInfo(this), attribute);
}

QString FileInfo::getAttributeAsString (const char *attribute)
{
    char* string =g_file_info_get_attribute_as_string (WO::getGFileInfo(this),
                                                             attribute);
    return QString(string);
    g_free(string);
}

bool FileInfo::getAttributeData(const char *attribute,
       FileAttributeType *type,
       void ** value_pp,
       FileAttributeStatus *status)
{
    GFileAttributeType gfaType;
    GFileAttributeStatus gfaStatus;
    
    return g_file_info_get_attribute_data(WO::getGFileInfo(this),
                 attribute,
                 &gfaType,
                 value_pp,
                 &gfaStatus);
    
    *type =  (FileAttributeType) gfaType;
    *status = (FileAttributeStatus) gfaStatus;
}

FileAttributeStatus FileInfo::getAttributeStatus(const char *attribute)
{
    return (FileAttributeStatus)
    g_file_info_get_attribute_status   (WO::getGFileInfo(this), attribute);
}

QString FileInfo::getAttributeString(const char *attribute)
{
    const char* string =g_file_info_get_attribute_string(WO::getGFileInfo(this),
                                                             attribute);
    return QString(string);
}

QByteArray FileInfo::getAttributeByteString(const char *attribute)
{
    const char* byteString = g_file_info_get_attribute_byte_string(WO::getGFileInfo(this),
                                                             attribute);

    return QByteArray(byteString);
}

bool FileInfo::getAttributeBoolean(const char *attribute)
{
    return g_file_info_get_attribute_boolean(WO::getGFileInfo(this), attribute);

}

quint32 FileInfo::getAttributeUInt32(const char *attribute)
{
    return g_file_info_get_attribute_uint32 (WO::getGFileInfo(this),
                                                             attribute);
}

qint32 FileInfo::getAttributeInt32(const char *attribute)
{
    return g_file_info_get_attribute_int32(WO::getGFileInfo(this),
                                                             attribute);
    
}

quint64 FileInfo::getAttributeUInt64(const char *attribute)
{
    return g_file_info_get_attribute_uint64(WO::getGFileInfo(this),
                                                             attribute);
    
}

qint64 FileInfo::getAttributeInt64(const char *attribute)
{
    return g_file_info_get_attribute_int64(WO::getGFileInfo(this),
                                                             attribute);
    
}

//QVariant FileInfo::getAttributeObject(const char *attribute)
//{
//    
//}
//
//void FileInfo::setAttribute(const char *attribute, FileInfo::FileAttributeType type, QVariant value)
//{
//    
//}

void FileInfo::setAttributeString(const char *attribute, const QString & value)
{
    g_file_info_set_attribute_string(WO::getGFileInfo(this),
                                                             attribute,
                                                             value.toUtf8().data());
    
}

void FileInfo::setAttributeByteString(const char *attribute, const QByteArray & value)
{
    g_file_info_set_attribute_byte_string(WO::getGFileInfo(this),
                                                             attribute,
                                                             value.data());
}

void FileInfo::setAttributeBoolean(const char *attribute, bool value)
{
    g_file_info_set_attribute_boolean(WO::getGFileInfo(this),
                                                             attribute,
                                                             value);
    
}

void FileInfo::setAttributeUInt32(const char *attribute, quint32 value)
{
    g_file_info_set_attribute_uint32(WO::getGFileInfo(this),
                                                             attribute,
                                                             value);
    
}

void FileInfo::setAttributeInt32(const char *attribute, qint32 value)
{
    g_file_info_set_attribute_int32(WO::getGFileInfo(this),
                                                             attribute,
                                                             value);
    
}

void FileInfo::setAttributeUInt64(const char *attribute, quint64 value)
{
    g_file_info_set_attribute_uint64(WO::getGFileInfo(this),
                                                             attribute,
                                                             value);
    
}

void FileInfo::setAttributeInt64(const char *attribute, qint64 value)
{
    g_file_info_set_attribute_int64(WO::getGFileInfo(this),
                                                             attribute,
                                                             value);
    
}

void FileInfo::clearStatus()
{
    g_file_info_clear_status(WO::getGFileInfo(this));

}

FileType FileInfo::getFileType()
{
    return (FileType)g_file_info_get_file_type(WO::getGFileInfo(this));
    
}

bool FileInfo::isHidden()
{
    return g_file_info_get_is_hidden(WO::getGFileInfo(this));
    
}

bool FileInfo::isBackup()
{
    return g_file_info_get_is_backup(WO::getGFileInfo(this));
    
}

bool FileInfo::isSymlink()
{
    return g_file_info_get_is_symlink(WO::getGFileInfo(this));
    
}

QString FileInfo::getName() const
{
    const char* name = g_file_info_get_name(WO::getGFileInfo(this));
    return QString(name);
}

QString FileInfo::getDisplayName() const
{
    const char* displayName = g_file_info_get_display_name(WO::getGFileInfo(this));
    return QString(displayName);
}

QString FileInfo::getEditName() const
{
    const char* editName = g_file_info_get_edit_name(WO::getGFileInfo(this));
    return QString(editName);
}

Icon *  FileInfo::getIcon() const
{
    GIcon* gIcon = g_file_info_get_icon(WO::getGFileInfo(this));
    if (gIcon==NULL) return NULL;
    g_object_ref(gIcon);
    
    Icon * icon = NULL;
    
    if (G_IS_THEMED_ICON(gIcon))
    {
        icon = new ThemedIcon();
    }
    else if(G_IS_FILE_ICON(gIcon))
    {
        icon = new FileIcon();
    } 
    else if(G_IS_LOADABLE_ICON(gIcon))
    {
        icon = new LoadableIcon();
    } 
    else
    {
        return NULL;
    }
    
    WO::setGIcon(icon, gIcon);
    return icon;
}

QString FileInfo::getContentType() const
{
    const char* contentType = g_file_info_get_content_type(WO::getGFileInfo(this));
    return QString(contentType);
    
}

quint64 FileInfo::getSize()
{
    return g_file_info_get_size(WO::getGFileInfo(this));
}

QDateTime FileInfo::getModificationTime() const
{
    GTimeVal gTimeVal;
    g_file_info_get_modification_time(WO::getGFileInfo(this), &gTimeVal);
    
    QDateTime result;
    result.setTime_t ( gTimeVal.tv_sec );
    result.addMSecs ( gTimeVal.tv_usec/1000 );
    return result;
}

QString FileInfo::getSymlinkTarget() const
{
    const char* symlinkTarget = g_file_info_get_symlink_target(WO::getGFileInfo(this));
    return QString(symlinkTarget);
}

QString FileInfo::getEtag() const
{
    const char* etag = g_file_info_get_etag(WO::getGFileInfo(this));
    return QString(etag);
}

qint32 FileInfo::getSortOrder()
{
    return g_file_info_get_sort_order(WO::getGFileInfo(this));
}

void FileInfo::setAttributeMask(const FileAttributeMatcher & mask)
{
//    g_file_info_set_attribute_mask(WO::getGFileInfo(this),
//           GFileAttributeMatcher *mask);
    // TODO
}

void FileInfo::unsetAttributeMask()
{
    g_file_info_unset_attribute_mask(WO::getGFileInfo(this));
    
}

void FileInfo::setFileType(FileType type)
{
    g_file_info_set_file_type(WO::getGFileInfo(this),
                  (GFileType) type);
    
}

void FileInfo::setIsHidden(bool isHidden)
{
    g_file_info_set_is_hidden(WO::getGFileInfo(this), isHidden);
    
}

void FileInfo::setIsSymlink(bool isSymlink)
{
    g_file_info_set_is_symlink(WO::getGFileInfo(this), isSymlink);
    
}

void FileInfo::setName(const QString & name)
{
    g_file_info_set_name(WO::getGFileInfo(this),name.toUtf8().data());
}

void FileInfo::setDisplayName(const QString & displayName)
{
    g_file_info_set_display_name(WO::getGFileInfo(this), displayName.toUtf8().data());
    
}

void FileInfo::setEditName(const QString & editName)
{
    g_file_info_set_edit_name(WO::getGFileInfo(this), editName.toUtf8().data());
    
}

void FileInfo::setIcon(const Icon & icon)
{
//    void                g_file_info_set_icon                (GFileInfo *info,
//                                                             GIcon *icon);
    // TODO
}

void FileInfo::setContentType(const QString & contentType)
{
    g_file_info_set_content_type(WO::getGFileInfo(this), contentType.toUtf8().data());
    
}

void FileInfo::setSize(quint64 size)
{
    g_file_info_set_size(WO::getGFileInfo(this), size);
    
}

void FileInfo::setModificationTime(const QDateTime & time)
{
    GTimeVal mtime;
    
    mtime.tv_sec = time.toTime_t();
    mtime.tv_usec = 0; // TODO
    
    g_file_info_set_modification_time(WO::getGFileInfo(this), &mtime);
    
}

void FileInfo::setSymlinkTarget(const QString & symlinkTarget)
{
    g_file_info_set_symlink_target(WO::getGFileInfo(this), symlinkTarget.toUtf8().data());
}

void FileInfo::setSortOrder(qint32 sortOrder)
{
    g_file_info_set_sort_order(WO::getGFileInfo(this), sortOrder);
}


}}

#include "moc_kgio-fileinfo.cpp"

