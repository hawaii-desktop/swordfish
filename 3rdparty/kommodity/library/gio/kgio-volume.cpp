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

#include <QtCore/QFile>
#include <QtCore/QString>
#include <QtCore/QPointer>

#include <glib.h>
#include <gio/gio.h>

#include "kgio-mount.h"
#include "kgio-drive.h"
#include "kgio-volume.h"
#include "kgio-file.h"
#include "kgio-wrappedobjectutils_p.h"

namespace Kommodity {
namespace GIO {
    
class Volume::Private : public QSharedData
{
public:
    Private() 
    {
    };
};

/////

static void  _mount_ready_callback(GObject *source_object, GAsyncResult *result, gpointer user_data)
{
    Error error;
    
    QPointer<Volume> * p = (QPointer<Volume> *)user_data;
    if (!p->isNull())
    {
        
        g_volume_mount_finish(
                    G_VOLUME(source_object),
                    result,
                    WO::getGErrorPtr(&error));
        
        p->data()->emitMountReady(p->data(), error);
    }
    delete p;
}


static void  _eject_ready_callback(GObject *source_object, GAsyncResult *result, gpointer user_data)
{
    Error error;

    QPointer<Volume> * p = (QPointer<Volume> *)user_data;
    if (!p->isNull())
    {
        g_volume_eject_with_operation_finish(
                    G_VOLUME(source_object),
                    result,
                    WO::getGErrorPtr(&error));

        p->data()->emitEjectReady(p->data(), error);
    }
    delete p;
}

/////

Volume::Volume() : d(0)
{
}


Volume::~Volume()
{
}

QString Volume::getName() const
{
    return WO::qStringFromUtf8Freed(g_volume_get_name(WO::getGVolume(this)));
}

QByteArray Volume::getUuid() const
{
    return WO::qByteArrayFromCStrFreed(g_volume_get_identifier(WO::getGVolume(this),"uuid"));
}

Icon * Volume::getIcon() const
{
    GIcon *gIcon = g_volume_get_icon(WO::getGVolume(this));
    if (!gIcon) return 0;
 
    Icon * icon = new Icon;
    WO::setGIcon(icon, gIcon);
    return icon;
}

Drive * Volume::getDrive() const
{
    GDrive * gDrive = g_volume_get_drive(WO::getGVolume(this));
    if (!gDrive) return 0;
    
    Drive * drive = new Drive;
    WO::setGDrive(drive, gDrive);
    return drive;
}

Mount * Volume::getMount() const
{
    GMount * gMount = g_volume_get_mount(WO::getGVolume(this));
    if (!gMount) return 0;
    
    Mount * mount = new Mount;
    WO::setGMount(mount, gMount);
    return mount;
    
}

bool Volume::canMount() const
{
    return g_volume_can_mount(WO::getGVolume(this));
}

bool Volume::shouldAutomount() const
{
    return g_volume_should_automount(WO::getGVolume(this));
}

void Volume::mount(Mount:: MountFlags flags,
           MountOperation & mountOperation,
           const Cancellable * cancellable)
{
    void * thisQPointer = new QPointer<Volume>(this);

    g_volume_mount(WO::getGVolume(this),
            (GMountMountFlags) flags,
            WO::getGMountOperation(&mountOperation),
            WO::getGCancellable(cancellable),
            _mount_ready_callback,
            thisQPointer);
}

bool Volume::canEject() const
{
    return g_volume_can_eject(WO::getGVolume(this));
}

void Volume::eject(Mount::UnmountFlags flags, const Cancellable * cancellable)
{
    void * thisQPointer = new QPointer<Volume>(this);
    
    g_volume_eject_with_operation(WO::getGVolume(this),
                                  (GMountUnmountFlags) flags,
                                  NULL,
                                  WO::getGCancellable(cancellable),
                                  _eject_ready_callback,
                                  thisQPointer);
}

QList<QByteArray> Volume::enumerateIdentifiers() const
{
    QList<QByteArray> result;
    
    char** identifiers = g_volume_enumerate_identifiers(WO::getGVolume(this));
    int cnt=0;
    while (identifiers[cnt])
    {
        result.append(QByteArray(identifiers[cnt]));
        cnt++;
    }
    return result;
}

QByteArray Volume::getIdentifier(const char *kind) const
{
    return QByteArray(g_volume_get_identifier(WO::getGVolume(this), kind));
}

///

void Volume::emitMountReady(Volume * volume, const Error & error)
{
    emit mountReady(volume, error);
}

void Volume::emitEjectReady(Volume * volume, const Error & error)
{
    emit ejectReady(volume, error);
}

}}

#include "moc_kgio-volume.cpp"
