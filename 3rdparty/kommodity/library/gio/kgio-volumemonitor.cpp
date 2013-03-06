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

#include <QtCore/QString>
#include <QtCore/QPointer>

#include <glib.h>
#include <gio/gio.h>

#include "kgio-volumemonitor.h"
#include "kgio-volume.h"
#include "kgio-drive.h"
#include "kgio-wrappedobjectutils_p.h"

namespace Kommodity {
namespace GIO {
    
/////

static void  _drive_changed_handler(GVolumeMonitor *volume_monitor,
        GDrive         *drive,
        gpointer        user_data)
{
    VolumeMonitor * p = (VolumeMonitor*)user_data;
    Drive * eDrive = new Drive;
    g_object_ref(drive);
    WO::setGDrive(eDrive, drive);
    p->emitDriveChanged(p, eDrive);
}
                                                        
static void _drive_connected_handler(GVolumeMonitor *volume_monitor,
        GDrive         *drive,
        gpointer        user_data)
{
    VolumeMonitor * p = (VolumeMonitor*)user_data;
    Drive * eDrive = new Drive;
    g_object_ref(drive);
    WO::setGDrive(eDrive, drive);
    p->emitDriveConnected(p, eDrive);
}
        
static void _drive_disconnected_handler(GVolumeMonitor *volume_monitor,
        GDrive         *drive,
        gpointer        user_data)
{
    VolumeMonitor * p = (VolumeMonitor*)user_data;
    Drive * eDrive = new Drive;
    g_object_ref(drive);
    WO::setGDrive(eDrive, drive);
    p->emitDriveDisconnected(p, eDrive);
}
        
static void _mount_added_handler(GVolumeMonitor *volume_monitor,
        GMount         *mount,
        gpointer        user_data)
{
    VolumeMonitor * p = (VolumeMonitor*)user_data;
    Mount * eMount = new Mount;
    g_object_ref(mount);
    WO::setGMount(eMount, mount);
    p->emitMountAdded(p, eMount);
}

static void _mount_changed_handler(GVolumeMonitor *volume_monitor,
        GMount         *mount,
        gpointer        user_data)
{
    VolumeMonitor * p = (VolumeMonitor*)user_data;
    Mount * eMount = new Mount;
    g_object_ref(mount);
    WO::setGMount(eMount, mount);
    p->emitMountChanged(p, eMount);
}

static void _mount_pre_unmount_handler(GVolumeMonitor *volume_monitor,
        GMount         *mount,
        gpointer        user_data)
{
    VolumeMonitor * p = (VolumeMonitor*)user_data;
    Mount * eMount = new Mount;
    g_object_ref(mount);
    WO::setGMount(eMount, mount);
    p->emitPreUnmount(p, eMount);
}

static void _mount_removed_handler(GVolumeMonitor *volume_monitor,
        GMount         *mount,
        gpointer        user_data)
{
    VolumeMonitor * p = (VolumeMonitor*)user_data;
    Mount * eMount = new Mount;
    g_object_ref(mount);
    WO::setGMount(eMount, mount);
    p->emitMountRemoved(p, eMount);
}

static void _volume_added_handler(GVolumeMonitor *volume_monitor,
        GVolume        *volume,
        gpointer        user_data)
{
    VolumeMonitor * p = (VolumeMonitor*)user_data;
    Volume * eVolume = new Volume;
    g_object_ref(volume);
    WO::setGVolume(eVolume, volume);
    p->emitVolumeAdded(p, eVolume);
}

static void _volume_changed_handler(GVolumeMonitor *volume_monitor,
        GVolume        *volume,
        gpointer        user_data)
{
    VolumeMonitor * p = (VolumeMonitor*)user_data;
    Volume * eVolume = new Volume;
    g_object_ref(volume);
    WO::setGVolume(eVolume, volume);
    p->emitVolumeChanged(p, eVolume);
}

static void _volume_removed_handler(GVolumeMonitor *volume_monitor,
        GVolume        *volume,
        gpointer        user_data)
{
    VolumeMonitor * p = (VolumeMonitor*)user_data;
    Volume * eVolume = new Volume;
    g_object_ref(volume);
    WO::setGVolume(eVolume, volume);
    p->emitVolumeRemoved(p, eVolume);
}

//////

class VolumeMonitor::Private
{
public:
    Private() 
    {
    };

