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

#include "kgio-appinfo.h"
#include "kgio-wrappedobjectutils_p.h"

namespace Kommodity {
namespace GIO {
    
class AppInfo::Private : public QSharedData
{
public:
    Private() 
    {
    };
};


AppInfo::AppInfo() : d(0)
{
    WO::initGIO();
}


AppInfo::~AppInfo()
{
}

AppInfo::AppInfo( const AppInfo& other) : SharedWrappedObject(other), d(0)
{
}

AppInfo &AppInfo::operator=(const AppInfo &other)
{
    SharedWrappedObject::operator=(other);
    d = other.d;
    return *this;
}

AppInfo AppInfo::createFromCommandline(const char *commandline, 
                                     const char *application_name,
                                     CreateFlags flags,
                                     Error & error)
{
    AppInfo appInfo;
    WO::setGAppInfo(&appInfo, g_app_info_create_from_commandline(commandline,
                                                             application_name,
                                                             (GAppInfoCreateFlags) flags,
                                                             WO::getGErrorPtr(&error)));
    return appInfo;
}

AppInfo AppInfo::dup()
{
    AppInfo appInfo;
    WO::setGAppInfo(&appInfo, g_app_info_dup(WO::getGAppInfo(this)));
    return appInfo;
}

bool AppInfo::equal(const AppInfo & appinfo)
{
    return g_app_info_equal(WO::getGAppInfo(this),WO::getGAppInfo(&appinfo));
}
                                                 
QByteArray AppInfo::getId() const
{
    return QByteArray(g_app_info_get_id(WO::getGAppInfo(this)));
}

QString AppInfo::getName() const
{
    return QString(g_app_info_get_name(WO::getGAppInfo(this)));
}

QString AppInfo::getDescription() const
{
    return QString(g_app_info_get_description(WO::getGAppInfo(this)));
}

QByteArray AppInfo::getExecutable() const
{
    return QByteArray(g_app_info_get_executable(WO::getGAppInfo(this)));
}

Icon * AppInfo::getIcon() const
{
    GIcon * gIcon = g_app_info_get_icon(WO::getGAppInfo(this));
    if (!gIcon) return 0;
    
    Icon * icon = new Icon;
    WO::setGIcon(icon, gIcon);
    return icon;
}

bool AppInfo::launch(const QList<File> & files, AppLaunchContext & launch_context, Error & error)
{
    GList * gFilesList = WO::gFileListFromFileList(files);
   
    bool success = g_app_info_launch(WO::getGAppInfo(this),
            gFilesList,
            WO::getGAppLaunchContext(&launch_context),
            WO::getGErrorPtr(&error));
    
    g_list_foreach(gFilesList, (GFunc) g_object_unref, NULL);
    g_list_free(gFilesList);
    return success;
}

bool AppInfo::supportsFiles() const
{
    return g_app_info_supports_files(WO::getGAppInfo(this));
}

bool AppInfo::supportsUris() const
{
    return g_app_info_supports_uris(WO::getGAppInfo(this));
}

bool AppInfo::launchUris(const QList<QString> & uris, AppLaunchContext & launch_context, Error & error)
{
    GList * gUrisList = WO::glistFromQStringList(uris);
    bool success = g_app_info_launch_uris(
            WO::getGAppInfo(this),
            gUrisList,
            WO::getGAppLaunchContext(&launch_context),
            WO::getGErrorPtr(&error));
    g_list_foreach(gUrisList, (GFunc) g_free, NULL);
    g_list_free(gUrisList);
    return success;
}

bool AppInfo::shouldShow()
{
    return g_app_info_should_show(WO::getGAppInfo(this));
}

bool AppInfo::setAsDefaultForType(const QString & contentType, Error & error)
{
    return g_app_info_set_as_default_for_type(
            WO::getGAppInfo(this),
            contentType.toUtf8().data(),
            WO::getGErrorPtr(&error));
}

bool AppInfo::setAsDefaultForExtension(const QString & extension, Error & error)
{
    return g_app_info_set_as_default_for_extension(
            WO::getGAppInfo(this),
            extension.toUtf8().data(),
            WO::getGErrorPtr(&error));
}

bool AppInfo::addSupportsType(const QString & contentType, Error & error)
{
    return g_app_info_add_supports_type(
            WO::getGAppInfo(this),
            contentType.toUtf8().data(),
            WO::getGErrorPtr(&error));
}

bool AppInfo::canRemoveSupportsType()
{
    return g_app_info_can_remove_supports_type(WO::getGAppInfo(this));
}

bool AppInfo::removeSupportsType(const QString & contentType, Error & error)
{
    return g_app_info_remove_supports_type     (WO::getGAppInfo(this),
            contentType.toUtf8().data(),
            WO::getGErrorPtr(&error));
}

static QList<AppInfo> _appinfo_qlist_from_glist(GList * gList)
{
    QList<AppInfo> qList;
    
    while (gList)
    {
        AppInfo appInfo;
        WO::setGAppInfo(&appInfo, (GAppInfo*)gList->data);
        qList.append(appInfo);
        gList = gList->next;
    }
    return qList;
}



QList<AppInfo> AppInfo::getAll()
{
    GList* gAppInfos = g_app_info_get_all();
    QList<AppInfo> result = _appinfo_qlist_from_glist(gAppInfos);

    g_list_foreach(gAppInfos, (GFunc) g_object_unref, NULL);
    g_list_free(gAppInfos);
    return result;
}

QList<AppInfo> AppInfo::getAllForType(const QString & contentType)
{
    GList* gAppInfos =  g_app_info_get_all_for_type(contentType.toUtf8().data());
    QList<AppInfo> result = _appinfo_qlist_from_glist(gAppInfos);

    g_list_foreach(gAppInfos, (GFunc) g_object_unref, NULL);
    g_list_free(gAppInfos);
    return result;
}

AppInfo AppInfo::getDefaultForType(const QString & contentType, bool mustSupportUris)
{
    AppInfo appInfo;
    WO::setGAppInfo(&appInfo, g_app_info_get_default_for_type(contentType.toUtf8().data(),
            mustSupportUris));
    return appInfo;
}

AppInfo AppInfo::getDefaultForUriScheme(const QString & uriScheme)
{
    AppInfo appInfo;
    WO::setGAppInfo(&appInfo, g_app_info_get_default_for_uri_scheme(uriScheme.toUtf8().data()));
    return appInfo;
}

bool AppInfo::launchDefaultForUri(const QString & uri, AppLaunchContext & launchContext, Error & error)
{
    return g_app_info_launch_default_for_uri(uri.toUtf8().data(),
            WO::getGAppLaunchContext(&launchContext),
            WO::getGErrorPtr(&error));
}


}}

#include "kgio-appinfo.moc"
