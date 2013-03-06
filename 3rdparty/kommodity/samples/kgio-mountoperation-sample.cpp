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
#include <QInputDialog>

#include "kgio-mountoperation-sample.h"

using namespace Kommodity::GIO;

MyDemoApplication::MyDemoApplication(const QString & fileName)
{
     Error error;
     textEdit = new QTextEdit;
     textEdit->setAcceptRichText(false);
     setCentralWidget(textEdit);

     QUrl url = QUrl(fileName);

     File file(url);

     // first stat file to find out if its already mounted
     FileInfo fileInfo = file.queryInfo("*", File::QueryInfoNorm, error);

     if (error.hasError())
     {
         if (!error.isIOError(IOError::NotMounted))
         {
             cerr << "error: " << error.getMessage().toUtf8().data() << "\n";
             return;
         }
         // not mounted -> continue with mounting
     }
     else
     {
         // no error -> already mounted
         listDir(file);
         return;
     }

     connect(&m_mountOperation, SIGNAL(
             askPassword(MountOperation *, const QString &, const QString &, const QString &, MountOperation::AskPasswordFlags)),
             this, SLOT(askPassword(MountOperation *, const QString &, const QString &, const QString &, MountOperation::AskPasswordFlags)));

     connect(&m_mountOperation, SIGNAL(
             replied(MountOperation *, MountOperation::MountOperationResult)),
             this, SLOT(replied(MountOperation *, MountOperation::MountOperationResult)));

     file.mountEnclosingVolumeAsync(m_mountOperation,
             Mount::MountNone,
             this,
             SLOT(mountEnclosingVolumeReady(const File & , const Error & )));


}



void MyDemoApplication::mountEnclosingVolumeReady(const File & file, const Error & error)
{
    cout << "mount ready\n";
    if (!error.hasError())
    {
        listDir(file);
    }
    else
    {
        cout << "mount error " << error.getMessage().toUtf8().data() << "\n";
    }
}

void MyDemoApplication::askPassword(MountOperation *op,
             const QString & message,
             const QString & defaultUser,
             const QString & defaultDomain,
             MountOperation::AskPasswordFlags flags)
{
    cout << "ask password" << "\n";
    bool ok;
    QString password = QInputDialog::getText(this, "Enter Password",
                                         message, QLineEdit::Normal,
                                         QString(), &ok);
    if (ok && !password.isEmpty())
    {
        op->setPassword(password);
        op->reply(MountOperation::MountOperationHandeled);
    }
    else
    {
        op->reply(MountOperation::MountOperationAborted);
    }
}

void MyDemoApplication::replied(MountOperation *op, MountOperation::MountOperationResult abort)
{
    cout << "mount opertion replied: " << abort << "\n";
}

void MyDemoApplication::listDir(const File & file)
{
    File myFile = file;
    Error error;

    QString str;
    QTextStream textStream(&str);

    FileEnumerator fileEnumerator = myFile.enumerateChildren(
            "*",
            File::QueryInfoNorm,
            error
    );

    if (error.hasError())
    {
        cerr << "error: " << error.getMessage().toUtf8().data() << "\n";
        return;
    }
    else
    {
        for (;;)
        {
            FileInfo fileInfo = fileEnumerator.nextFile(error);
            if (error.hasError())
            {
                cerr << "error: " << error.getMessage().toUtf8().data() << "\n";
                return;
            }

            if (fileInfo.isNull())
            {
                break;  // finished
            }

            textStream << fileInfo.getName()
                << "\t" << fileInfo.getSize()
                << "\t" << fileInfo.getContentType() << "\n";

        }
        fileEnumerator.close(error);
        if (error.hasError())
        {
            cerr << "error: " << error.getMessage().toUtf8().data() << "\n";
            return;
        }
    }
    textEdit->document()->setPlainText(*textStream.string());
}



int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    bool ok;
    QString urlStr = QInputDialog::getText(0, "mountoperation-sample",
                                         "Enter url to enumerate", QLineEdit::Normal,
                                         QString(), &ok);
    if (ok && !urlStr.isEmpty())
    {
        MyDemoApplication demoApp(urlStr);
        demoApp.show();
        return app.exec();
    }
}


#include "kgio-mountoperation-sample.moc"

