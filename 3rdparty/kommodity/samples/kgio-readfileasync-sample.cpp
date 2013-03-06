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

#include <iostream>
using namespace std;


#include <QApplication>
#include <QPushButton>
#include <QString>
#include <QUrl>

#include <Kommodity/GIO/File>

#include "kgio-readfileasync-sample.h"

using namespace Kommodity::GIO;

MyDemoApplication::MyDemoApplication()
{
     textEdit = new QTextEdit;
     textEdit->setAcceptRichText(false);
     setCentralWidget(textEdit);
     QPushButton * cancelButton = new QPushButton("cancel");
     connect(cancelButton, SIGNAL(pressed()),
                  this, SLOT(cancelButtonPressed()));

     setMenuWidget(cancelButton);

     File file(QUrl("http://dot.kde.org/"));
     //File file("/etc/passwd");
     Q_ASSERT(!file.isNull());

     file.queryInfoAsync("*",
             File::QueryInfoNorm,
             this,
             SLOT(queryInfoReady(const File &, const FileInfo & , const Error & )));

     file.readAsync(
             this,
             SLOT(inputStreamReady(const File&, FileInputStream *, const Error &)),
             &m_cancellable);

}

void MyDemoApplication::queryInfoReady(const File & file, const FileInfo & fileInfo, const Error & error)
{

    if (error.hasError())
    {
        cerr << "query-info error: " << error.getMessage().toUtf8().data() << "\n";
        return;
    }

    cerr << "queryInfoReady name: " << fileInfo.getName().toUtf8().data() << endl;


}



void MyDemoApplication::inputStreamReady(const File & file, FileInputStream * inputStream, const Error & error)
{
    if (error.hasError())
    {
        cout << "open error: " << error.getMessage().toUtf8().data() << "\n";
    }
    else
    {
        m_fileInputStream = inputStream;
        connect(m_fileInputStream, SIGNAL(readAsyncReady(InputStream * , const qint64, const Error &)),
                     this, SLOT(readAsyncReady(InputStream * , const qint64, const Error &)));
        connect(m_fileInputStream, SIGNAL(closeAsyncReady(InputStream *, const Error &)),
                     this, SLOT(closeAsyncReady(InputStream *, const Error &)));

        m_fileInputStream->readAsync(buffer, 80, 100, &m_cancellable);
    }
}

void MyDemoApplication::readAsyncReady(InputStream * inputStream, const qint64 count, const Error & error)
{
    if (error.hasError())
    {
        cout << "read error: " << error.getMessage().toUtf8().data() << "\n";
        m_fileInputStream->closeAsync(100);
    }
    else if (count<1)
    {
        // finished!

        m_fileInputStream->closeAsync(100);
    }
    else
    {
        QString chunk = QString::fromUtf8 (buffer, count);
        //cout << chunk.toUtf8().data();
        textEdit->append ( chunk );
        m_fileInputStream->readAsync(buffer, 80, 100, &m_cancellable);
    }
}

void MyDemoApplication::closeAsyncReady(InputStream * inputStream, const Error & error)
{
    if (error.hasError())
    {
        cout << "close error: " << error.getMessage().toUtf8().data() << "\n";
    }
    cout << "closed\n";
}

void MyDemoApplication::cancelButtonPressed()
{
    cout << "cancel pressed\n";
    m_cancellable.cancel();
}

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    MyDemoApplication demoApp;
    demoApp.show();
    return app.exec();
}


#include "kgio-readfileasync-sample.moc"

