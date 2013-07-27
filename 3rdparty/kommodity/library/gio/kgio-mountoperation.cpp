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

#include <QtCore/QPointer>
#include <QtCore/QString>

#include <glib.h>
#include <gio/gio.h>

#include "kgio-mountoperation.h"
#include "kgio-file.h"
#include "kgio-mount.h"
#include "kgio-wrappedobjectutils_p.h"

namespace Kommodity {
namespace GIO {
    
static void _ask_password_handler(GMountOperation  *op,
                                  gchar            *message,
                                  gchar            *default_user,
                                  gchar            *default_domain,
                                  GAskPasswordFlags flags,
                                  gpointer          user_data)
{
    MountOperation * p = (MountOperation *)user_data;
    p->emitAskPassword(p,
                QString(message),
                QString(default_user),
                QString(default_domain),
                (MountOperation::AskPasswordFlags) flags);
}
    
static void _ask_question_handler(GMountOperation *op,
                                                        gchar           *message,
                                                        GStrv           *choices,
                                                        gpointer         user_data)
{
    MountOperation * p = (MountOperation *)user_data;
    QList<QString> choicesList;
        
    int cnt=0;
    while (*choices[cnt])
    {
        choicesList.append(QString(*choices[cnt]));
        cnt++;
    }
        
    p->emitAskQuestion(p,
                QString(message),
                choicesList);
}

void _reply_handler(GMountOperation      *op,
                                                        GMountOperationResult abort,
                                                        gpointer              user_data)
{
    MountOperation * p = (MountOperation *)user_data;
    p->emitReplied(p,
                (MountOperation::MountOperationResult) abort);
}


class MountOperation::Private
{
public:
    Private() 
    {
    };
    
    void connectSignalHandlers(MountOperation * p)
    {
        GMountOperation * gMountOperation = WO::getGMountOperation(p);
        Q_ASSERT(gMountOperation!=NULL);
        ask_password_handler_id = g_signal_connect (gMountOperation, "ask-password", (GCallback)_ask_password_handler, p);
        ask_question_handler_id = g_signal_connect (gMountOperation, "ask-question", (GCallback)_ask_question_handler, p);
        reply_handler_id        = g_signal_connect (gMountOperation, "reply", (GCallback)_reply_handler, p);
    }

    void disconnectSignalHandlers(MountOperation * p)
    {
        GMountOperation * gMountOperation = WO::getGMountOperation(p);
        Q_ASSERT(gMountOperation!=NULL);
        g_signal_handler_disconnect(gMountOperation, ask_password_handler_id);
        g_signal_handler_disconnect(gMountOperation, ask_question_handler_id);
        g_signal_handler_disconnect(gMountOperation, reply_handler_id);
    }
    
    int ask_password_handler_id;
    int ask_question_handler_id;
    int reply_handler_id;
    
    
};


MountOperation::MountOperation() : d(new Private)
{
    GMountOperation* mountOperation = g_mount_operation_new ();
    
    WO::setGMountOperation(this, mountOperation);
    d->connectSignalHandlers(this);
    
}


MountOperation::~MountOperation()
{
    d->disconnectSignalHandlers(this);
    delete d;
}

//MountOperation::MountOperation( const MountOperation& other) : d(new Private), WrappedObject(other)
//{
//    WO::initGIO();
//    d->connectSignalHandlers(this);
//}

QString MountOperation::getUsername() const
{
    return QString(g_mount_operation_get_username(WO::getGMountOperation(this))); 
}

void MountOperation::setUsername(const QString &username)
{
    g_mount_operation_set_username(WO::getGMountOperation(this), username.toUtf8().data());
}

QString MountOperation::getPassword() const
{
    return QString(g_mount_operation_get_password(WO::getGMountOperation(this)));   
}

void MountOperation::setPassword(const QString &password)
{
    g_mount_operation_set_password(WO::getGMountOperation(this),
                                                             password.toUtf8().data());
}

bool MountOperation::getAnonymous() const
{
    return g_mount_operation_get_anonymous(WO::getGMountOperation(this));
}

void MountOperation::setAnonymous(bool anonymous)
{
    g_mount_operation_set_anonymous(WO::getGMountOperation(this), anonymous);
}

QString MountOperation::getDomain() const
{
    return QString(g_mount_operation_get_domain(WO::getGMountOperation(this))); 
}

void MountOperation::setDomain(const QString &domain)
{
    g_mount_operation_set_domain(WO::getGMountOperation(this), domain.toUtf8().data());
}


MountOperation::PasswordSave MountOperation::getPasswordSave()
{
    return (MountOperation::PasswordSave)g_mount_operation_get_password_save (WO::getGMountOperation(this));
}

void MountOperation::setPasswordSave(MountOperation::PasswordSave save)
{
    g_mount_operation_set_password_save (WO::getGMountOperation(this),
                                                             (GPasswordSave) save);
}


int MountOperation::getChoice()
{
    return g_mount_operation_get_choice(WO::getGMountOperation(this));   
}

void MountOperation::setChoice(int choice)
{
    g_mount_operation_set_choice(WO::getGMountOperation(this), choice);
}

void MountOperation::reply(MountOperationResult result)
{
    g_mount_operation_reply(WO::getGMountOperation(this),(GMountOperationResult) result); 
}

void MountOperation::emitAskPassword(MountOperation *op,
                 const QString & message,
                 const QString & defaultUser,
                 const QString & defaultDomain,
                 MountOperation::AskPasswordFlags flags)
{
    emit askPassword(op,
            message,
            defaultUser,
            defaultDomain,
            flags);
}

void MountOperation::emitAskQuestion(MountOperation *op,
                 const QString & message,
                 const QList<QString> & choices)
{
    emit askQuestion(op,
                     message,
                     choices);
}

void MountOperation::emitReplied(MountOperation *op, MountOperationResult abort)
{
    emit replied(op, abort);
}


}}

#include "moc_kgio-mountoperation.cpp"
