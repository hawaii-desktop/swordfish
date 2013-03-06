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


#ifndef KGIOFILEMONITOR_H_
#define KGIOFILEMONITOR_H_

#include <QtCore/QObject>
#include <QtCore/QString>
#include <QtCore/QUrl>
#include <QtCore/QSharedData>

#include <Kommodity/GIO/WrappedObject>
#include <Kommodity/GIO/Error>
#include <Kommodity/GIO/FileInputStream>
#include <Kommodity/GIO/FileOutputStream>
#include <Kommodity/GIO/AppInfo>
#include <Kommodity/GIO/Cancellable>
#include <Kommodity/GIO/FileInfo>

namespace Kommodity {
namespace GIO {

class File;
    
class Q_DECL_EXPORT FileMonitor : public QObject, public virtual WrappedObject
{
    Q_OBJECT

public:
    
    FileMonitor(const FileMonitor&);
    FileMonitor();
    virtual ~FileMonitor();
    
    FileMonitor &operator=(const FileMonitor &other);
    
    enum FileMonitorEvent {
        Changed,
        ChangesDoneHint,
        Deleted,
        Created,
        AttributeChanged,
        PreUnmount,
        Unmounted
    };
    
    bool cancel();
    bool isCancelled() const;
    void setRateLimit(int limitMsecs);
    void emitEvent(const File & child, const File & otherFile, FileMonitorEvent eventType);

Q_SIGNALS:
    void changed(FileMonitor *monitor,
                 const File & file,
                 const File & otherFile,
                 FileMonitorEvent eventType);
    
private:

    class Private;
    friend class Private;
    Private * d;
    
public: // internal
    void emitChanged(FileMonitor *monitor,
                 const File & file,
                 const File & otherFile,
                 FileMonitorEvent eventType);

    
};

}}
#endif /*KGIOFILEMONITOR_H_*/
