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

#ifndef KGIOREADFILEASYNCTEST_H_
#define KGIOREADFILEASYNCTEST_H_

#include <QtGui>
#include <QApplication>
#include <QPushButton>
#include <QString>
#include <QUrl>
#include <Kommodity/GIO/File>
#include <Kommodity/GIO/MountOperation>

using namespace Kommodity::GIO;

class MyDemoApplication : public QMainWindow
{
    Q_OBJECT
public:
    MyDemoApplication(const QString & fileName);
    QTextEdit *textEdit;

    MountOperation m_mountOperation;
    File m_file;

    void listDir(const File & file);

public Q_SLOTS:

    void mountEnclosingVolumeReady(const File & file, const Error & error);

    void askPassword(MountOperation *op,
                 const QString & message,
                 const QString & defaultUser,
                 const QString & defaultDomain,
                 MountOperation::AskPasswordFlags flags);

    void replied(MountOperation *op, MountOperation::MountOperationResult abort);

};



#endif /*KGIOREADFILEASYNCTEST_H_*/
