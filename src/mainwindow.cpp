/****************************************************************************
 * This file is part of Swordfish.
 *
 * Copyright (C) 2010-2013 Pier Luigi Fiorini <pierluigi.fiorini@gmail.com>
 *
 * Author(s):
 *    Pier Luigi Fiorini
 *
 * $BEGIN_LICENSE:GPL2+$
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * $END_LICENSE$
 ***************************************************************************/

#include <QCoreApplication>
#include <QDebug>
#include <QActionGroup>
#include <QClipboard>
#include <QDesktopServices>
#include <QFileInfo>
#include <QLibraryInfo>
#include <QMessageBox>
#include <QMimeData>
#include <QScrollBar>
#include <QStandardPaths>
#include <QToolButton>
#include <QTranslator>
#include <QUrl>

#include <VFileSystemModel>
#include <VBreadcrumbView>
#include <VAboutDialog>

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "fileviewcontroller.h"
#include "filemanager.h"
#include "infodialog.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow())
    , m_translator(0)
    , m_historyPoint(0)
    , m_viewController(new FileViewController(this))
{
    ui->setupUi(this);

    // Load translations
    loadTranslations();

    // Interface settings
    m_desktopSettings = new VSettings("org.hawaii.desktop");

    // Create the file manager interface
    m_fileMan = new FileManager(this);
    connect(m_fileMan, SIGNAL(copyFinished(QUrl, QUrl)),
            this, SLOT(fileCopied(QUrl, QUrl)));
    connect(m_fileMan, SIGNAL(moveFinished(QUrl, QUrl)),
            this, SLOT(fileMoved(QUrl, QUrl)));

    // Hide the menu bar by default because we have the app menu button
    menuBar()->hide();

    // Setup actions
    setupActions();

#if 0
    // TODO: Enable again when we have a decent implementation of the widget
    // Create breadcrumb
    m_breadcrumb = new VBreadcrumbView(this);
    ui->toolBar->addWidget(m_breadcrumb);
#endif

    // Create the menu
    m_appMenu = new QMenu(this);
    m_appMenu->addAction(ui->actionNewTab);
    m_appMenu->addAction(ui->actionNewWindow);
    m_appMenu->addSeparator();
    m_appMenu->addAction(ui->actionConnectToServer);
    m_appMenu->addSeparator();
    m_appMenu->addAction(ui->actionPreferences);
    m_appMenu->addSeparator();
    m_appMenu->addAction(ui->actionAbout);

    // Menu button
    m_appMenuButton = new QToolButton(this);
    m_appMenuButton->setIcon(QIcon::fromTheme("document-properties"));
    m_appMenuButton->setPopupMode(QToolButton::InstantPopup);
    m_appMenuButton->setMenu(m_appMenu);
    ui->toolBar->addWidget(m_appMenuButton);

    // Set root path in the model
    const QString &rootPath = QDir::homePath();
    m_viewController->model()->setRootPath("/");
    m_history.append(m_viewController->model()->index(rootPath));

    // Setup file views
    setupViews();

#if 0
    // Set bread crumb model
    m_breadcrumb->setModel(m_viewController->model());
#endif

    // Set root path
    setRootPath(m_viewController->model()->index(rootPath));

    // Signals
    connect(QApplication::clipboard(), SIGNAL(dataChanged()),
            this, SLOT(clipboardChanged()));
    connect(QApplication::clipboard(), SIGNAL(changed(QClipboard::Mode)),
            this, SLOT(clipboardChanged()));
    connect(m_viewController, SIGNAL(itemSelected(QModelIndex)),
            this, SLOT(selectItem(QModelIndex)));
    connect(m_viewController, SIGNAL(itemsSelected(QModelIndexList)),
            this, SLOT(itemsSelected(QModelIndexList)));
    connect(m_viewController, SIGNAL(selectionCleared()),
            this, SLOT(selectionCleared()));
}

MainWindow::~MainWindow()
{
    delete ui;
    delete m_desktopSettings;
}

