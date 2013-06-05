/****************************************************************************
 * This file is part of Swordfish.
 *
 * Copyright (C) 2010-2013 Pier Luigi Fiorini <pierluigi.fiorini@gmail.com>
 *
 * Author(s):
 *    Pier Luigi Fiorini
 *
 * $BEGIN_LICENSE:LGPL2.1+$
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 2.1 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * $END_LICENSE$
 ***************************************************************************/

#include <QQmlEngine>
#include <QQmlComponent>

#include "swordfishplugin.h"
#include "placesmodel.h"
#include "foldermodel.h"

void SwordfishPlugin::registerTypes(const char *uri)
{
    Q_ASSERT(uri == QStringLiteral("Swordfish"));

    // Register QML types
    qmlRegisterType<PlacesModel>("Swordfish", 0, 1, "PlacesModel");
    qmlRegisterType<FolderModel>("Swordfish", 0, 1, "FolderModel");
}

void SwordfishPlugin::initializeEngine(QQmlEngine *engine, const char *uri)
{
    Q_ASSERT(uri == QStringLiteral("Swordfish"));
}

#include "moc_swordfishplugin.cpp"