    void connectSignalHandlers(VolumeMonitor * p)
    {
        GVolumeMonitor * gVolumeMonitor = WO::getGVolumeMonitor(p);
        drive_changed_handler_id       = g_signal_connect (gVolumeMonitor, "drive-changed", (GCallback)_drive_changed_handler, p);
        drive_connected_handler_id     = g_signal_connect (gVolumeMonitor, "drive-connected", (GCallback)_drive_connected_handler, p);
        drive_disconnected_handler_id  = g_signal_connect (gVolumeMonitor, "drive-disconnected", (GCallback)_drive_disconnected_handler, p);
        mount_added_handler_id         = g_signal_connect (gVolumeMonitor, "mount-added", (GCallback)_mount_added_handler, p);
        mount_changed_handler_id       = g_signal_connect (gVolumeMonitor, "mount-changed", (GCallback)_mount_changed_handler, p);
        mount_pre_unmount_handler_id   = g_signal_connect (gVolumeMonitor, "mount-pre-unmount", (GCallback)_mount_pre_unmount_handler, p);
        mount_removed_handler_id       = g_signal_connect (gVolumeMonitor, "mount-removed", (GCallback)_mount_removed_handler, p);
        volume_added_handler_id        = g_signal_connect (gVolumeMonitor, "volume-added", (GCallback)_volume_added_handler, p);
        volume_changed_handler_id      = g_signal_connect (gVolumeMonitor, "volume-changed", (GCallback)_volume_changed_handler, p);
        volume_removed_handler_id      = g_signal_connect (gVolumeMonitor, "volume-removed", (GCallback)_volume_removed_handler, p);
    }

