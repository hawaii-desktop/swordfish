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

#include <glib.h>
#include <gio/gio.h>

#include "kgio-drive.h"
#include "kgio-volume.h"
#include "kgio-file.h"
#include "kgio-wrappedobjectutils_p.h"

namespace Kommodity {
namespace GIO {



void WO::setGMount(Mount * mount, GMount * gMount)
{
    mount->setObjectPtr(gMount);
}
    
GMount * WO::getGMount(const Mount * mount)
{
    return (GMount*)mount->getObjectPtr();
}    

/////

void WO::setGDrive(Drive * drive, GDrive * gDrive)
{
    drive->setObjectPtr(gDrive);
}
    
GDrive * WO::getGDrive(const Drive * drive)
{
    return (GDrive*)drive->getObjectPtr();
}        

/////

void WO::setGVolume(Volume * volume, GVolume * gVolume)
{
    volume->setObjectPtr(gVolume);
}
    
GVolume * WO::getGVolume(const Volume * volume)
{
    return (GVolume*)volume->getObjectPtr();
}        

/////

void WO::setGVolumeMonitor(VolumeMonitor * volumeMonitor, GVolumeMonitor * gVolumeMonitor)
{
    volumeMonitor->setObjectPtr(gVolumeMonitor);
}
    
GVolumeMonitor * WO::getGVolumeMonitor(const VolumeMonitor * volumeMonitor)
{
    return (GVolumeMonitor*)volumeMonitor->getObjectPtr();
}      



}}
