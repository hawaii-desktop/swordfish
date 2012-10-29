/****************************************************************************
 * This file is part of Swordfish
 *
 * Copyright (c) 2010-2012 Pier Luigi Fiorini
 *
 * Author(s):
 *    Pier Luigi Fiorini <pierluigi.fiorini@gmail.com>
 *
 * Swordfish is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Swordfish is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Swordfish.  If not, see <http://www.gnu.org/licenses/>.
 ***************************************************************************/

#include <QItemSelectionModel>

#include <VFileSystemModel>

#include "fileviewcontroller.h"

FileViewController::FileViewController(QObject *parent)
    : QObject(parent)
    , m_model(new VFileSystemModel(this))
    , m_selectionModel(new QItemSelectionModel(m_model))
{
    setModel(new VFileSystemModel(this));
    setSelectionModel(new QItemSelectionModel(model(), this));
}

void FileViewController::setModel(VFileSystemModel *model)
{
    m_model = model;
    m_model->setReadOnly(false);
}

void FileViewController::setSelectionModel(QItemSelectionModel *model)
{
    m_selectionModel = model;

    connect(model, SIGNAL(selectionChanged(QItemSelection, QItemSelection)),
            this, SLOT(selectionChanged(QItemSelection, QItemSelection)));
}

void FileViewController::selectionChanged(const QItemSelection &selected, const QItemSelection &deselected)
{
    switch (selected.length()) {
        case 0:
            emit selectionCleared();
            break;
        case 1:
            emit itemSelected(selected.indexes().at(0));
            break;
        default:
            emit itemsSelected(selected.indexes());
            break;
    }
}

#include "moc_fileviewcontroller.cpp"
