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

#include "kgio-fileicon.h"
#include "kgio-wrappedobjectutils_p.h"

namespace Kommodity {
namespace GIO {
    
class FileIcon::Private : public QSharedData
{
public:
    Private() 
    {
    };
};

FileIcon::FileIcon() : d(0)
{
    WO::initGIO();
}


FileIcon::~FileIcon()
{
}

FileIcon::FileIcon(const File& file)
{
    WO::setGIcon(this, g_file_icon_new(WO::getGFile(&file)));
}

File FileIcon::getFile() const
{
    File file;
    WO::setGFile(&file, g_file_icon_get_file(WO::getGFileIcon(this)));
    return file;
}

}}

#include "moc_kgio-fileicon.cpp"