QAbstractItemView *MainWindow::currentView() const
{
    return m_currentView;
}

void MainWindow::changeEvent(QEvent *event)
{
    QMainWindow::changeEvent(event);

    switch (event->type()) {
        case QEvent::LanguageChange:
            loadTranslations();
            ui->retranslateUi(this);
            break;
        case QEvent::LocaleChange:
            loadTranslations();
            break;
        default:
            break;
    }
}

void MainWindow::loadTranslations()
{
    // Locale name
    const QString locale = QLocale::system().name();

    // Qt translations
    QTranslator qtTranslator;
    qtTranslator.load("qt_" + locale,
                      QLibraryInfo::location(QLibraryInfo::TranslationsPath));
    QCoreApplication::instance()->installTranslator(&qtTranslator);

    // Remove translation of the previously loaded locale
    if (m_translator) {
        QCoreApplication::instance()->removeTranslator(m_translator);
        delete m_translator;
    }

    // Load our translations for the current locale
    m_translator = new QTranslator(this);
    QString localeDir = QStandardPaths::locate(
                            QStandardPaths::GenericDataLocation,
                            QLatin1String("swordfish/translations"),
                            QStandardPaths::LocateDirectory);
    m_translator->load(locale, localeDir);
    QCoreApplication::instance()->installTranslator(m_translator);
}

void MainWindow::setupActions()
{
    // Connect actions
    connect(ui->actionNewWindow, SIGNAL(triggered()),
            this, SLOT(newWindow()));
    connect(ui->actionNewTab, SIGNAL(triggered()),
            this, SLOT(newTab()));
    connect(ui->actionNewFolder, SIGNAL(triggered()),
            this, SLOT(newFolder()));
    connect(ui->actionOpen, SIGNAL(triggered()),
            this, SLOT(openFile()));
    connect(ui->actionOpenWithOtherApplication, SIGNAL(triggered()),
            this, SLOT(openFileWith()));
    connect(ui->actionDuplicate, SIGNAL(triggered()),
            this, SLOT(duplicateFile()));
    connect(ui->actionCreateAlias, SIGNAL(triggered()),
            this, SLOT(createAlias()));
    connect(ui->actionSearch, SIGNAL(triggered()),
            this, SLOT(search()));
    connect(ui->actionUndo, SIGNAL(triggered()),
            this, SLOT(undo()));
    connect(ui->actionRedo, SIGNAL(triggered()),
            this, SLOT(redo()));
    connect(ui->actionCut, SIGNAL(triggered()),
            this, SLOT(cut()));
    connect(ui->actionCopy, SIGNAL(triggered()),
            this, SLOT(copy()));
    connect(ui->actionPaste, SIGNAL(triggered()),
            this, SLOT(paste()));
    connect(ui->actionSelectAll, SIGNAL(triggered()),
            this, SLOT(selectAll()));
    connect(ui->actionRename, SIGNAL(triggered()),
            this, SLOT(rename()));
    connect(ui->actionGetInformation, SIGNAL(triggered()),
            this, SLOT(getInformation()));
    connect(ui->actionMoveToTrash, SIGNAL(triggered()),
            this, SLOT(moveToTrash()));
    connect(ui->actionDeletePermanently, SIGNAL(triggered()),
            this, SLOT(deletePermanently()));
    connect(ui->actionPreferences, SIGNAL(triggered()),
            this, SLOT(preferences()));
    connect(ui->actionBack, SIGNAL(triggered()),
            this, SLOT(goBack()));
    connect(ui->actionForward, SIGNAL(triggered()),
            this, SLOT(goForward()));
    connect(ui->actionComputer, SIGNAL(triggered()),
            this, SLOT(goComputer()));
    connect(ui->actionHome, SIGNAL(triggered()),
            this, SLOT(goHome()));
    connect(ui->actionTrash, SIGNAL(triggered()),
            this, SLOT(goNetwork()));
    connect(ui->actionContents, SIGNAL(triggered()),
            this, SLOT(helpContents()));
    connect(ui->actionReportProblem, SIGNAL(triggered()),
            this, SLOT(reportProblem()));
    connect(ui->actionAbout, SIGNAL(triggered()),
            this, SLOT(about()));

    // Setup view mode actions
    QActionGroup *viewMode = new QActionGroup(this);
    viewMode->setExclusive(true);
    viewMode->addAction(ui->actionAsIcons);
    viewMode->addAction(ui->actionAsDetails);
    viewMode->addAction(ui->actionAsColumns);
    connect(viewMode, SIGNAL(triggered(QAction *)),
            this, SLOT(viewModeSelected(QAction *)));

    // Setup arrange options
    QActionGroup *arrangeOptions = new QActionGroup(this);
    arrangeOptions->setExclusive(true);
    arrangeOptions->addAction(ui->actionArrangeByName);
    arrangeOptions->addAction(ui->actionArrangeBySize);
    arrangeOptions->addAction(ui->actionArrangeByType);
    arrangeOptions->addAction(ui->actionArrangeByModificationDate);
    connect(arrangeOptions, SIGNAL(triggered(QAction *)),
            this, SLOT(arrangeOptionSelected(QAction *)));
}

