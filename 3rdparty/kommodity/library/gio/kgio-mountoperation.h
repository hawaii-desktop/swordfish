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


#ifndef KGIOMOUNTOPERATION_H_
#define KGIOMOUNTOPERATION_H_

#include <QtCore/QObject>
#include <QtCore/QString>
#include <QtCore/QUrl>
#include <QtCore/QSharedData>

#include <Kommodity/GIO/WrappedObject>
#include <Kommodity/GIO/Error>

namespace Kommodity {
namespace GIO {
    
class Q_DECL_EXPORT MountOperation : public QObject, public virtual WrappedObject
{
    Q_OBJECT

public:
    
    MountOperation();
    virtual ~MountOperation();
    
    enum PasswordSave 
    {
      PasswordSaveNever,
      PasswordSaveForSession,
      PasswordSavePermanently
    };

    enum MountOperationResult {
      MountOperationHandeled,
      MountOperationAborted,
      MountOperationUnhandeled
    };
    
    enum AskPasswordFlags {
      AskPasswordNeedPassword       = (1 << 0),
      AskPasswordNeedUsername       = (1 << 1),
      AskPasswordNeedDomain         = (1 << 2),
      AskPasswordSavingSupported    = (1 << 3),
      AskPasswordAnonymousSupported = (1 << 4)
    };
    
    QString getUsername() const;
    void setUsername(const QString &username);

    QString getPassword() const;
    void setPassword(const QString &password);

    bool getAnonymous() const;
    void setAnonymous(bool anonymous);
    
    QString getDomain() const;
    void setDomain(const QString &domain);
    
    PasswordSave getPasswordSave();
    void setPasswordSave(PasswordSave save);
    
    int getChoice();
    void setChoice(int choice);
    
    void reply(MountOperationResult result);

Q_SIGNALS:    
    void askPassword(MountOperation *op,
                     const QString & message,
                     const QString & defaultUser,
                     const QString & defaultDomain,
                     MountOperation::AskPasswordFlags flags);

    void askQuestion(MountOperation *op,
                     const QString & message,
                     const QList<QString> & choices);
    
    void replied(MountOperation *op, MountOperation::MountOperationResult abort);    
    
public: // internal
    void emitAskPassword(MountOperation *op,
                     const QString & message,
                     const QString & defaultUser,
                     const QString & defaultDomain,
                     AskPasswordFlags flags);

    void emitAskQuestion(MountOperation *op,
                     const QString & message,
                     const QList<QString> & choices);
    
    void emitReplied(MountOperation *op, MountOperationResult abort);        
    
private:
    class Private;
    friend class Private;
    Private * d;
};

}}

#endif /*KGIOMOUNTOPERATION_H_*/
