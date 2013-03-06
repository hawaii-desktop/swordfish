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
    Error error;

    File file(QUrl("http://dot.kde.org/"));
    
    FileInputStream * in = file.read(error);
    
    if (error.hasError())
    {
        cerr << "error: " << error.getMessage().toUtf8().data() << "\n";
        return 1;
    }
    else
    {
        for (;;)
        {
            char buffer[81];
        
            qint64 n = in->read(buffer, 80, error);
            if (error.hasError())
            {
                cerr << "error: " << error.getMessage().toUtf8().data() << "\n";
                return 1;
            }
    
            if (n<1) break;
            buffer[n]='\0';
            
            cout << buffer;
            
        }
        in->close(error);
        if (error.hasError())
        {
            cerr << "error: " << error.getMessage().toUtf8().data() << "\n";
            return 1;
        }
    }
    
    delete in;
    
    return 0;
}
