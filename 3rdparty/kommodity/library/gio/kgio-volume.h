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


#ifndef KGIOVOLUME_H_
#define KGIOVOLUME_H_

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
    
class Q_DECL_EXPORT Volume : public QObject, public virtual WrappedObject
{
    Q_OBJECT

public:
    
    Volume();
    virtual ~Volume();
    
    QString getName() const;
    QByteArray getUuid() const;
    Icon * getIcon() const;
    
    Drive * getDrive() const;
    Mount * getMount() const;
    bool canMount() const;
    bool shouldAutomount() const;
    void mount(Mount:: MountFlags flags,
               MountOperation & mount_operation,
               const Cancellable * cancellable = 0);

    bool canEject() const;
    void eject(Mount::UnmountFlags flags, const Cancellable * cancellable = 0);
    
    QList<QByteArray> enumerateIdentifiers() const;
    QByteArray getIdentifier(const char *kind) const;
    
Q_SIGNALS:
    void mountReady(Volume * volume, const Error & error);
    void ejectReady(Volume * volume, const Error & error);    
    
private:

    class Private;
    friend class Private;
    Private * d;
    
public: // internal
    void emitMountReady(Volume * volume, const Error & error);
    void emitEjectReady(Volume * volume, const Error & error);
    
};

}}
#endif /*KGIOVOLUME_H_*/
