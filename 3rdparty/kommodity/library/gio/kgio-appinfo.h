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


#ifndef KGIOAPPINFO_H_
#define KGIOAPPINFO_H_

#include <Kommodity/GIO/Error>
#include <Kommodity/GIO/SharedWrappedObject>
#include <Kommodity/GIO/Icon>
#include <Kommodity/GIO/AppLaunchContext>

namespace Kommodity {
namespace GIO {

class File;
    
class Q_DECL_EXPORT AppInfo : public virtual SharedWrappedObject
{
public:
   
    AppInfo(const AppInfo&);
    AppInfo();
    virtual ~AppInfo();
    AppInfo &operator=(const AppInfo &other);
    
    enum CreateFlags {
      CreateNone = 0,           
      CreateNeedsTerminal = (1<<0),  
      CreateSupportsUris = (1<<1)   
    };

    static AppInfo createFromCommandline(const char *commandline, 
                                         const char *application_name,
                                         CreateFlags flags,
                                         Error & error);
    AppInfo dup();
    
    bool equal(const AppInfo & appinfo);
                                                     
    QByteArray getId() const;
    QString getName() const;
    QString getDescription() const;
    QByteArray getExecutable() const;
    Icon * getIcon() const;
    bool launch(const QList<File> & files, AppLaunchContext & launch_context, Error & error);

    bool supportsFiles() const;
    bool supportsUris() const;
    bool launchUris(const QList<QString> & uris, AppLaunchContext & launch_context, Error & error);

    bool shouldShow();
    bool setAsDefaultForType(const QString & contentType, Error & error);

    bool setAsDefaultForExtension(const QString & extension, Error & error);
    bool addSupportsType(const QString & content_type, Error & error);

    bool canRemoveSupportsType();
    bool removeSupportsType(const QString & contentType, Error & error);

    static QList<AppInfo> getAll();
    static QList<AppInfo> getAllForType(const QString & contentType);
    static AppInfo getDefaultForType(const QString & contentType, bool mustSupportUris);
    static AppInfo getDefaultForUriScheme(const QString & uriScheme);
    static bool launchDefaultForUri(const QString & uri, AppLaunchContext & launch_context, Error & error);
    QString getDisplay(AppLaunchContext & launch_context, const QList<QString> & files);

    QByteArray getStartupNotifyId(AppLaunchContext & launch_context, const QList<QString> & files);
    
    void launchFailed(AppLaunchContext & launch_context, const QByteArray & startupNotifyId);
    
    
private:

    class Private;
    friend class Private;
    QSharedDataPointer<Private> d;
};

}}  // namespace


#endif /*KGIOAPPINFO_H_*/
