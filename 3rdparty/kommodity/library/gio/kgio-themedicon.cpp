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

#include "kgio-themedicon.h"
#include "kgio-wrappedobjectutils_p.h"

#include <stdlib.h>

namespace Kommodity {
namespace GIO {
    
class ThemedIcon::Private : public QSharedData
{
public:
    Private() 
    {
    };
};


ThemedIcon::ThemedIcon() : d(0)
{
}


ThemedIcon::~ThemedIcon()
{
}

ThemedIcon::ThemedIcon(const QString & name, bool defaultFallbacks) : d(0)
{
    if (!defaultFallbacks)
    {
        WO::setGIcon(this, g_themed_icon_new(name.toUtf8().data()));
    }
    else
    {
        WO::setGIcon(this, g_themed_icon_new_with_default_fallbacks
                                                            (name.toUtf8().data()));
    }
}

ThemedIcon::ThemedIcon(const QList<QString> & names)
{
    char ** iconnames = (char **)malloc(sizeof(char*) * names.size());
    
    for (int i = 0; i < names.size(); ++i) {
         QString qstr = names.at(i);
         iconnames[i] = g_strdup(qstr.toUtf8().data());
    }
    WO::setGIcon(this, g_themed_icon_new_from_names        (iconnames,
            names.size()));

    for (int i = 0; i < names.size(); ++i)
    {
        g_free(iconnames[i]);
    }
    g_free(iconnames);
}

void ThemedIcon::appendName(const QString & iconName)
{
    g_themed_icon_append_name((GThemedIcon *)WO::getGIcon(this),iconName.toUtf8().data());

}

QList<QString> ThemedIcon::getNames() const
{
    //gchar ** names;
    //g_object_get (WO::getGIcon(this), "names", &names, NULL);
    
    const char * const * names = g_themed_icon_get_names(G_THEMED_ICON(WO::getGIcon(this)));
    
    QList<QString> result;
    
    if (!names) return result;
    
    int cnt=0;
    while (names[cnt])
    {
        result.append(QString(names[cnt]));
        cnt++;
    }
    
    //g_strfreev (names);
    return result;
}

}}

#include "moc_kgio-themedicon.cpp"
