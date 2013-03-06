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
    
class Drive::Private : public QSharedData
{
public:
    Private() 
    {
    };
};

/////

static void  _poll_for_media_ready_callback(GObject *source_object, GAsyncResult *result, gpointer user_data)
{
    Error error;
    
    QPointer<Drive> * p = (QPointer<Drive> *)user_data;
    if (!p->isNull())
    {
        gboolean success = g_drive_poll_for_media_finish(
                G_DRIVE(source_object),
                result,
                WO::getGErrorPtr(&error));
        
        p->data()->emitPollForMediaReady(p->data(), error);
    }
    delete p;
}


static void  _eject_ready_callback(GObject *source_object, GAsyncResult *result, gpointer user_data)
{
    Error error;

    QPointer<Drive> * p = (QPointer<Drive> *)user_data;
    if (!p->isNull())
    {
        gboolean success = g_drive_eject_finish(
                G_DRIVE(source_object),
                result,
                WO::getGErrorPtr(&error));

        p->data()->emitEjectReady(p->data(), error);
    }
    delete p;
}

/////

Drive::Drive() : d(0)
{
    WO::initGIO();
}


Drive::~Drive()
{
}

QString Drive::getName() const
{
    return WO::qStringFromUtf8Freed(g_drive_get_name(WO::getGDrive(this)));
}

Icon * Drive::getIcon() const
{
    GIcon * gIcon = g_drive_get_icon(WO::getGDrive(this));
    if (!gIcon) return 0;
    
    Icon * icon = new Icon;
    WO::setGIcon(icon, gIcon);
    return icon;
}

bool Drive::canEject() const
{
    return g_drive_can_eject(WO::getGDrive(this));
}

bool Drive::hasVolumes() const
{
    return g_drive_has_volumes(WO::getGDrive(this));
}

QList<Volume*> Drive::getVolumes() const
{
    QList<Volume*> result;
    
    GList* volumes = g_drive_get_volumes(WO::getGDrive(this));
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


bool Drive::canPollForMedia() const
{
    return g_drive_can_poll_for_media(WO::getGDrive(this));
}

void Drive::pollForMedia(const Cancellable * cancellable)
{
    void * thisQPointer = new QPointer<Drive>(this);
    
    return g_drive_poll_for_media(WO::getGDrive(this),
            WO::getGCancellable(cancellable),
            _poll_for_media_ready_callback,
            thisQPointer);
}

bool Drive::driveHasMedia() const
{
    return g_drive_has_media(WO::getGDrive(this));
}

bool Drive::isMediaCheckAutomatic() const
{
    return g_drive_is_media_check_automatic(WO::getGDrive(this));
}

bool Drive::isMediaRemovable() const
{
    return g_drive_is_media_removable(WO::getGDrive(this));
}

QList<QByteArray> Drive::enumerateIdentifiers() const
{
    QList<QByteArray> result;
    
    char** identifiers = g_drive_enumerate_identifiers(WO::getGDrive(this));
    int cnt=0;
    while (identifiers[cnt])
    {
        result.append(QByteArray(identifiers[cnt]));
        cnt++;
    }
    return result;
}

QByteArray Drive::getIdentifier(const char *kind) const
{
    return WO::qByteArrayFromCStrFreed(g_drive_get_identifier(WO::getGDrive(this), kind));
}

void Drive::eject(Mount::UnmountFlags flags, const Cancellable * cancellable)
{
    void * thisQPointer = new QPointer<Drive>(this);
    
    g_drive_eject(WO::getGDrive(this),
            (GMountUnmountFlags) flags,
            WO::getGCancellable(cancellable),
            _eject_ready_callback,
            thisQPointer);
}

/////

void Drive::emitPollForMediaReady(Drive * drive, const Error & error)
{
    emit pollForMediaReady(drive, error);
}

void Drive::emitEjectReady(Drive * drive, const Error & error)
{
    emit ejectReady(drive, error);
}



}}

#include "moc_kgio-drive.cpp"
