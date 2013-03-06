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

#include "kgio-applaunchcontext.h"
#include "kgio-wrappedobjectutils_p.h"

namespace Kommodity {
namespace GIO {
    
   
class AppLaunchContext::Private : public QSharedData
{
public:
    Private() : m_gAppLaunchContext(0)
    {
    };
    
    ~Private()
    {
//        if (m_gAppLaunchContext)
//            g_free(m_gAppLaunchContext);
    };
    GAppLaunchContext* m_gAppLaunchContext;
    
};


AppLaunchContext::AppLaunchContext() : d(new Private)
{
    WO::initGIO();
    d->m_gAppLaunchContext = g_app_launch_context_new();
}


AppLaunchContext::~AppLaunchContext()
{
}

AppLaunchContext::AppLaunchContext( const AppLaunchContext& other) : d(other.d)
{
    WO::initGIO();
}

AppLaunchContext &AppLaunchContext::operator=(const AppLaunchContext &other)
{
    d = other.d;
    return *this;
}

void * AppLaunchContext::getObjectPtr() const
{
    return d->m_gAppLaunchContext;
}

void AppLaunchContext::setObjectPtr(void * objectPtr)
{
    d->m_gAppLaunchContext = (GAppLaunchContext*)objectPtr;
}

QByteArray AppLaunchContext::getDisplay(const AppInfo & info, const QList<File> & files)
{
    GList * gFilesList = WO::gFileListFromFileList(files);
    
    QByteArray result = QByteArray(g_app_launch_context_get_display(d->m_gAppLaunchContext,
                                                             WO::getGAppInfo(&info),
                                                             gFilesList));
    g_list_foreach(gFilesList, (GFunc) g_object_unref, NULL);
    g_list_free(gFilesList);
    return result;
}

QByteArray AppLaunchContext::getStartupNotifyId(const AppInfo & info, const QList<File> & files)
{
    GList * gFilesList = WO::gFileListFromFileList(files);
    
    QByteArray result = QByteArray(g_app_launch_context_get_startup_notify_id(
            d->m_gAppLaunchContext,
            WO::getGAppInfo(&info),
            gFilesList));
    g_list_foreach(gFilesList, (GFunc) g_object_unref, NULL);
    g_list_free(gFilesList);
    return result;
    
}

void AppLaunchContext::launchFailed(const QByteArray & startupNotifyId)
{
    g_app_launch_context_launch_failed(d->m_gAppLaunchContext, startupNotifyId.data());
}


}}

#include "kgio-applaunchcontext.moc"

