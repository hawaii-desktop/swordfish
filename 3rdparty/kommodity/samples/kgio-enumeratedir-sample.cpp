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
#include <Kommodity/GIO/File>
#include <iostream>

using namespace std;
using namespace Kommodity::GIO;

int main(int argc, char *argv[])
{
    const char *path = argv[1] ? argv[1] : "/etc";

    Error error;

    File file(path);
    
    FileEnumerator fileEnumerator = file.enumerateChildren(
            "*",  
            File::QueryInfoNorm,
            error
    );
    
    if (error.hasError())
    {
        cerr << "error: " << error.getMessage().toUtf8().data() << "\n";
        return 1;
    }
    else
    {
        for (;;)
        {
            FileInfo fileInfo = fileEnumerator.nextFile(error);
            if (error.hasError())
            {
                cerr << "error: " << error.getMessage().toUtf8().data() << "\n";
                return 1;
            }
    
            if (fileInfo.isNull())
            {
                break;  // finished
            }
            
            cout << fileInfo.getName().toUtf8().data()
                << "\t" << fileInfo.getSize()
                << "\t" << fileInfo.getContentType().toUtf8().data() << "\n";
            
        }
        fileEnumerator.close(error);
        if (error.hasError())
        {
            cerr << "error: " << error.getMessage().toUtf8().data() << "\n";
            return 1;
        }
    }
    return 0;
}
