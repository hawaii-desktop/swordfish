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

    File file("/tmp/kgio_testfile.txt");
    
    FileOutputStream * out;
    
    if (!file.queryExists())
    {
        out = file.create(File::CreateNorm , error);
    }
    else
    {
        out = file.replace(QString(), true, File::CreateNorm , error);
    }
    
    if (error.hasError())
    {
        cerr << "error: " << error.getMessage().toUtf8().data() << "\n";
        return 1;
    }
    else
    {
        for (int i=0;i<20;i++)
        {

            QByteArray line("........ test data ........\n");
            
            quint64 bytesWritten;
            
            out->writeAll((const void*)line.data(),
                         (quint64)line.size(),
                         bytesWritten,
                         error);
            
            if (error.hasError())
            {
                cerr << "error: " << error.getMessage().toUtf8().data() << "\n";
                break;
            }
         }
        out->close(error);
        if (error.hasError())
        {
            cerr << "error: " << error.getMessage().toUtf8().data() << "\n";
            return 1;
        }
    }
    
    delete out;
    
    // ----- Query file-info
    
    FileInfo fileInfo = file.queryInfo("*", File::QueryInfoNorm, error);

    if (error.hasError())
    {
        cerr << "error: " << error.getMessage().toUtf8().data() << "\n";
        return 1;
    }    
    
    cout << "fileinfo: name=" << fileInfo.getName().toUtf8().data() << " content-type=" << fileInfo.getContentType().toUtf8().data() << "\n"; 
    
    // ----- Now - let's launch the default editor for this file
    
    AppInfo appInfo = file.queryDefaultHandler(error);

    if (error.hasError())
    {
        cerr << "error: " << error.getMessage().toUtf8().data() << "\n";
        return 1;
    }
    
    cout << "appinfo: name=" << appInfo.getName().toUtf8().data() << " description=" << appInfo.getDescription().toUtf8().data() << "\n"; 
    
    AppLaunchContext launchContext;
    QList<File> launchFiles;
    launchFiles.append(file);
    
    appInfo.launch(launchFiles, launchContext, error);
    
    if (error.hasError())
    {
        cerr << "error: " << error.getMessage().toUtf8().data() << "\n";
        return 1;
    }    
    
    return 0;
}