    void disconnectSignalHandlers(VolumeMonitor * p)
    {
        GVolumeMonitor * gVolumeMonitor = WO::getGVolumeMonitor(p);
        g_signal_handler_disconnect(gVolumeMonitor, drive_changed_handler_id);
        g_signal_handler_disconnect(gVolumeMonitor, drive_connected_handler_id);     
        g_signal_handler_disconnect(gVolumeMonitor, drive_disconnected_handler_id);  
        g_signal_handler_disconnect(gVolumeMonitor, mount_added_handler_id); 
        g_signal_handler_disconnect(gVolumeMonitor, mount_changed_handler_id);       
        g_signal_handler_disconnect(gVolumeMonitor, mount_pre_unmount_handler_id);   
        g_signal_handler_disconnect(gVolumeMonitor, mount_removed_handler_id);       
        g_signal_handler_disconnect(gVolumeMonitor, volume_added_handler_id);        
        g_signal_handler_disconnect(gVolumeMonitor, volume_changed_handler_id);      
        g_signal_handler_disconnect(gVolumeMonitor, volume_removed_handler_id);      
    }
    
    
    int drive_changed_handler_id;
    int drive_connected_handler_id;
    int drive_disconnected_handler_id;
    int mount_added_handler_id;
    int mount_changed_handler_id;
    int mount_pre_unmount_handler_id;
    int mount_removed_handler_id;
    int volume_added_handler_id;
    int volume_changed_handler_id;
    int volume_removed_handler_id;
    
    
};

/////

VolumeMonitor::VolumeMonitor() : d(new Private)
{
    WO::initGIO();
    
    GVolumeMonitor * gVolumeMonitor = g_volume_monitor_get();

    WO::setGVolumeMonitor(this, gVolumeMonitor);
    d->connectSignalHandlers(this);
}


VolumeMonitor::~VolumeMonitor()
{
    d->disconnectSignalHandlers(this);
    delete d;
    
}

QList<Drive*> VolumeMonitor::getConnectedDrives() const
{
    QList<Drive*> result;
    
    GList* drives = g_volume_monitor_get_connected_drives(WO::getGVolumeMonitor(this));
    GList* next = drives;
    
    while (next)
    {
        Drive * drive = new Drive;
        WO::setGDrive(drive, (GDrive*) next->data);
        result.append(drive);
        next = next->next;
    }
    g_list_free(drives);
    return result;    
}

QList<Volume*> VolumeMonitor::getVolumes() const
{
    
    QList<Volume*> result;
    
    GList* volumes = g_volume_monitor_get_volumes(WO::getGVolumeMonitor(this));
    GList* next = volumes;
    
    while (next)
    {
        Volume * volume = new Volume;
        WO::setGVolume(volume, (GVolume*) next->data);
        result.append(volume);
        next = next->next;
    }
    g_list_free(volumes);
    return result;    
}

QList<Mount*> VolumeMonitor::getMounts() const
{
    QList<Mount*> result;
    
    GList* mounts = g_volume_monitor_get_mounts(WO::getGVolumeMonitor(this));
    GList* next = mounts;
    
    while (next)
    {
        Mount * mount = new Mount;
        WO::setGMount(mount, (GMount*) next->data);
        result.append(mount);
        next = next->next;
    }
    g_list_free(mounts);
    return result;    
}

Volume * VolumeMonitor::adoptOrphanMount(const Mount * mount)
{
    GVolume * gVolume = g_volume_monitor_adopt_orphan_mount(WO::getGMount(mount));
    if (!gVolume) return 0;
    
    Volume * volume = new Volume;
    WO::setGVolume(volume, gVolume);
    return volume;
}

Mount * VolumeMonitor::getMountForUuid (const QByteArray & uuid)
{
    GMount * gMount = g_volume_monitor_get_mount_for_uuid (WO::getGVolumeMonitor(this), uuid.data());
    if (!gMount) return 0;
    
    Mount * mount = new Mount;
    WO::setGMount(mount, gMount);
    return mount;
}

Volume * VolumeMonitor::getVolumeForUuid(const QByteArray & uuid)
{
    GVolume * gVolume = g_volume_monitor_get_volume_for_uuid(WO::getGVolumeMonitor(this), uuid.data());
    if (!gVolume) return 0;
    
    Volume * volume = new Volume;
    WO::setGVolume(volume, gVolume);
    return volume;
}

/////

void VolumeMonitor::emitDriveChanged(const VolumeMonitor * volumeMonitor, const Drive * drive)
{
    emit driveChanged(volumeMonitor, drive);
}

void VolumeMonitor::emitDriveConnected(const VolumeMonitor * volumeMonitor, const Drive * drive)
{
    emit driveConnected(volumeMonitor, drive);
}

void VolumeMonitor::emitDriveDisconnected(const VolumeMonitor * volumeMonitor, const Drive * drive)
{
    emit driveDisconnected(volumeMonitor, drive);
}

void VolumeMonitor::emitMountAdded(const VolumeMonitor * volumeMonitor, const Mount * mount)
{
    emit mountAdded(volumeMonitor, mount);
}

void VolumeMonitor::emitMountChanged(const VolumeMonitor * volumeMonitor, const Mount * mount)
{
    emit mountChanged(volumeMonitor, mount);
}

void VolumeMonitor::emitPreUnmount(const VolumeMonitor * volumeMonitor, const Mount * mount)
{
    emit preUnmount(volumeMonitor, mount);
}

void VolumeMonitor::emitMountRemoved(const VolumeMonitor * volumeMonitor, const Mount * mount)
{
    emit mountRemoved(volumeMonitor, mount);
}

void VolumeMonitor::emitVolumeAdded(const VolumeMonitor * volumeMonitor, const Volume * volume)
{
    emit volumeAdded(volumeMonitor, volume);
}

void VolumeMonitor::emitVolumeChanged(const VolumeMonitor * volumeMonitor, const Volume * volume)
{
    emit volumeChanged(volumeMonitor, volume);
}

void VolumeMonitor::emitVolumeRemoved(const VolumeMonitor * volumeMonitor, const Volume * volume)
{
    emit volumeRemoved(volumeMonitor, volume);
}

}}

#include "moc_kgio-volumemonitor.cpp"
