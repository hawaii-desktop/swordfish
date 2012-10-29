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

#ifndef FILEVIEWCONTROLLER_H
#define FILEVIEWCONTROLLER_H

#include <QObject>
#include <QModelIndex>

class QItemSelectionModel;
class QItemSelection;

class VFileSystemModel;

class FileViewController : public QObject
{
    Q_OBJECT
public:
    FileViewController(QObject *parent = 0);

    void setModel(VFileSystemModel *model);
    VFileSystemModel *model() const {
        return m_model;
    }

    void setSelectionModel(QItemSelectionModel *model);
    QItemSelectionModel *selectionModel() const {
        return m_selectionModel;
    }

signals:
    void itemSelected(const QModelIndex &index);
    void itemsSelected(const QModelIndexList &indexes);
    void selectionCleared();

private:
    VFileSystemModel *m_model;
    QItemSelectionModel *m_selectionModel;

private slots:
    void selectionChanged(const QItemSelection &selected, const QItemSelection &deselected);
};

#endif // FILEVIEWCONTROLLER_H