void MainWindow::setupViews()
{
    int iconSize = m_desktopSettings->value("interface/iconview-icon-size").toInt();

    // Icon view
    ui->iconView->setModel(m_viewController->model());
    ui->iconView->setSelectionModel(m_viewController->selectionModel());
    ui->iconView->setIconSize(QSize(iconSize, iconSize));
    ui->iconView->setGridSize(QSize(iconSize * 2, iconSize * 2));
    m_currentView = ui->iconView;
    connect(ui->iconView, SIGNAL(customContextMenuRequested(QPoint)),
            this, SLOT(customContextMenuRequested(QPoint)));
    connect(ui->iconView, SIGNAL(doubleClicked(QModelIndex)),
            this, SLOT(doubleClicked(QModelIndex)));

    // Tree view
    ui->treeView->setModel(m_viewController->model());
    ui->treeView->setSelectionModel(m_viewController->selectionModel());
    QHeaderView *treeHeader = ui->treeView->header();
    QFontMetrics fm = QApplication::fontMetrics();
    treeHeader->resizeSection(0, fm.width(QLatin1String("aaaaaaaaaaaaaaaaaaaaaaaaaa")));
    treeHeader->resizeSection(1, fm.width(QLatin1String("4242.42 GB")));
    treeHeader->resizeSection(2, fm.width(QLatin1String("Folder")));
    treeHeader->resizeSection(3, fm.width(QLatin1String("05/27/1982 10:42 PM")));
    treeHeader->setContextMenuPolicy(Qt::ActionsContextMenu);
    connect(ui->treeView, SIGNAL(customContextMenuRequested(QPoint)),
            this, SLOT(customContextMenuRequested(QPoint)));
    connect(ui->treeView, SIGNAL(doubleClicked(QModelIndex)),
            this, SLOT(doubleClicked(QModelIndex)));

    // Column view
    ui->columnView->setModel(m_viewController->model());
    ui->columnView->setSelectionModel(m_viewController->selectionModel());
    connect(ui->columnView, SIGNAL(customContextMenuRequested(QPoint)),
            this, SLOT(customContextMenuRequested(QPoint)));
    connect(ui->columnView, SIGNAL(doubleClicked(QModelIndex)),
            this, SLOT(doubleClicked(QModelIndex)));
}

QModelIndex MainWindow::select(const QModelIndex &index) const
{
    Q_ASSERT(index.isValid() ? index.model() == m_viewController->model() : true);

    if (index.isValid() && !m_currentView->selectionModel()->isSelected(index))
        m_currentView->selectionModel()->select(
            index, QItemSelectionModel::Select | QItemSelectionModel::Rows);
    return index;
}

