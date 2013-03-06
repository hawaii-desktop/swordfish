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

#include <QtCore/QString>
#include <QtCore/QPointer>

#include <glib.h>
#include <gio/gio.h>

#include "kgio-file.h"
#include "kgio-fileinfo.h"
#include "kgio-wrappedobjectutils_p.h"

namespace Kommodity {
namespace GIO {

/////


static  void _file_changed_handler(GFileMonitor     *monitor,
                                   GFile            *file,
                                   GFile            *other_file,
                                   GFileMonitorEvent event_type,
                                   gpointer          user_data)

{
    FileMonitor * p = (FileMonitor *)user_data;
    File eFile;
    File eOtherFile;
    WO::setGFile(&eFile, file);
    WO::setGFile(&eOtherFile, other_file);
    p->emitChanged(p, eFile, eOtherFile, (FileMonitor::FileMonitorEvent) event_type);
}


/////

class FileMonitor::Private : public QSharedData
{
public:
    Private() : changed_handler_id(-1) 
    {
    };
    
    void connectSignalHandlers(FileMonitor * p)
    {
        GFileMonitor * gFileMonitor = WO::getGFileMonitor(p);
        if (gFileMonitor==NULL) return;
        Q_ASSERT(changed_handler_id==-1);
        changed_handler_id = g_signal_connect (gFileMonitor, "changed", (GCallback)_file_changed_handler, p);
    }

    void disconnectSignalHandlers(FileMonitor * p)
    {
        GFileMonitor * gFileMonitor = WO::getGFileMonitor(p);
        if (gFileMonitor==NULL || changed_handler_id == -1) return;
        g_signal_handler_disconnect(gFileMonitor, changed_handler_id);
    }
    
    int changed_handler_id;
    
};


/////



FileMonitor::FileMonitor() : d(new Private())
{
    WO::initGIO();
}


FileMonitor::~FileMonitor()
{
    d->disconnectSignalHandlers(this);
    delete d;
}

FileMonitor::FileMonitor( const FileMonitor& other) : WrappedObject(other), d(new Private)
{
    d->connectSignalHandlers(this);
}

FileMonitor &FileMonitor::operator=(const FileMonitor &other)
{
    WrappedObject::operator=(other);
    
    d->connectSignalHandlers(this);
    return *this;
}

bool FileMonitor::cancel()
{
    return g_file_monitor_cancel(WO::getGFileMonitor(this));
}

bool FileMonitor::isCancelled() const
{
    return g_file_monitor_is_cancelled(WO::getGFileMonitor(this));
}

void FileMonitor::setRateLimit(int limitMsecs)
{
    g_file_monitor_set_rate_limit(WO::getGFileMonitor(this), limitMsecs);
}

void FileMonitor::emitEvent(const File & child, const File & otherFile, FileMonitorEvent eventType)
{
    g_file_monitor_emit_event(WO::getGFileMonitor(this),
                              WO::getGFile(&child),
                              WO::getGFile(&otherFile),
                              (GFileMonitorEvent) eventType);

}

void FileMonitor::emitChanged(FileMonitor *monitor,
             const File & file,
             const File & otherFile,
             FileMonitorEvent eventType)
{
    emit changed(monitor,
                 file,
                 otherFile,
                 eventType);
}


}}

#include "moc_kgio-filemonitor.cpp"
