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


#ifndef KGIOVOLUMEMONITOR_H_
#define KGIOVOLUMEMONITOR_H_

#include <QtCore/QObject>
#include <QtCore/QString>
#include <QtCore/QSharedData>

#include <Kommodity/GIO/WrappedObject>
#include <Kommodity/GIO/Error>
#include <Kommodity/GIO/Icon>
#include <Kommodity/GIO/Mount>

namespace Kommodity {
namespace GIO {
    
class Mount;
    
class Q_DECL_EXPORT VolumeMonitor : public QObject, public virtual WrappedObject
{
    Q_OBJECT

public:
    
    VolumeMonitor();
    virtual ~VolumeMonitor();
    
    QList<Drive*> getConnectedDrives() const;
    QList<Volume*> getVolumes() const;
    QList<Mount*> getMounts() const;
    static Volume *adoptOrphanMount(const Mount * mount);
    Mount* getMountForUuid (const QByteArray & uuid);
    Volume* getVolumeForUuid(const QByteArray & uuid);

Q_SIGNALS:    
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
    
private:

    class Private;
    friend class Private;
    Private * d;
    
public: // internal
    
    void emitDriveChanged(const VolumeMonitor * volumeMonitor, const Drive * drive);
    void emitDriveConnected(const VolumeMonitor * volumeMonitor, const Drive * drive);
    void emitDriveDisconnected(const VolumeMonitor * volumeMonitor, const Drive * drive);
    void emitMountAdded(const VolumeMonitor * volumeMonitor, const Mount * mount);
    void emitMountChanged(const VolumeMonitor * volumeMonitor, const Mount * mount);
    void emitPreUnmount(const VolumeMonitor * volumeMonitor, const Mount * mount);
    void emitMountRemoved(const VolumeMonitor * volumeMonitor, const Mount * mount);
    void emitVolumeAdded(const VolumeMonitor * volumeMonitor, const Volume * volume);
    void emitVolumeChanged(const VolumeMonitor * volumeMonitor, const Volume * volume);
    void emitVolumeRemoved(const VolumeMonitor * volumeMonitor, const Volume * volume);
    
};

}}
#endif /*KGIOVOLUMEMONITOR_H_*/