void MainWindow::setRootPath(const QModelIndex &index)
{
    // Set window title according to the new root path
    setWindowTitle(m_viewController->model()->filePath(index));

    // Set the new root for all the views
    ui->iconView->setRootIndex(index);
    ui->treeView->setRootIndex(index);
    ui->columnView->setRootIndex(index);
#if 0
    m_breadcrumb->enterTree(index);
#endif

    // Enable actions
    ui->actionNewFolder->setEnabled(m_currentView->model()->flags(index) & Qt::ItemIsDropEnabled);

    // Unselect
    m_currentView->selectionModel()->clear();
}

void MainWindow::newWindow()
{
    MainWindow *window = new MainWindow();
    window->show();
    window->move(geometry().topLeft() + QPoint(20, 20));
    window->resize(size());
}

void MainWindow::newTab()
{
}

void MainWindow::newFolder()
{
    m_currentView->clearSelection();

    QString newFolderString = tr("New Folder");
    QString folderName = newFolderString;
    QString prefix = m_viewController->model()->rootDirectory().absolutePath() + QDir::separator();
    if (QFile::exists(prefix + folderName)) {
        qlonglong suffix = 2;
        while (QFile::exists(prefix + folderName))
            folderName = newFolderString + QString::number(suffix++);
    }

    QModelIndex parent = m_currentView->rootIndex();
    QModelIndex index = m_viewController->model()->mkdir(parent, folderName);
    if (!index.isValid())
        return;

    selectItem(index);
    index = select(index);
    if (index.isValid()) {
        m_currentView->setCurrentIndex(index);
        m_currentView->edit(index);
    }
}

void MainWindow::openFile()
{
    QModelIndexList selection = m_viewController->selectionModel()->selection().indexes();
    foreach(QModelIndex index, selection)
    doubleClicked(index);
}

void MainWindow::openFileWith()
{
}

void MainWindow::closeWindow()
{
    this->close();
}

void MainWindow::duplicateFile()
{
}

void MainWindow::createAlias()
{
}

void MainWindow::search()
{
}

void MainWindow::undo()
{
}

void MainWindow::redo()
{
}

void MainWindow::cut()
{
    QModelIndexList list = m_viewController->selectionModel()->selectedIndexes();
    QMimeData *data = m_viewController->model()->mimeData(list);
    data->setData("application/x-oss-cutaction", QByteArray("1"));
    QApplication::clipboard()->setMimeData(data);
}

void MainWindow::copy()
{
    QModelIndexList list = m_viewController->selectionModel()->selectedIndexes();
    QMimeData *data = m_viewController->model()->mimeData(list);
    QApplication::clipboard()->setMimeData(data);
}

void MainWindow::paste()
{
    QByteArray cutData = QApplication::clipboard()->mimeData()->data("application/x-oss-cutaction");

    QList<QUrl> urls = QApplication::clipboard()->mimeData()->urls();
    foreach(QUrl url, urls) {
        QString dstPath = m_viewController->model()->filePath(m_currentView->rootIndex());
        if (cutData == QByteArray("1"))
            m_fileMan->move(url, QUrl::fromLocalFile(dstPath));
        else
            m_fileMan->copy(url, QUrl::fromLocalFile(dstPath));
    }
}

void MainWindow::selectAll()
{
    m_currentView->selectAll();
}

void MainWindow::rename()
{
    QModelIndexList list = m_viewController->selectionModel()->selection().indexes();
    if (list.length() > 0) {
        QModelIndex index = list.at(0);
        if (index.isValid())
            m_currentView->edit(index);
    }
}

void MainWindow::getInformation()
{
    InfoDialog dialog(this);
    dialog.setModel(m_viewController->model());
    dialog.setModelIndexList(m_viewController->selectionModel()->selection().indexes());
    dialog.move(frameGeometry().center());
    dialog.exec();
}

void MainWindow::moveToTrash()
{
    QModelIndexList selection = m_viewController->selectionModel()->selection().indexes();
    foreach(QModelIndex index, selection) {
        // TODO: Move to the trash instead of delete it
        // TODO: Check the error
        m_viewController->model()->remove(index);
    }
}

