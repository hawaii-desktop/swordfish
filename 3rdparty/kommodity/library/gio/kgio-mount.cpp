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

#include "kgio-mount.h"
#include "kgio-drive.h"
#include "kgio-volume.h"
#include "kgio-wrappedobjectutils_p.h"

namespace Kommodity {
namespace GIO {
    
class Mount::Private : public QSharedData
{
public:
    Private() 
    {
    };
};

static void  _unmount_ready_callback(GObject *source_object, GAsyncResult *result, gpointer user_data)
{
    Error error;
    
    QPointer<Mount> * p = (QPointer<Mount> *)user_data;
    if (!p->isNull())
    {
        
        gboolean success = g_mount_unmount_finish(
                G_MOUNT(source_object),
                result,
                WO::getGErrorPtr(&error));
        
        p->data()->emitUnmountReady(p->data(), error);
    }
    delete p;
}

static void  _remount_ready_callback(GObject *source_object, GAsyncResult *result, gpointer user_data)
{
    Error error;

    QPointer<Mount> * p = (QPointer<Mount> *)user_data;
    if (!p->isNull())
    {
        gboolean success = g_mount_remount_finish(
                G_MOUNT(source_object),
                result,
                WO::getGErrorPtr(&error));

        p->data()->emitRemountReady(p->data(), error);
    }
    delete p;
}

static void  _eject_ready_callback(GObject *source_object, GAsyncResult *result, gpointer user_data)
{
    Error error;

    QPointer<Mount> * p = (QPointer<Mount> *)user_data;
    if (!p->isNull())
    {
        gboolean success = g_mount_eject_finish(
                G_MOUNT(source_object),
                result,
                WO::getGErrorPtr(&error));

        p->data()->emitEjectReady(p->data(), error);
    }
    delete p;
}


Mount::Mount() : d(0)
{
    WO::initGIO();
}


Mount::~Mount()
{
}

QString Mount::getName() const
{
    return WO::qStringFromUtf8Freed(g_mount_get_name(WO::getGMount(this)));
}

QByteArray Mount::getUuid() const
{
    return WO::qByteArrayFromCStrFreed(g_mount_get_uuid(WO::getGMount(this)));
}

Icon * Mount::getIcon() const
{
    GIcon * gIcon = g_mount_get_icon(WO::getGMount(this));
    if (!gIcon) return 0;
    
    Icon * icon = new Icon;
    WO::set<Icon,GIcon>(icon, gIcon);
    //WO::setGIcon(&icon, g_mount_get_icon(WO::getGMount(this)));
    return icon;
}

Drive * Mount::getDrive() const
{
    GDrive * gDrive = g_mount_get_drive(WO::getGMount(this)); 
    if (!gDrive) return 0;
    
    Drive * drive = new Drive;
    WO::setGDrive(drive, gDrive);
    return drive;
}

File Mount::getRoot() const
{
    File file;
    WO::setGFile(&file, g_mount_get_root(WO::getGMount(this)));
    return file;
}

Volume * Mount::getVolume() const
{
    GVolume * gVolume = g_mount_get_volume(WO::getGMount(this));
    if (!gVolume) return 0;
    
    Volume * volume = new Volume;
    WO::setGVolume(volume, gVolume);
    return volume;
}

bool Mount::canUnmount() const
{
    return g_mount_can_unmount(WO::getGMount(this));
}

void Mount::unmount(UnmountFlags flags, const Cancellable * cancellable)
{
    void * thisQPointer = new QPointer<Mount>(this);
    
    g_mount_unmount(WO::getGMount(this),
            (GMountUnmountFlags) flags,
            WO::getGCancellable(cancellable),
            _unmount_ready_callback,
            thisQPointer);
}

void Mount::remount(MountFlags flags,
             MountOperation & mount_operation,
             const Cancellable * cancellable)
{
    void * thisQPointer = new QPointer<Mount>(this);
    
    g_mount_remount(WO::getGMount(this),
            (GMountMountFlags) flags,
            WO::getGMountOperation(&mount_operation),
            WO::getGCancellable(cancellable),
            _remount_ready_callback,
            thisQPointer);
}

bool Mount::canEject()
{
    return g_mount_can_eject(WO::getGMount(this));
}

void Mount::eject(Mount::UnmountFlags flags, const Cancellable * cancellable)
{
    void * thisQPointer = new QPointer<Mount>(this);
    
    g_mount_eject(WO::getGMount(this),
            (GMountUnmountFlags) flags,
            WO::getGCancellable(cancellable),
            _eject_ready_callback,
            thisQPointer);
}

///

void Mount::emitUnmountReady(Mount * mount, const Error & error)
{
    emit unmountReady(mount, error);
}

void Mount::emitRemountReady(Mount * mount, const Error & error)
{
    emit remountReady(mount, error);
}

void Mount::emitEjectReady(Mount * mount, const Error & error)
{
    emit ejectReady(mount, error);
}



}}

#include "moc_kgio-mount.cpp"
