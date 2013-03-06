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
#include <QFile>
#include <QDomDocument>
#include <Kommodity/GIO/File>
#include <Kommodity/GIO/FileDevice>

using namespace Kommodity::GIO;

int main(int argc, char *argv[])
{
    QTextStream cout(stdout, QIODevice::WriteOnly);
    QTextStream cerr(stderr, QIODevice::WriteOnly);
    
    
    Error error;

    File file(QUrl("http://planetkde.org/rss20.xml"));

    FileDevice fileDevice(file);
    
    fileDevice.open(QIODevice::ReadOnly);
    
    QString errorMsg;
    QDomDocument doc("mydocument");
    if (!doc.setContent(&fileDevice, &errorMsg)) {
        fileDevice.close();
        cout << "doc.setContent error: " << errorMsg << "\n";
        return 1;
    }
    fileDevice.close();

    QDomElement docElem = doc.documentElement();
    QDomNode n = docElem.firstChild();
    while(!n.isNull()) {
        QDomElement e = n.toElement(); 
        if(!e.isNull()) {
            cout << e.tagName() << endl; 
        }
        n = n.nextSibling();
    }
    
}
