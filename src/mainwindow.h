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

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QModelIndex>
#include <QMainWindow>
#include <QAbstractItemView>

#include <VSettings>

namespace Ui
{
    class MainWindow;
}

class QToolButton;
class VBreadcrumbView;
class FileManager;
class FileViewController;

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

    QAbstractItemView *currentView() const;

protected:
    void changeEvent(QEvent *e);

private:
    Ui::MainWindow *ui;
    FileManager *m_fileMan;
#if 0
    VBreadcrumbView *m_breadcrumb;
#endif
    QMenu *m_appMenu;
    QToolButton *m_appMenuButton;
    QAbstractItemView *m_currentView;
    QList<QModelIndex> m_history;
    int m_historyPoint;
    FileViewController *m_viewController;
    VSettings *m_desktopSettings;

    void setupActions();
    void setupViews();

    QModelIndex select(const QModelIndex &index) const;
    void setRootPath(const QModelIndex &index);

private slots:
    void newWindow();
    void newTab();
    void newFolder();
    void openFile();
    void openFileWith();
    void closeWindow();
    void duplicateFile();
    void createAlias();
    void search();

    void undo();
    void redo();
    void cut();
    void copy();
    void paste();
    void selectAll();
    void rename();
    void moveToTrash();
    void deletePermanently();
    void preferences();

    void viewModeSelected(QAction *action);
    void arrangeOptionSelected(QAction *action);

    void goBack();
    void goForward();
    void goParent();
    void goComputer();
    void goHome();
    void goTrash();
    void goNetwork();

    void helpContents();
    void reportProblem();
    void about();

    void clipboardChanged();

    void selectItem(const QModelIndex &index);
    void itemsSelected(const QModelIndexList &indexes);
    void selectionCleared();

    void doubleClicked(const QModelIndex &index);
    void customContextMenuRequested(const QPoint &pos);

    void fileCopied(const QUrl &from, const QUrl &to);
    void fileMoved(const QUrl &from, const QUrl &to);
};

#endif // MAINWINDOW_H
