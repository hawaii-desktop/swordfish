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


#ifndef KGIOFILEATTRIBUTE_H_
#define KGIOFILEATTRIBUTE_H_

#include <QtCore/QString>

enum FileAttributeType {
    FileAttributeTypeInvalid = 0,
    FileAttributeTypeString,
    FileAttributeTypeByteString, /* zero terminated string of non-zero bytes */
    FileAttributeTypeBool,
    FileAttributeTypeUInt32,
    FileAttributeTypeInt32,
    FileAttributeTypeUInt64,
    FileAttributeTypeInt64,
    FileAttributeTypeObject
};

enum FileType {
    FileTypeUnknown = 0,
    FileTypeRegular,
    FileTypeDirectory,
    FileTypeSymbolicLink,
    FileTypeSpecial, /* socket, fifo, blockdev, chardev */
    FileTypeShortcut,
    FileTypeMountable
};

enum FileAttributeStatus {
    FileAttributeStatusUnset = 0,
    FileAttributeStatusSet,
    FileAttributeStatusErrorSetting
};

enum FileAttributeInfoFlags {
    FileAttributeInfoNone = 0,
    FileAttributeInfoCopyWithFile = 1 << 0,
    FileAttributeInfoCopyWhenMoved = 1 << 1
};



struct FileAttributeInfo {
  QString name;
  FileAttributeType type;
  FileAttributeInfoFlags flags;
};



#endif /*KGIOFILEATTRIBUTE_H_*/
