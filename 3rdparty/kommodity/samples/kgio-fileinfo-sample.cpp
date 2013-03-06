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

#include <QString>
#include <QUrl>
#include <QTextStream>
#include <Kommodity/GIO/File>
#include <Kommodity/GIO/FileInfo>
#include <Kommodity/GIO/ThemedIcon>
#include <Kommodity/GIO/FileIcon>
#include <Kommodity/GIO/LoadableIcon>

using namespace Kommodity::GIO;

int main(int argc, char *argv[])
{
    QTextStream cout(stdout, QIODevice::WriteOnly);
    QTextStream cerr(stderr, QIODevice::WriteOnly);
    
    
    Error error;

    File file("/tmp");
    FileInfo fileInfo = file.queryInfo("*", File::QueryInfoNorm, error);
    
    if (error.hasError())
    {
        cerr << "error: " << error.getMessage() << "\n";
        return 1;
    }    
    
    cout << "\nAttribute values:\n";
    
    Q_FOREACH ( QByteArray attribute, fileInfo.listAttributes(0) )
    {
        cout << "" << QString(attribute).leftJustified(30);

        switch (fileInfo.getAttributeType(attribute.data()))
        {
        
            case FileAttributeTypeInvalid:
                cout << "Invalid";
                break;
            case FileAttributeTypeString:
                cout << "String\t" << fileInfo.getAttributeString(attribute.data());
                break;                
            case FileAttributeTypeByteString:
                cout << "ByteStr\t" << fileInfo.getAttributeByteString(attribute.data());
                break;                
            case FileAttributeTypeBool:
                cout << "Bool\t" << fileInfo.getAttributeBoolean(attribute.data());
                break;                
            case FileAttributeTypeUInt32:
                cout << "UInt32\t" << fileInfo.getAttributeUInt32(attribute.data());
                break;                
            case FileAttributeTypeInt32:
                cout << "Int32\t" << fileInfo.getAttributeInt32(attribute.data());
                break;                
            case FileAttributeTypeUInt64:
                cout << "UInt64\t" << fileInfo.getAttributeUInt64(attribute.data());
                break;                
            case FileAttributeTypeInt64:
                cout << "Int64\t" << fileInfo.getAttributeInt64(attribute.data());
                break;                
            case FileAttributeTypeObject:
                cout << "Object";
                break;
            default:
                cout << "Unknown Type";
        }
        
        cout << "\n";
        
    }
    
    cout << "\nSettable attributes:\n";
    
    QList<FileAttributeInfo> attributeInfoList = file.querySettableAttributes(error);
    
    if (error.hasError())
    {
        cerr << "error: " << error.getMessage() << "\n";
        return 1;
    }    
    
    Q_FOREACH(FileAttributeInfo attributeInfo , attributeInfoList)
    {
        cout << attributeInfo.name << "\n";
    }
    
    ///// Icon
    
    Icon * icon = fileInfo.getIcon();
    
    ThemedIcon * themedIcon = dynamic_cast<ThemedIcon *> (icon); 
    if (themedIcon!=0)
    {
        cout << "\nThemedIcon:\n"; 
        
        Q_FOREACH(QString name , themedIcon->getNames())
        {
            cout << name << "\n";
        }
        
        cout << "\n";
    }

    LoadableIcon * loadableIcon = dynamic_cast<LoadableIcon *> (icon); 
    if (loadableIcon!=0)
    {
        cout << "\nLoadableIcon\n"; 
    }
    
    FileIcon * fileIcon = dynamic_cast<FileIcon *> (icon); 
    if (fileIcon!=0)
    {
        cout << "\nFileIcon:" << fileIcon->getFile().getPath() << "\n"; 
    }
    
    delete icon;
    
    return 0;
}
