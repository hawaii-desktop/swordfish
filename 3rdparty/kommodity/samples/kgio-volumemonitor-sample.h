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

#include <QtWidgets>
#include <QApplication>
#include <QPushButton>
#include <QString>
#include <QUrl>
#include <Kommodity/GIO/VolumeMonitor>
#include <Kommodity/GIO/Volume>
#include <Kommodity/GIO/Mount>
#include <Kommodity/GIO/Drive>

using namespace Kommodity::GIO;

class MyDemoApplication : public QMainWindow
{
    Q_OBJECT
public:
    MyDemoApplication();
    
    
    
    QTextEdit *textEdit;

    VolumeMonitor * m_volumeMonitor;
    Cancellable * m_cancellable;
    
    void updateList();
    
    
public Q_SLOTS:

    void driveChanged(const VolumeMonitor * volumeMonitor, const Drive * drive);
    void driveConnected(const VolumeMonitor * volumeMonitor, const Drive * drive);
    void driveDisconnected(const VolumeMonitor * volumeMonitor, const Drive * drive);
    void mountAdded(const VolumeMonitor * volumeMonitor, const Mount * mount);
    void mountChanged(const VolumeMonitor * volumeMonitor, const Mount * mount);
    void preUnmount(const VolumeMonitor * volumeMonitor, const Mount * mount);
    void mountRemoved(const VolumeMonitor * volumeMonitor, const Mount * mount);
    void volumeAdded(const VolumeMonitor * volumeMonitor, const Volume * volume);
    void volumeChanged(const VolumeMonitor * volumeMonitor, const Volume * volume);
    void volumeRemoved(const VolumeMonitor * volumeMonitor, const Volume * volume);
    
    
    
};



#endif /*KGIOREADFILEASYNCTEST_H_*/