void MainWindow::deletePermanently()
{
    // Ask first
    QMessageBox dialog;
    dialog.setIcon(QMessageBox::Question);
    dialog.setText(tr("<b>Do you really want to delete the selected items permanently?</b>"));
    dialog.setInformativeText(tr("If you delete an item, it will be gone for ever."));
    dialog.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    dialog.setDefaultButton(QMessageBox::Yes);
    if (dialog.exec() == QMessageBox::Yes) {
        // Then permanently delete
        QModelIndexList selection = m_viewController->selectionModel()->selection().indexes();
        foreach(QModelIndex index, selection) {
            // TODO: Check the error
            m_viewController->model()->remove(index);
        }
    }
}

void MainWindow::preferences()
{
}

void MainWindow::viewModeSelected(QAction *action)
{
    if (action->objectName() == "actionAsIcons") {
        ui->stackedWidget->setCurrentIndex(0);
        m_currentView = ui->iconView;
    } else if (action->objectName() == "actionAsDetails") {
        ui->stackedWidget->setCurrentIndex(1);
        m_currentView = ui->treeView;
    } else if (action->objectName() == "actionAsColumns") {
        ui->stackedWidget->setCurrentIndex(2);
        m_currentView = ui->columnView;
    }

    m_currentView->doItemsLayout();
}

void MainWindow::arrangeOptionSelected(QAction *action)
{
}

void MainWindow::goBack()
{
    if (!m_history.isEmpty() && m_historyPoint > 0) {
        --m_historyPoint;

        QModelIndex prevIndex = m_history.at(m_historyPoint);
        setRootPath(prevIndex);

        ui->actionBack->setEnabled(m_historyPoint > 0);
        ui->actionForward->setEnabled(m_history.size() - m_historyPoint > 1);
    }
}

void MainWindow::goForward()
{
    if (!m_history.isEmpty() && m_historyPoint < m_history.size() - 1) {
        ++m_historyPoint;

        QModelIndex nextLocation = m_history.at(m_historyPoint);
        setRootPath(nextLocation);

        ui->actionBack->setEnabled(m_historyPoint > 0);
        ui->actionForward->setEnabled(m_history.size() - m_historyPoint > 1);
    }
}

void MainWindow::goParent()
{
    VFileSystemModel *model = m_viewController->model();

    QDir rootDir(model->rootDirectory());

    // Find parent path
    if (rootDir.isRoot()) {
        // Root directory's parent is "My Computer"
        setRootPath(model->index(model->myComputer().toString()));
    } else {
        // Up one level
        rootDir.cdUp();
        setRootPath(model->index(rootDir.absolutePath()));
    }
}

void MainWindow::goComputer()
{
    VFileSystemModel *model = m_viewController->model();
    setRootPath(model->index(model->myComputer().toString()));
}

void MainWindow::goHome()
{
    VFileSystemModel *model = m_viewController->model();
    setRootPath(model->index(QDir::homePath()));
}

void MainWindow::goTrash()
{
    // TODO: How do we implement this?
}

void MainWindow::goNetwork()
{
    // TODO: How do we implement this?
}

void MainWindow::helpContents()
{
}

void MainWindow::reportProblem()
{
    QDesktopServices::openUrl(QUrl("https://github.com/hawaii-desktop/swordfish/issues"));
}

void MainWindow::about()
{
    QStringList authors;
    authors << "Pier Luigi Fiorini <pierluigi.fiorini@gmail.com>";

    VAboutDialog dialog(this);
    dialog.setAuthors(authors);
    dialog.setCopyright("Pier Luigi Fiorini");
    dialog.setDescription("File manager with a focus on speed and ease of use.");
    dialog.setLink(QUrl("http://www.maui-project.org/"));

    QFile licenseFile(":/COPYING");
    if (licenseFile.open(QIODevice::ReadOnly)) {
        dialog.setLicenseText(licenseFile.readAll());
        licenseFile.close();
    }

    dialog.exec();
}

