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

#include "kgio-loadableicon.h"
#include "kgio-wrappedobjectutils_p.h"

namespace Kommodity {
namespace GIO {
    
class LoadableIcon::Private : public QSharedData
{
public:
    Private() 
    {
    };
};

////////

static void  _icon_load_async_ready_callback(GObject *source_object, GAsyncResult *result, gpointer user_data)
{
    Error error;
    char * type;
    
    GInputStream* gInputStream = g_loadable_icon_load_finish(
            G_LOADABLE_ICON(source_object),
            result,
            &type,
            WO::getGErrorPtr(&error));
    
    InputStream * inputStream;
    if (!error.hasError())
    {
        inputStream = new InputStream;
        WO::setGInputStream(inputStream, gInputStream);
    }
    
    QPointer<LoadableIcon> * p = (QPointer<LoadableIcon> *)user_data;
    if (!p->isNull())
    {
        p->data()->emitLoadAsyncReady(
            p->data(),
            inputStream,
            QString(type),
            error);
    }
    delete p;
}

////////


LoadableIcon::LoadableIcon() : d(0)
{
    WO::initGIO();
}


LoadableIcon::~LoadableIcon()
{
}


InputStream * LoadableIcon::load(
        int size, 
        char **type, 
        Error & error, 
        const Cancellable * cancellable)
{
    InputStream * inputStream = new InputStream;
    WO::setGInputStream(inputStream, g_loadable_icon_load(WO::getGLoadableIcon(this),
                                                           size,
                                                           type,
                                                           WO::getGCancellable(cancellable),
                                                           WO::getGErrorPtr(&error)));
    return inputStream;
}

void LoadableIcon::loadAsync(int size,
        const Cancellable * cancellable)
{
    void * thisQPointer = new QPointer<LoadableIcon>(this);
    
    g_loadable_icon_load_async(WO::getGLoadableIcon(this),
                               size,
                               WO::getGCancellable(cancellable),
                               _icon_load_async_ready_callback,
                               thisQPointer);
}

void LoadableIcon::emitLoadAsyncReady(LoadableIcon * icon, InputStream * inputStream, const QString & type, const Error & error)
{
    emit loadAsyncReady(icon, inputStream, type, error);
}


}}

#include "kgio-loadableicon.moc"
