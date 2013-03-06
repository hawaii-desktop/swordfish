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


#ifndef KGIODRIVE_H_
#define KGIODRIVE_H_

#include <QtCore/QObject>
#include <QtCore/QString>
#include <QtCore/QSharedData>

#include <Kommodity/GIO/WrappedObject>
#include <Kommodity/GIO/Error>
#include <Kommodity/GIO/Icon>
#include <Kommodity/GIO/Mount>

namespace Kommodity {
namespace GIO {
    
class Q_DECL_EXPORT Drive : public QObject, public virtual WrappedObject
{
    Q_OBJECT

public:
    
    Drive();
    virtual ~Drive();
    
    QString getName() const;
    Icon * getIcon() const;
    
    bool hasVolumes() const;
    QList<Volume*> getVolumes() const;
    bool canEject() const;
    bool canPollForMedia() const;
    void pollForMedia(const Cancellable * cancellable = 0);
    
    bool driveHasMedia() const;
    bool isMediaCheckAutomatic() const;
    bool isMediaRemovable() const;
    
    void eject(Mount::UnmountFlags flags, const Cancellable * cancellable = 0);

    QList<QByteArray> enumerateIdentifiers() const;
    QByteArray getIdentifier(const char *kind) const;
    
Q_SIGNALS:
    void pollForMediaReady(Drive * drive, const Error & error);
    void ejectReady(Drive * drive, const Error & error);    
    
private:

    class Private;
    friend class Private;
    Private * d;

public: // internal
    void emitPollForMediaReady(Drive * drive, const Error & error);
    void emitEjectReady(Drive * drive, const Error & error);    
    
};

}}
#endif /*KGIODRIVE_H_*/