void MainWindow::clipboardChanged()
{
    // Enable paste action if something was copied to the clipboard
    const QMimeData *data = QApplication::clipboard()->mimeData();
    qDebug() << data->hasUrls() << data->urls() << data->data("text/uri-list");
    ui->actionPaste->setEnabled(data->hasUrls());
}

void MainWindow::selectItem(const QModelIndex &index)
{
    // Set index as the current item
    m_currentView->selectionModel()->select(index, QItemSelectionModel::SelectCurrent);
    m_currentView->scrollTo(index);

    // Since we got an item selected we can enable some actions
    ui->actionCut->setEnabled(true);
    ui->actionCopy->setEnabled(true);
    ui->actionPaste->setEnabled(true);
    ui->actionRename->setEnabled(true);
    ui->actionMoveToTrash->setEnabled(true);
    ui->actionDeletePermanently->setEnabled(true);
}

void MainWindow::itemsSelected(const QModelIndexList &indexes)
{
}

void MainWindow::selectionCleared()
{
    // Since we got not selection we disable some actions
    ui->actionCut->setEnabled(false);
    ui->actionCopy->setEnabled(false);
    ui->actionPaste->setEnabled(false);
    ui->actionRename->setEnabled(false);
    ui->actionMoveToTrash->setEnabled(false);
    ui->actionDeletePermanently->setEnabled(false);
}

void MainWindow::doubleClicked(const QModelIndex &index)
{
    QFileInfo fileInfo(m_viewController->model()->fileInfo(index));

    if (fileInfo.isDir()) {
        if (m_historyPoint < 0 || m_history.value(m_historyPoint) != index) {
            while (m_historyPoint >= 0 && m_historyPoint + 1 < m_history.count())
                m_history.removeLast();
            m_history.append(index);
            ++m_historyPoint;
        }

        ui->actionBack->setEnabled(m_historyPoint > 0);
        ui->actionForward->setEnabled(m_history.size() - m_historyPoint > 1);

        setRootPath(index);
    } else
        QDesktopServices::openUrl(QUrl::fromLocalFile(m_viewController->model()->filePath(index)));
}

void MainWindow::customContextMenuRequested(const QPoint &pos)
{
    // Find the item that is located at the current position, if it's not valid
    // then the context menu was requested for the root path
    QModelIndex index = m_currentView->indexAt(pos);

    // Context menu
    QMenu menu(this);

    if (index.isValid()) {
        // File context menu
        menu.addAction(ui->actionCut);
        menu.addAction(ui->actionCopy);
        menu.addSeparator();
        menu.addAction(ui->actionRename);
        menu.addSeparator();
        menu.addAction(ui->actionMoveToTrash);
        menu.addAction(ui->actionDeletePermanently);
        menu.addSeparator();
        menu.addAction(ui->actionGetInformation);
    } else {
        // Current folder context menu
        menu.addAction(ui->actionNewFolder);
        menu.addSeparator();
        QMenu *arrangeMenu = menu.addMenu(tr("Arran&ge Items"));
        arrangeMenu->addAction(ui->actionArrangeByName);
        arrangeMenu->addAction(ui->actionArrangeBySize);
        arrangeMenu->addAction(ui->actionArrangeByType);
        arrangeMenu->addAction(ui->actionArrangeByModificationDate);
        arrangeMenu->addSeparator();
        arrangeMenu->addAction(ui->actionArrangeReversedOrder);
        menu.addSeparator();
        menu.addAction(ui->actionPaste);
        menu.addSeparator();
        menu.addAction(ui->actionGetInformation);
    }

    // Show the context menu
    menu.exec(m_currentView->viewport()->mapToGlobal(pos));
}

void MainWindow::fileCopied(const QUrl &from, const QUrl &to)
{
    Q_UNUSED(from);
    VFileSystemModel *model = m_viewController->model();
    QModelIndex index = model->index(to.toLocalFile());
    m_viewController->selectionModel()->select(index, QItemSelectionModel::SelectCurrent);
}

void MainWindow::fileMoved(const QUrl &from, const QUrl &to)
{
    fileCopied(from, to);
}

#include "moc_mainwindow.cpp"
