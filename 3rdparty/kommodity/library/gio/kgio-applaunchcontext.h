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


#ifndef KGIOAPPLAUNCHCONTEXT_H_
#define KGIOAPPLAUNCHCONTEXT_H_

#include <Kommodity/GIO/Error>
#include <Kommodity/GIO/WrappedObject>

namespace Kommodity {
namespace GIO {

class AppInfo;
class WO;
class File;

class Q_DECL_EXPORT AppLaunchContext
{

public:
   
    AppLaunchContext(const AppLaunchContext&);
    AppLaunchContext();
    virtual ~AppLaunchContext();
    AppLaunchContext &operator=(const AppLaunchContext &other);

    QByteArray getDisplay(const AppInfo & info, const QList<File> & files);
    QByteArray getStartupNotifyId(const AppInfo & info, const QList<File> & files);
    void launchFailed(const QByteArray & startup_notify_id);
    
private:
    
    void * getObjectPtr() const;
    void setObjectPtr(void * objectPtr);
    
    class Private;
    friend class Private;
    QExplicitlySharedDataPointer<Private> d;
    friend class WO;
};

}}
#endif /*KGIOAPPLAUNCHCONTEXT_H_*/
