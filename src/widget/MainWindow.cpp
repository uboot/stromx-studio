/* 
*  Copyright 2011 Matthias Fuchs
*
*  This file is part of stromx-studio.
*
*  Stromx-studio is free software: you can redistribute it and/or modify
*  it under the terms of the GNU General Public License as published by
*  the Free Software Foundation, either version 3 of the License, or
*  (at your option) any later version.
*
*  Stromx-studio is distributed in the hope that it will be useful,
*  but WITHOUT ANY WARRANTY; without even the implied warranty of
*  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
*  GNU General Public License for more details.
*
*  You should have received a copy of the GNU General Public License
*  along with stromx-studio.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <QAction>
#include <QApplication>
#include <QCloseEvent>
#include <QDockWidget>
#include <QLabel>
#include <QMenu>
#include <QMenuBar>
#include <QMessageBox>
#include <QSplitter>
#include <QToolBar>
#include <QDir>
#include <QFileDialog>
#include <QHBoxLayout>
#include <QSettings>
#include <QtDebug>
#include <QtGlobal>
#include <iostream>
#include <memory>
#include <stromx/runtime/DirectoryFileInput.h>
#include <stromx/runtime/DirectoryFileOutput.h>
#include <stromx/runtime/Exception.h>
#include <stromx/runtime/ZipFileInput.h>
#include <stromx/runtime/ZipFileOutput.h>
#include "Common.h"
#include "Config.h"
#include "Exception.h"
#include "LimitUndoStack.h"
#include "StreamEditorScene.h"
#include "model/ErrorListModel.h"
#include "model/ObserverTreeModel.h"
#include "model/OperatorLibraryModel.h"
#include "model/StreamModel.h"
#include "widget/ErrorListView.h"
#include "widget/FindPackagesDialog.h"
#include "widget/MainWindow.h"
#include "widget/ObserverTreeView.h"
#include "widget/ObserverView.h"
#include "widget/ObserverWindow.h"
#include "widget/OperatorLibraryView.h"
#include "widget/PropertyView.h"
#include "widget/SettingsDialog.h"
#include "widget/StreamEditor.h"
#include "widget/ThreadListView.h"
#include "widget/DataVisualizer.h"
#include "widget/DocumentationWindow.h"

MainWindow::MainWindow(QWidget *parent)
  : QMainWindow(parent),
    m_model(0),
    m_timeoutMessageIsActive(false)
{
    m_undoStack = new LimitUndoStack(this);
    m_streamEditor = new StreamEditor;
    m_threadListView = new ThreadListView(this);
    m_docWindow = new DocumentationWindow(this);
    m_settingsDialog = new SettingsDialog(this);
    m_findPackagesDialog = new FindPackagesDialog(this);
    
    QHBoxLayout* layout = new QHBoxLayout;
    layout->addWidget(m_threadListView);
    QWidget* threadListWidget = new QWidget(this);
    threadListWidget->setLayout(layout);
    
    QSplitter* splitter = new QSplitter(Qt::Vertical);
    splitter->addWidget(m_streamEditor);
    splitter->addWidget(threadListWidget);
    setCentralWidget(splitter);
    
    createDockWidgets();    
    createActions();
    createMenus();
    createToolBars();
    
    updateCurrentFile("");
    readSettings();
    
    connect(m_streamEditor->scene(), SIGNAL(selectedModelChanged(PropertyModel*)),
            m_propertyView, SLOT(setModel(PropertyModel*)));
    connect(m_streamEditor->scene(), SIGNAL(modelWasReset(StreamModel*)),
            this, SLOT(resetObserverWindows(StreamModel*)));
    connect(m_undoStack, SIGNAL(cleanChanged(bool)), this, SLOT(updateWindowTitle(bool)));
    connect(m_undoStack, SIGNAL(cleanChanged(bool)), m_saveAct, SLOT(setDisabled(bool)));
    
    StreamModel* streamModel = new StreamModel(m_undoStack,
                                               m_operatorLibraryView->operatorLibraryModel(),
                                               this);
    setModel(streamModel);
}

MainWindow::~MainWindow()
{
}

void MainWindow::createDockWidgets()
{
    m_operatorLibraryView = new OperatorLibraryView(this);
    m_operatorLibraryDockWidget = new QDockWidget(this);
    m_operatorLibraryDockWidget->setWindowTitle(tr("Operator Library"));
    m_operatorLibraryDockWidget->setObjectName("OperatorLibrary");
    m_operatorLibraryDockWidget->setWidget(m_operatorLibraryView);
    m_operatorLibraryDockWidget->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
    
    m_observerTreeView = new ObserverTreeView(this);
    m_observerDockWidget = new QDockWidget(this);
    m_observerDockWidget->setWindowTitle("Observers");
    m_observerDockWidget->setObjectName("ObserverTree");
    m_observerDockWidget->setWidget(m_observerTreeView);
    m_observerDockWidget->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
    
    m_propertyView = new PropertyView(this);
    m_propertyDockWidget = new QDockWidget(this);
    m_propertyDockWidget->setWindowTitle(tr("Properties"));
    m_propertyDockWidget->setObjectName("PropertyEditor");
    m_propertyDockWidget->setWidget(m_propertyView);
    m_propertyDockWidget->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
    
    m_errorListView = new ErrorListView(this);
    m_errorDockWidget = new QDockWidget(this);
    m_errorDockWidget->setWindowTitle(tr("Error log"));
    m_errorDockWidget->setObjectName("ErrorLog");
    m_errorDockWidget->setWidget(m_errorListView);
    m_errorDockWidget->setAllowedAreas(Qt::BottomDockWidgetArea);
    
    addDockWidget(Qt::LeftDockWidgetArea, m_operatorLibraryDockWidget);
    addDockWidget(Qt::RightDockWidgetArea, m_propertyDockWidget);
    addDockWidget(Qt::RightDockWidgetArea, m_observerDockWidget);
    addDockWidget(Qt::BottomDockWidgetArea, m_errorDockWidget);
    m_errorDockWidget->hide();
}

void MainWindow::setModel(StreamModel* model)
{
    Q_ASSERT(model);
    
    // the model is zero when this function is called for the first time
    if(m_model)
    {
        // stop the old stream
        stop();
        
        // Wait for the stream to finish. This is important because otherwise
        // the asynchronous operation which waits for the join would be still 
        // active when deleting the model below.
        m_model->join();
        
        // call join to update the stream controls
        join();
    }
    
    // clear the undo stack
    m_undoStack->clear();
    
    // clear the error list
    m_errorListView->errorListModel()->clear();
    
    // close the settings dialog
    m_settingsDialog->close();
    
    // set all editors to the new model
    m_streamEditor->streamEditorScene()->setModel(model);
    m_threadListView->setStreamModel(model);
    m_observerTreeView->setModel(model->observerModel());
    model->setExceptionObserver(m_errorListView->errorListModel()->exceptionObserver());
    m_settingsDialog->setModel(model);
    
    // delete the old model
    if(m_model)
        delete m_model;
    
    // remember the new model
    m_model = model;
    connect(m_model, SIGNAL(accessTimedOut()), this, SLOT(handleAccessTimeout()));
    connect(m_model, SIGNAL(streamJoined()), this, SLOT(join()));
    connect(m_model->observerModel(), SIGNAL(observerAdded(ObserverModel*)), this, SLOT(createObserverWindow(ObserverModel*)));
    connect(m_model->observerModel(), SIGNAL(observerRemoved(ObserverModel*)), this, SLOT(destroyObserverWindow(ObserverModel*)));
    connect(m_model, SIGNAL(delayActiveChanged(bool)), m_slowAction, SLOT(setChecked(bool)));
    
    // update the state of the slow action
    m_slowAction->setChecked(m_model->delayActive());
}

void MainWindow::createActions()
{
    m_undoAct = m_undoStack->createLimitUndoAction(this);
    m_undoAct->setShortcuts(QKeySequence::Undo);
    m_redoAct = m_undoStack->createRedoAction(this);
    m_redoAct->setShortcuts(QKeySequence::Redo);
    m_initializeAct = m_streamEditor->streamEditorScene()->createInitializeAction(this);
    m_deinitializeAct = m_streamEditor->streamEditorScene()->createDeinitializeAction(this);
    m_addThreadAct = m_threadListView->createAddThreadAction(this);
    m_removeThreadAct = m_threadListView->createRemoveThreadAction(this);
    m_removeSelectedItemsAct = m_streamEditor->streamEditorScene()->createRemoveAction(this);
    m_addObserverAct = m_observerTreeView->createAddObserverAction(this);
    m_removeObserverAct = m_observerTreeView->createRemoveObserverAction(this);
    m_removeInputAct = m_observerTreeView->createRemoveInputAction(this);
    m_resetZoomAct = m_streamEditor->createResetZoomAction(this);
    
    m_saveAct = new QAction(tr("&Save"), this);
    m_saveAct->setShortcuts(QKeySequence::Save);
    m_saveAct->setStatusTip(tr("Save the current stream"));
    m_saveAct->setEnabled(false);
    connect(m_saveAct, SIGNAL(triggered()), this, SLOT(save()));

    m_saveAsAct = new QAction(tr("Save &As..."), this);
    m_saveAsAct->setShortcuts(QKeySequence::SaveAs);
    m_saveAsAct->setStatusTip(tr("Save the current stream as"));
    connect(m_saveAsAct, SIGNAL(triggered()), this, SLOT(saveAs()));

    m_openAct = new QAction(tr("&Open..."), this);
    m_openAct->setShortcuts(QKeySequence::Open);
    m_openAct->setStatusTip(tr("Open a stream"));
    connect(m_openAct, SIGNAL(triggered()), this, SLOT(open()));

    m_closeAct = new QAction(tr("&Close"), this);
    m_closeAct->setShortcuts(QKeySequence::Close);
    m_closeAct->setStatusTip(tr("Close the current stream"));
    connect(m_closeAct, SIGNAL(triggered()), this, SLOT(closeStream()));
    
    for (int i = 0; i < MAX_RECENT_FILES; ++i)
    {
        m_recentFileActs[i] = new QAction(this);
        m_recentFileActs[i]->setVisible(false);
        connect(m_recentFileActs[i], SIGNAL(triggered()),
                this, SLOT(openRecentFile()));
    }

    m_loadPackagesAct = new QAction(tr("&Load packages..."), this);
    m_loadPackagesAct->setStatusTip(tr("Load operator packages"));
    connect(m_loadPackagesAct, SIGNAL(triggered()), this, SLOT(loadPackages()));

    m_resetPackagesAct = new QAction(tr("&Reset packages..."), this);
    m_resetPackagesAct->setStatusTip(tr("Reset operator packages"));
    connect(m_resetPackagesAct, SIGNAL(triggered()), this, SLOT(resetPackages()));

    m_findPackagesAct = new QAction(tr("&Find packages..."), this);
    m_findPackagesAct->setStatusTip(tr("Find operator packages"));
    connect(m_findPackagesAct, SIGNAL(triggered()), this, SLOT(findPackages()));

    m_quitAct = new QAction(tr("&Quit"), this);
    m_quitAct->setShortcuts(QKeySequence::Quit);
    m_quitAct->setStatusTip(tr("Quit the application"));
    connect(m_quitAct, SIGNAL(triggered()), this, SLOT(close()));

    m_startAct = new QAction(QIcon(":/images/start.png"), tr("Start"), this);
    m_startAct->setStatusTip(tr("Start the stream"));
    m_startAct->setShortcut(tr("F2"));
    connect(m_startAct, SIGNAL(triggered()), this, SLOT(start()));
    
    m_pauseAct = new QAction(QIcon(":/images/pause.png"), tr("Pause"), this);
    m_pauseAct->setStatusTip(tr("Pause the stream"));
    m_pauseAct->setShortcut(tr("F3"));
    m_pauseAct->setEnabled(false);
    connect(m_pauseAct, SIGNAL(triggered()), this, SLOT(pause()));

    m_stopAct = new QAction(QIcon(":/images/stop.png"), tr("Stop"), this);
    m_stopAct->setStatusTip(tr("Stop the stream"));
    m_stopAct->setShortcut(tr("F4"));
    m_stopAct->setEnabled(false);
    connect(m_stopAct, SIGNAL(triggered()), this, SLOT(stop()));

    m_aboutAct = new QAction(tr("&About stromx-studio"), this);
    m_aboutAct->setStatusTip(tr("Show the application's About box"));
    connect(m_aboutAct, SIGNAL(triggered()), this, SLOT(about()));

    m_aboutQtAct = new QAction(tr("About &Qt"), this);
    m_aboutQtAct->setStatusTip(tr("Show the Qt library's About box"));
    connect(m_aboutQtAct, SIGNAL(triggered()), qApp, SLOT(aboutQt()));
    
    m_slowAction = new QAction(tr("Slow processing"), this);
    m_slowAction->setStatusTip(tr("Delay each processing step by predefined time"));
    m_slowAction->setCheckable(true);
    connect(m_slowAction, SIGNAL(toggled(bool)), this, SLOT(setSlowProcessing(bool)));
    
    m_emptyRecentFilesAct = new QAction(tr("Empty recent files"), this);
    m_emptyRecentFilesAct->setStatusTip(tr("Empty the list of recently opened files"));
    connect(m_emptyRecentFilesAct, SIGNAL(triggered(bool)), this, SLOT(emptyRecentFiles()));
    
    m_showSettingsAct = new QAction(tr("Settings..."), this);
    m_showSettingsAct->setStatusTip(tr("Edit the settings of the current stream"));
    connect(m_showSettingsAct, SIGNAL(triggered(bool)), m_settingsDialog, SLOT(show()));
    
    m_showOperatorLibraryAct = new QAction(tr("Operator library"), this);
    m_showOperatorLibraryAct->setStatusTip(tr("Show operator library window"));
    m_showOperatorLibraryAct->setCheckable(true);
    m_showOperatorLibraryAct->setShortcut(tr("F5"));
    connect(m_operatorLibraryDockWidget, SIGNAL(visibilityChanged(bool)), m_showOperatorLibraryAct, SLOT(setChecked(bool)));
    connect(m_showOperatorLibraryAct, SIGNAL(toggled(bool)), m_operatorLibraryDockWidget, SLOT(setVisible(bool)));
    
    m_showPropertyViewAct = new QAction(tr("Properties"), this);
    m_showPropertyViewAct->setStatusTip(tr("Show properties window"));
    m_showPropertyViewAct->setCheckable(true);
    m_showPropertyViewAct->setShortcut(tr("F6"));
    connect(m_propertyDockWidget, SIGNAL(visibilityChanged(bool)), m_showPropertyViewAct, SLOT(setChecked(bool)));
    connect(m_showPropertyViewAct, SIGNAL(toggled(bool)), m_propertyDockWidget, SLOT(setVisible(bool)));
    
    m_showObserverTreeViewAct = new QAction(tr("Observers"), this);
    m_showObserverTreeViewAct->setStatusTip(tr("Show observers window"));
    m_showObserverTreeViewAct->setCheckable(true);
    m_showObserverTreeViewAct->setShortcut(tr("F7"));
    connect(m_observerDockWidget, SIGNAL(visibilityChanged(bool)), m_showObserverTreeViewAct, SLOT(setChecked(bool)));
    connect(m_showObserverTreeViewAct, SIGNAL(toggled(bool)), m_observerDockWidget, SLOT(setVisible(bool)));
    
    m_showErrorListViewAct = new QAction(tr("Error log"), this);
    m_showErrorListViewAct->setStatusTip(tr("Show error log window"));
    m_showErrorListViewAct->setCheckable(true);
    m_showErrorListViewAct->setShortcut(tr("F8"));
    connect(m_errorDockWidget, SIGNAL(visibilityChanged(bool)), m_showErrorListViewAct, SLOT(setChecked(bool)));
    connect(m_showErrorListViewAct, SIGNAL(toggled(bool)), m_errorDockWidget, SLOT(setVisible(bool)));
}

void MainWindow::createMenus()
{
    m_fileMenu = menuBar()->addMenu(tr("&File"));
    m_fileMenu->addAction(m_openAct);
    m_fileMenu->addAction(m_saveAct);
    m_fileMenu->addAction(m_saveAsAct);
    m_fileMenu->addAction(m_closeAct);
    m_recentFilesSeparatorAct = m_fileMenu->addSeparator();
    for (int i = 0; i < MAX_RECENT_FILES; ++i)
        m_fileMenu->addAction(m_recentFileActs[i]);
    m_fileMenu->addSeparator();
    m_fileMenu->addAction(m_emptyRecentFilesAct);
    m_fileMenu->addSeparator();
    m_fileMenu->addAction(m_loadPackagesAct);
    m_fileMenu->addAction(m_findPackagesAct);
    m_fileMenu->addAction(m_resetPackagesAct);
    m_fileMenu->addSeparator();
    m_fileMenu->addAction(m_quitAct);

    m_editMenu = menuBar()->addMenu(tr("&Edit"));
    m_editMenu->addAction(m_undoAct);
    m_editMenu->addAction(m_redoAct);

    m_streamMenu = menuBar()->addMenu(tr("&Stream"));
    m_streamMenu->addAction(m_startAct);
    m_streamMenu->addAction(m_pauseAct);
    m_streamMenu->addAction(m_stopAct);
    m_streamMenu->addSeparator();
    m_streamMenu->addAction(m_removeSelectedItemsAct);
    m_streamMenu->addAction(m_initializeAct);
    m_streamMenu->addAction(m_deinitializeAct);
    m_streamMenu->addSeparator();
    m_streamMenu->addAction(m_addThreadAct);
    m_streamMenu->addAction(m_removeThreadAct);
    m_streamMenu->addSeparator();
    m_streamMenu->addAction(m_addObserverAct);
    m_streamMenu->addAction(m_removeObserverAct);
    m_streamMenu->addAction(m_removeInputAct);
    m_streamMenu->addSeparator();
    m_streamMenu->addAction(m_showSettingsAct);

    m_viewMenu = menuBar()->addMenu(tr("&View"));
    m_viewMenu->addAction(m_resetZoomAct);
    m_viewMenu->addSeparator();
    m_viewMenu->addAction(m_showOperatorLibraryAct);
    m_viewMenu->addAction(m_showPropertyViewAct);
    m_viewMenu->addAction(m_showObserverTreeViewAct);
    m_viewMenu->addAction(m_showErrorListViewAct);
    m_viewMenuSeparatorAct = m_viewMenu->addSeparator();

    menuBar()->addSeparator();

    m_helpMenu = menuBar()->addMenu(tr("&Help"));
    m_helpMenu->addAction(m_aboutAct);
    m_helpMenu->addAction(m_aboutQtAct);
     
    updateRecentFileActions();
}

void MainWindow::createToolBars()
{    
     m_streamToolBar = addToolBar(tr("Stream"));
     m_streamToolBar->setObjectName("StreamToolbar");
     m_streamToolBar->addAction(m_startAct);
     m_streamToolBar->addAction(m_pauseAct);
     m_streamToolBar->addAction(m_stopAct);
     m_streamToolBar->addAction(m_slowAction);
     m_streamToolBar->setMovable(false);
}

void MainWindow::about()
{
    stromx::runtime::Version libVersion = stromx::runtime::version();
        
    QString description = tr(
        "This is stromx-studio <b>%1.%2.%3</b> linked against the stromx ")
        .arg(STROMX_STUDIO_VERSION_MAJOR)
        .arg(STROMX_STUDIO_VERSION_MINOR)
        .arg(STROMX_STUDIO_VERSION_PATCH) +
        tr("runtime library <b>%1.%2.%3</b>.")
        .arg(libVersion.major())
        .arg(libVersion.minor())
        .arg(libVersion.revision());
                     
    QMessageBox::about(this, tr("About stromx-studio"), description);
}

bool MainWindow::save()
{
    if(m_currentFile.isEmpty())
    {
        // return false if the saving dialog was cancelled
        if(! saveAs())
            return false;
    }
    
    return writeFile(m_currentFile);
}

void MainWindow::start()
{
    if(m_model->start())
    {
        m_startAct->setEnabled(false);
        m_pauseAct->setEnabled(true);
        m_stopAct->setEnabled(true);
        m_redoAct->setEnabled(false);
        m_undoAct->setEnabled(false);
        m_undoStack->activateLimit();
    }
}

void MainWindow::stop()
{
    if(m_model->stop())
    {
        m_startAct->setEnabled(false);
        m_pauseAct->setEnabled(false);
        m_stopAct->setEnabled(true);
    }
}

void MainWindow::pause()
{
    if(m_model->pause())
    {
        m_startAct->setEnabled(true);
        m_pauseAct->setEnabled(false);
        m_stopAct->setEnabled(true);
    }
}

void MainWindow::join()
{
    m_startAct->setEnabled(true);
    m_pauseAct->setEnabled(false);
    m_stopAct->setEnabled(false);
    m_undoStack->deactivateLimit();
    m_undoAct->setEnabled(m_undoStack->canUndo());
    m_redoAct->setEnabled(m_undoStack->canRedo());
}

bool MainWindow::openRecentFile()
{
    QAction *action = qobject_cast<QAction *>(sender());
    if (action)
    {
        if(! saveBeforeClosing())
            return false;
        
        QString filepath = action->data().toString();
        
        if(! readFile(filepath))
        {
            // if the file could not be opened remove it from the list of recently opened files
            QSettings settings("stromx", "stromx-studio");
            QStringList files = settings.value("recentFileList").toStringList();
            files.removeAll(filepath);
            settings.setValue("recentFileList", files);
            
            updateRecentFileActions();
        }
        
        return true;
    }
    
    return false;
}

bool MainWindow::open()
{
    if(! saveBeforeClosing())
        return false;
    
    QSettings settings("stromx", "stromx-studio");
    QString lastDir = settings.value("lastStreamOpened", QDir::home().absolutePath()).toString();
    
    QString file = QFileDialog::getOpenFileName(this, tr("Select a stream to open"),
                                                lastDir, tr("Stromx (*.stromx);;Zip (*.zip);;XML (*.xml)")); 
    
    if(file.isNull())
        return false;
    
    readFile(file);
    
    return true;
}

bool MainWindow::readFile(const QString& filepath)
{ 
    QString basename = QFileInfo(filepath).baseName();
    QString extension = QFileInfo(filepath).suffix();
    
    if(extension != "xml" && extension != "zip" && extension != "stromx")
    {
        QMessageBox::critical(this, tr("Failed to load file"),
                              tr("The file extension '%1' is not recognized").arg(extension),
                              QMessageBox::Ok, QMessageBox::Ok);
        return false;
    }
    
    // try to read the stream
    StreamModel* stream = 0;
    std::auto_ptr<stromx::runtime::FileInput> input;
    QString location;
    
    try
    {
        if(extension == "xml")
        {
            location = QFileInfo(filepath).absoluteDir().absolutePath();
            input = std::auto_ptr<stromx::runtime::FileInput>(new stromx::runtime::DirectoryFileInput(location.toStdString()));
        }
        else if(extension == "zip" || extension == "stromx")
        {
            location = filepath;
            input = std::auto_ptr<stromx::runtime::FileInput>(new stromx::runtime::ZipFileInput(filepath.toStdString()));
        }
    }
    catch(stromx::runtime::FileAccessFailed&)
    {
        QMessageBox::critical(this, tr("Failed to load file"),
                              tr("The location %1 could not be openend for reading").arg(location),
                              QMessageBox::Ok, QMessageBox::Ok);
        return false;
    }
    
    try
    {
        if(extension == "xml")
        {
            stream = new StreamModel(*input, basename, m_undoStack,
                                     m_operatorLibraryView->operatorLibraryModel(), this);
        }
        else if(extension == "zip" || extension == "stromx")
        {
            stream = new StreamModel(*input, "stream", m_undoStack,
                                     m_operatorLibraryView->operatorLibraryModel(), this);
        }
        
        updateCurrentFile(filepath);
    }
    catch(stromx::runtime::FileAccessFailed&)
    {
        QMessageBox::critical(this, tr("Failed to load file"),
                              tr("The location %1 could not be openend for reading").arg(location),
                              QMessageBox::Ok, QMessageBox::Ok);
        return false;
    }
    catch(ReadStreamFailed& e)
    {
        QMessageBox::critical(this, tr("Failed to load file"), e.what(),
                              QMessageBox::Ok, QMessageBox::Ok);
        return false;
    }
    
    try
    {
        if(extension == "xml")
        {
            stream->readStudioData(*input, basename);
        }
        else if(extension == "zip" || extension == "stromx")
        {
            stream->readStudioData(*input, "stream");
        }
        
    }
    catch(ReadStudioDataFailed& e)
    {
        QMessageBox::warning(this, tr("Loaded only part of file"), e.what(),
                             QMessageBox::Ok, QMessageBox::Ok);
    }
    
    // reading was successful
    // set the new stream
    setModel(stream);
    
    // read and restore the geometry of the observer windows
    try
    {
        if(extension == "xml")
        {
            readWindowStates(*input, basename);
        }
        else if(extension == "zip" || extension == "stromx")
        {
            readWindowStates(*input, "stream");
        }
        
    }
    catch(ReadStreamFailed& e)
    {
        QMessageBox::warning(this, tr("Loaded only part of file"), e.what(),
                             QMessageBox::Ok, QMessageBox::Ok);
    }
    
    // remember the last file
    QSettings settings("stromx", "stromx-studio");
    settings.setValue("lastStreamOpened", filepath);
    setWindowFilePath(filepath);

    // update the list of recently opened files
    QStringList files = settings.value("recentFileList").toStringList();
    files.removeAll(filepath);
    files.prepend(filepath);
    while (files.size() > MAX_RECENT_FILES)
        files.removeLast();
    settings.setValue("recentFileList", files);

    foreach (QWidget *widget, QApplication::topLevelWidgets())
    {
        MainWindow *mainWin = qobject_cast<MainWindow *>(widget);
        if (mainWin)
            mainWin->updateRecentFileActions();
    }
    
    return true;
}

void MainWindow::updateWindowTitle(bool undoStackIsClean)
{
    QString cleanMarker = undoStackIsClean ? "" : tr("*");
    QString file = m_currentFile.isEmpty() ? tr("Untitled") : QFileInfo(m_currentFile).fileName();
    QString title = file + cleanMarker + " - " + tr("stromx-studio");
    setWindowTitle(title);
}

void MainWindow::updateCurrentFile(const QString& filepath)
{
    m_currentFile = filepath;
    m_undoStack->setClean();
    updateWindowTitle(true);
}

void MainWindow::updateRecentFileActions()
{
    QSettings settings("stromx", "stromx-studio");
    QStringList files = settings.value("recentFileList").toStringList();

    int numRecentFiles = qMin(files.size(), int(MAX_RECENT_FILES));

    for (int i = 0; i < numRecentFiles; ++i) {
        QString text = tr("&%1 %2").arg(i + 1).arg(strippedName(files[i]));
        m_recentFileActs[i]->setText(text);
        m_recentFileActs[i]->setData(files[i]);
        m_recentFileActs[i]->setVisible(true);
    }
    for (int j = numRecentFiles; j < MAX_RECENT_FILES; ++j)
        m_recentFileActs[j]->setVisible(false);
    
    m_emptyRecentFilesAct->setEnabled(numRecentFiles > 0);
    m_recentFilesSeparatorAct->setVisible(numRecentFiles > 0);
}

bool MainWindow::saveBeforeClosing()
{
    if(! m_undoStack->isClean())
    { 
        QMessageBox msgBox;
        msgBox.setText(tr("The stream has been modified."));
        msgBox.setInformativeText(tr("Do you want to save your changes?"));
        msgBox.setStandardButtons(QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
        msgBox.setDefaultButton(QMessageBox::Save);
        int ret = msgBox.exec();
        
        switch (ret)
        {
        case QMessageBox::Save:
            // cancel closing if saving was cancelled
            if(! save())
                return false;
            break;
        case QMessageBox::Discard:
            // forget the changes
            break;
        case QMessageBox::Cancel:
            // cancel closing
            return false;
        default:
            Q_ASSERT(false);
            break;
        }
    }
    
    return true;
}



bool MainWindow::closeStream()
{
    if(! saveBeforeClosing())
        return false;
    
    // replace the current model with a new one
    StreamModel* newModel = new StreamModel(m_undoStack, 
                                            m_operatorLibraryView->operatorLibraryModel(), this);
    setModel(newModel);
    
    updateCurrentFile("");
    
    return true;
}

bool MainWindow::saveAs()
{
    QSettings settings("stromx", "stromx-studio");
    QString lastDir = settings.value("lastStreamSavedDir", QDir::home().absolutePath()).toString();
    
    QString file = QFileDialog::getSaveFileName(this, tr("Save stromx-studio project"),
                                                lastDir, tr("Stromx (*.stromx);;Zip (*.zip);;XML (*.xml)")); 
    
    if(file.isNull())
        return false;
    
    return writeFile(file);
}

bool MainWindow::writeFile(const QString& filepath)
{
    QString basename = QFileInfo(filepath).baseName();
    QString extension = QFileInfo(filepath).suffix();
    if(extension != "xml" && extension != "zip" && extension != "stromx")
    {
        QMessageBox::critical(this, tr("Failed to save file"), tr("Invalid file extension"),
                              QMessageBox::Ok, QMessageBox::Ok);
        return false;
    }
        
    // write the stream
    QString location;
    try
    {
        if(extension == "xml")
        {
            location = QFileInfo(filepath).absoluteDir().absolutePath();
            stromx::runtime::DirectoryFileOutput output(location.toStdString());
            m_streamEditor->streamEditorScene()->model()->write(output, basename);
            writeWindowStates(output, basename);
        }
        else if(extension == "zip" || extension == "stromx")
        {
            location = filepath;
            stromx::runtime::ZipFileOutput output(location.toStdString());
            m_streamEditor->streamEditorScene()->model()->write(output, "stream");
            writeWindowStates(output, "stream");
            
            // manually close the file to catch exceptions on writing
            output.close();
        }
    
        updateCurrentFile(filepath);
        
        // remember the last dir
        QSettings settings("stromx", "stromx-studio");
        settings.setValue("lastStreamSavedDir", QFileInfo(filepath).dir().absolutePath());
    }
    catch(stromx::runtime::FileAccessFailed&)
    {
        QMessageBox::critical(this, tr("Failed to save file"),
                              tr("The location %1 could not be openend for writing").arg(location),
                              QMessageBox::Ok, QMessageBox::Ok);
        return false;
    }
    catch(WriteStreamFailed& e)
    {
        QMessageBox::critical(this, tr("Failed to save file"), e.what(),
                              QMessageBox::Ok, QMessageBox::Ok);
        return false;
    }
    
    return true;
}

void MainWindow::readSettings()
{
    QSettings settings("stromx", "stromx-studio");
    restoreGeometry(settings.value("geometry").toByteArray());
    restoreState(settings.value("windowState").toByteArray());
}

void MainWindow::writeSettings()
{
    QSettings settings("stromx", "stromx-studio");
    settings.setValue("geometry", saveGeometry());
    settings.setValue("windowState", saveState());
}

void MainWindow::loadPackages()
{
    QSettings settings("stromx", "stromx-studio");
    QString lastDir = settings.value("lastPackage", QDir::home().absolutePath()).toString();
    
    QStringList files = QFileDialog::getOpenFileNames(
                            this,
                            tr("Select one or more stromx packages to open"),
                            lastDir,
#ifdef UNIX                          
                            tr("Package (*.so)")); 
#endif // UNIX
#ifdef WIN32                          
                            tr("Package (*.dll)")); 
#endif // WIN32
    
    // load each library
    // Qt documentation recommends to iterate over a copy of the file list
    QStringList fileList = files;
    foreach(QString file, fileList)
    {
        try
        {
            m_operatorLibraryView->operatorLibraryModel()->loadPackage(file);
        }
        catch(LoadPackageFailed&)
        {
            std::cout << "Failed to load '" << file.toStdString() << "'" << std::endl;
        }
    }
    
    // remember the last library
    if(files.size())
    {
        QString lastDir = files.back();
        settings.setValue("lastPackage", lastDir);
    }
}

void MainWindow::resetPackages()
{
    QMessageBox msgBox;
    msgBox.setText(tr("Do you want to reset the operator libraries?"));
    msgBox.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
    msgBox.setDefaultButton(QMessageBox::Cancel);
    int ret = msgBox.exec();
    
    if(ret == QMessageBox::Ok)
        m_operatorLibraryView->operatorLibraryModel()->resetLibrary();
}

QString MainWindow::strippedName(const QString &fullFileName)
{
    return QFileInfo(fullFileName).fileName();
}

void MainWindow::closeEvent(QCloseEvent* event)
{ 
    if(closeStream())
    {
        event->accept();
    }
    else
    {
        event->ignore();
    }
    
    writeSettings();
}

void MainWindow::setSlowProcessing(bool isSlow)
{
    m_model->setDelayActive(isSlow);
}

void MainWindow::emptyRecentFiles()
{
    QSettings settings("stromx", "stromx-studio");
    settings.setValue("recentFileList", QStringList());
    updateRecentFileActions();
}

void MainWindow::createObserverWindow(ObserverModel* observer)
{
    ObserverWindow* observerWindow = new ObserverWindow(observer, m_undoStack, this);
    m_observerWindows.append(observerWindow);
    m_viewMenu->addAction(observerWindow->showAction());
    m_viewMenuSeparatorAct->setVisible(m_observerWindows.count() > 0);
}

void MainWindow::destroyObserverWindow(ObserverModel* observer)
{
    ObserverWindow* window = 0;
    
    foreach(ObserverWindow* w, m_observerWindows)
    {
        if(w->observerView()->model() == observer)
            window = w;
    }
    
    Q_ASSERT(window);
    m_viewMenu->removeAction(window->showAction());
    m_observerWindows.removeAll(window);
    
    m_viewMenuSeparatorAct->setVisible(m_observerWindows.count() > 0);
    
    delete window;
}

void MainWindow::resetObserverWindows(StreamModel* model)
{
    foreach(ObserverWindow* window, m_observerWindows)
    {
        m_viewMenu->removeAction(window->showAction());
        delete window;
    }
    m_observerWindows.clear();
    
    foreach(ObserverModel* observer, model->observerModel()->observers())
        createObserverWindow(observer);
}

void MainWindow::readWindowStates(stromx::runtime::FileInput& input, const QString& basename)
{
    QByteArray data;
    
    try
    {
        input.initialize("", (basename + ".studio.geometry").toStdString());
        input.openFile(stromx::runtime::InputProvider::BINARY);
        
        // read all data from the input stream
        int dataSize = 0;
        const int CHUNK_SIZE = 10;
        while(! input.file().eof())
        {
            data.resize(data.size() + CHUNK_SIZE);
            char* dataPtr = data.data() + dataSize;
            input.file().read(dataPtr, CHUNK_SIZE);
            dataSize += (int)(input.file().gcount());
        }
        data.resize(dataSize);
    }
    catch(stromx::runtime::FileAccessFailed& e)
    {
        // simply ignore errors and do not update the window geometry
        qWarning() << e.what();
    }
    
    // construct a input stream from the data
    QDataStream dataStream(data);
    
    // read the state of the stream view
    QTransform viewTransform;
    dataStream >> viewTransform;
    m_streamEditor->setTransform(viewTransform);
    
    // read the state and geometry of the documentation window
    bool visible;
    dataStream >> data;
    m_docWindow->restoreGeometry(data);
    dataStream >> data;
    m_docWindow->restoreState(data);
    dataStream >> visible;
    m_docWindow->setVisible(visible);
        
    // read the state and geometry of each observer window
    foreach(ObserverWindow* window, m_observerWindows)
    {
        dataStream >> data;
        window->restoreGeometry(data);
        dataStream >> data;
        window->restoreState(data);
        dataStream >> visible;
        window->setVisible(visible);
        
        dataStream >> viewTransform;
        window->visualizer()->setTransform(viewTransform);
    }
}

void MainWindow::writeWindowStates(stromx::runtime::FileOutput& output, const QString& basename) const
{
    // construct an output stream
    QByteArray data;
    QDataStream dataStream(&data, QIODevice::WriteOnly | QIODevice::Truncate);
    
    // write the view transform of the stream view
    dataStream << m_streamEditor->transform();
    
    // write the state and geometry of the documentation window
    dataStream << m_docWindow->saveGeometry();
    dataStream << m_docWindow->saveState();
    dataStream << m_docWindow->isVisible();
    
    // write the state and geometry of each observer window
    foreach(ObserverWindow* window, m_observerWindows)
    {
        dataStream << window->saveGeometry();
        dataStream << window->saveState();
        dataStream << window->isVisible();
        dataStream << window->visualizer()->transform();
    }
    
    try
    {
        output.initialize(basename.toStdString());
        output.openFile("studio.geometry", stromx::runtime::OutputProvider::BINARY);
        output.file().write(data.data(), data.size());
    }
    catch(stromx::runtime::FileAccessFailed& e)
    {
        qWarning() << e.what();
        QString error = e.container().empty() 
                        ? tr("The file %1 could not be opened for writing.").arg(QString::fromStdString(e.filename()))
                        : tr("The file %1 in %2 could not be opened for writing.").arg(QString::fromStdString(e.filename()),
                                                                                       QString::fromStdString(e.container()));
        throw WriteStreamFailed(error);
    }
}

void MainWindow::handleAccessTimeout()
{
    QEvent* event = new QEvent(QEvent::Type(QEvent::User + Timeout));
    QCoreApplication::instance()->postEvent(this, event);
}

void MainWindow::customEvent(QEvent* event)
{
    if(event->type() == QEvent::Type(QEvent::User + Timeout) && ! m_timeoutMessageIsActive)
    {
        m_timeoutMessageIsActive = true;
        
        QMessageBox msgBox;
        msgBox.setText(tr("\
        An operation accessing the stream was stopped because its waiting time exceeded \
        the current limit. The stream might be dead-locked in which case it should be \
        stopped. Otherwise the maximal waiting time for operations accessing the stream \
        can be increased in <b>Stream -> Settings...</b>"));
        msgBox.setInformativeText(tr("Do you want to stop the stream?"));
        msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
        msgBox.setDefaultButton(QMessageBox::No);
        int ret = msgBox.exec();
        
        switch (ret)
        {
        case QMessageBox::Yes:
            if(m_model->isActive())
                stop();
            break;
        case QMessageBox::No:
            break;
        default:
            Q_ASSERT(false);
        }
        
        m_timeoutMessageIsActive = false;
    }
}

void MainWindow::findPackages()
{
    QFileInfoList packages = m_operatorLibraryView->operatorLibraryModel()->findInstalledPackages();
    m_findPackagesDialog->setPackages(packages);
    QSettings settings("stromx", "stromx-studio");
    bool showOnStartup = settings.value("showFindPackagesOnStartup", true).toBool();
    m_findPackagesDialog->setShowOnStartup(showOnStartup);
    if (m_findPackagesDialog->exec() == QDialog::Accepted)
    {
        foreach (const QFileInfo & info, m_findPackagesDialog->selectedPackages())
        {
            QString file = info.absoluteFilePath();
            try
            {
                m_operatorLibraryView->operatorLibraryModel()->loadPackage(file);
            }
            catch(LoadPackageFailed&)
            {
                std::cout << "Failed to load '" << file.toStdString() << "'" << std::endl;
            }
        }
    }
    settings.setValue("showFindPackagesOnStartup", m_findPackagesDialog->showOnStartup());
}


