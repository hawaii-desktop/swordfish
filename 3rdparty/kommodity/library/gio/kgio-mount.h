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


#ifndef KGIOMOUNT_H_
#define KGIOMOUNT_H_

#include <QtCore/QObject>
#include <QtCore/QString>
#include <QtCore/QByteArray>
#include <QtCore/QSharedData>
#include <QtGui/QIcon>
#include <QtCore/QFile>

#include <Kommodity/GIO/WrappedObject>
#include <Kommodity/GIO/Error>
#include <Kommodity/GIO/Icon>
//#include <Kommodity/GIO/File>
#include <Kommodity/GIO/MountOperation>

namespace Kommodity {
namespace GIO {

class Volume;
class Drive;
class File;

class Q_DECL_EXPORT Mount : public QObject, public virtual WrappedObject
{
    Q_OBJECT

public:

    Mount();
    virtual ~Mount();

    enum MountFlags {
        MountNone = 0
    };

    enum UnmountFlags {
        UnmountNorm = 0,
        UnmountForce = (1<<0)
    };

    QString getName() const;
    QByteArray getUuid() const;
    QIcon getIcon() const;
    Drive * getDrive() const;
    File getRoot() const;
    Volume * getVolume() const;

    bool canUnmount() const;

    void unmount(UnmountFlags flags = UnmountNorm,
                const Cancellable * cancellable = 0);

    void remount(MountFlags flags,
                MountOperation & mount_operation,
                const Cancellable * cancellable = 0);

    bool canEject();

    void eject(Mount::UnmountFlags flags,
                const Cancellable * cancellable = 0);

Q_SIGNALS:
    void unmountReady(Mount * mount, const Error & error);
    void remountReady(Mount * mount, const Error & error);
    void ejectReady(Mount * mount, const Error & error);

private:

    class Private;
    friend class Private;
    Private * d;

public: // internal
    void emitUnmountReady(Mount * mount, const Error & error);
    void emitRemountReady(Mount * mount, const Error & error);
    void emitEjectReady(Mount * mount, const Error & error);
};

}}
#endif /*KGIOMOUNT_H_*/
