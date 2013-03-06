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

#include "kgio-volumemonitor-sample.h"

using namespace Kommodity::GIO;

MyDemoApplication::MyDemoApplication()
{
     textEdit = new QTextEdit;
     textEdit->setAcceptRichText(false);
     setCentralWidget(textEdit);

     m_volumeMonitor = new VolumeMonitor();
     
     connect(m_volumeMonitor, SIGNAL(driveChanged(const VolumeMonitor * , const Drive * )),
                  this, SLOT(driveChanged(const VolumeMonitor * , const Drive * )));

     connect(m_volumeMonitor, SIGNAL(driveConnected(const VolumeMonitor * , const Drive * )),
                  this, SLOT(driveConnected(const VolumeMonitor * , const Drive * )));

     connect(m_volumeMonitor, SIGNAL(driveDisconnected(const VolumeMonitor * , const Drive * )),
                  this, SLOT(driveDisconnected(const VolumeMonitor * , const Drive * )));

     connect(m_volumeMonitor, SIGNAL(mountAdded(const VolumeMonitor * , const Mount * )),
                  this, SLOT(mountAdded(const VolumeMonitor * , const Mount * )));

     connect(m_volumeMonitor, SIGNAL(mountChanged(const VolumeMonitor * , const Mount * )),
                  this, SLOT(mountChanged(const VolumeMonitor * , const Mount * )));

     connect(m_volumeMonitor, SIGNAL(preUnmount(const VolumeMonitor * , const Mount * )),
                  this, SLOT(preUnmount(const VolumeMonitor * , const Mount * )));

     connect(m_volumeMonitor, SIGNAL(mountRemoved(const VolumeMonitor * , const Mount * )),
                  this, SLOT(mountRemoved(const VolumeMonitor * , const Mount * )));

     connect(m_volumeMonitor, SIGNAL(volumeAdded(const VolumeMonitor * , const Volume * )),
                  this, SLOT(volumeAdded(const VolumeMonitor * , const Volume * )));

     connect(m_volumeMonitor, SIGNAL(volumeChanged(const VolumeMonitor * , const Volume * )),
                  this, SLOT(volumeChanged(const VolumeMonitor * , const Volume * )));

     connect(m_volumeMonitor, SIGNAL(volumeRemoved(const VolumeMonitor * , const Volume * )),
                  this, SLOT(volumeRemoved(const VolumeMonitor * , const Volume * )));

     updateList();
     
}

void MyDemoApplication::driveChanged(const VolumeMonitor * volumeMonitor, const Drive * drive)
{
    cout << "drive changed: " << drive->getName().toUtf8().data() << "\n";
    updateList();
}

void MyDemoApplication::driveConnected(const VolumeMonitor * volumeMonitor, const Drive * drive)
{
    cout << "drive connected: " << drive->getName().toUtf8().data() << "\n";
    updateList();
}

void MyDemoApplication::driveDisconnected(const VolumeMonitor * volumeMonitor, const Drive * drive)
{
    cout << "drive disconnected: " << drive->getName().toUtf8().data() << "\n";
    updateList();
}

void MyDemoApplication::mountAdded(const VolumeMonitor * volumeMonitor, const Mount * mount)
{
    cout << "mount added: " << mount->getName().toUtf8().data() << "\n";
    updateList();
}

void MyDemoApplication::mountChanged(const VolumeMonitor * volumeMonitor, const Mount * mount)
{
    cout << "mount changed: " << mount->getName().toUtf8().data() << "\n";
    updateList();
}

void MyDemoApplication::preUnmount(const VolumeMonitor * volumeMonitor, const Mount * mount)
{
    cout << "pre unmount: " << mount->getName().toUtf8().data() << "\n";
    updateList();
}

void MyDemoApplication::mountRemoved(const VolumeMonitor * volumeMonitor, const Mount * mount)
{
    cout << "mount removed: " << mount->getName().toUtf8().data() << "\n";
    updateList();
}

void MyDemoApplication::volumeAdded(const VolumeMonitor * volumeMonitor, const Volume * volume)
{
    cout << "volume added: " << volume->getName().toUtf8().data() << "\n";
    updateList();
}

void MyDemoApplication::volumeChanged(const VolumeMonitor * volumeMonitor, const Volume * volume)
{
    cout << "volume changed: " << volume->getName().toUtf8().data() << "\n";
    updateList();
}

void MyDemoApplication::volumeRemoved(const VolumeMonitor * volumeMonitor, const Volume * volume)
{
    cout << "volume removed: " << volume->getName().toUtf8().data() << "\n";
    updateList();
}

void MyDemoApplication::updateList()
{
    QString str;
    QTextStream textStream(&str);
    
    textStream << "=== Mounts ===\n";    
    QList<Mount*> mounts = m_volumeMonitor->getMounts();
    
    for (int i = 0; i < mounts.size(); ++i) {
        Mount * mount = mounts.at(i);
        textStream << mount->getName() 
            << " root=" << mount->getRoot().getUri().toString()
            << "\n";
    }

    textStream << "\n";    
    textStream << "=== Drives ===\n";    
    QList<Drive*> drives = m_volumeMonitor->getConnectedDrives();
    
    for (int i = 0; i < drives.size(); ++i) {
        Drive * drive = drives.at(i);
        textStream << drive->getName() << "\n";
    }    

    textStream << "\n";    
    textStream << "=== Volumes ===\n";    
    QList<Volume*> volumes = m_volumeMonitor->getVolumes();
    
    for (int i = 0; i < volumes.size(); ++i) {
        Volume * volume = volumes.at(i);
        textStream << volume->getName() << "\n";
    }    
    
    textEdit->document()->setPlainText(*textStream.string());
    
}

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    MyDemoApplication demoApp;
    demoApp.show();
    return app.exec();
}


#include "kgio-volumemonitor-sample.moc"

