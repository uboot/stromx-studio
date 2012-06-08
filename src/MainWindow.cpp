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
#include <stromx/core/DirectoryFileInput.h>
#include <stromx/core/DirectoryFileOutput.h>
#include <stromx/core/Exception.h>
#include <stromx/core/ZipFileInput.h>
#include <stromx/core/ZipFileOutput.h>
#include "Exception.h"
#include "LimitUndoStack.h"
#include "MainWindow.h"
#include "ObserverTreeModel.h"
#include "ObserverTreeView.h"
#include "ObserverView.h"
#include "ObserverWindow.h"
#include "OperatorLibraryView.h"
#include "OperatorLibraryModel.h"
#include "PropertyView.h"
#include "StreamEditor.h"
#include "StreamEditorScene.h"
#include "StreamModel.h"
#include "ThreadListView.h"

MainWindow::MainWindow(QWidget *parent)
  : QMainWindow(parent),
    m_model(0)
{
    m_undoStack = new LimitUndoStack(this);
    
    m_streamEditor = new StreamEditor;
    
    m_threadListView = new ThreadListView(this);
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
    createStatusBar();
    
    updateCurrentFile("");
    readSettings();
    
    connect(m_streamEditor->scene(), SIGNAL(selectedModelChanged(QAbstractItemModel*)),
            m_propertyView, SLOT(setModel(QAbstractItemModel*)));
    connect(m_streamEditor->scene(), SIGNAL(modelWasReset(StreamModel*)),
            this, SLOT(resetObserverWindows(StreamModel*)));
    connect(m_undoStack, SIGNAL(cleanChanged(bool)), this, SLOT(updateWindowTitle(bool)));
    connect(m_undoStack, SIGNAL(cleanChanged(bool)), m_saveAct, SLOT(setDisabled(bool)));
    
    OperatorLibraryModel* operatorLibraryModel = new OperatorLibraryModel(this);
    m_operatorLibraryView->setOperatorLibraryModel(operatorLibraryModel);
    
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
    
    m_errorDockWidget = new QDockWidget(this);
    m_errorDockWidget->setWindowTitle(tr("Error log"));
    m_errorDockWidget->setObjectName("ErrorLog");
    m_errorDockWidget->setAllowedAreas(Qt::BottomDockWidgetArea);
    
    addDockWidget(Qt::LeftDockWidgetArea, m_propertyDockWidget);
    addDockWidget(Qt::RightDockWidgetArea, m_propertyDockWidget);
    addDockWidget(Qt::RightDockWidgetArea, m_observerDockWidget);
    addDockWidget(Qt::BottomDockWidgetArea, m_errorDockWidget);
    m_errorDockWidget->hide();
}

void MainWindow::setModel(StreamModel* model)
{
    Q_ASSERT(model);
    
    // clear the undo stack
    m_undoStack->clear();
    
    // the model is zero when this function is called for the first time
    if(m_model)
    {
        // stop the old stream
        stop();
        
        // call join to update the stream controls
        join();
    }
    
    // set all editors to the new model
    m_streamEditor->streamEditorScene()->setModel(model);
    m_threadListView->setStreamModel(model);
    m_observerTreeView->setModel(model->observerModel());
    
    // delete the old model
    if(m_model)
        delete m_model;
    
    // remember the new model
    m_model = model;
    connect(m_model, SIGNAL(streamJoined()), this, SLOT(join()));
    connect(m_model->observerModel(), SIGNAL(observerAdded(ObserverModel*)), this, SLOT(createObserverWindow(ObserverModel*)));
    connect(m_model->observerModel(), SIGNAL(observerRemoved(ObserverModel*)), this, SLOT(destroyObserverWindow(ObserverModel*)));
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

    m_loadLibrariesAct = new QAction(tr("&Load Libraries..."), this);
    m_loadLibrariesAct->setStatusTip(tr("Load operator libraries"));
    connect(m_loadLibrariesAct, SIGNAL(triggered()), this, SLOT(loadLibraries()));

    m_resetLibrariesAct = new QAction(tr("&Reset Libraries..."), this);
    m_resetLibrariesAct->setStatusTip(tr("Reset operator libraries"));
    connect(m_resetLibrariesAct, SIGNAL(triggered()), this, SLOT(resetLibraries()));

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

    m_aboutAct = new QAction(tr("&About"), this);
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
    m_emptyRecentFilesAct->setStatusTip(tr("Empties the list of recently opened files"));
    connect(m_emptyRecentFilesAct, SIGNAL(triggered(bool)), this, SLOT(emptyRecentFiles()));
    
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
    m_fileMenu->addAction(m_loadLibrariesAct);
    m_fileMenu->addAction(m_resetLibrariesAct);
    m_fileMenu->addSeparator();
    m_fileMenu->addAction(m_quitAct);

    m_editMenu = menuBar()->addMenu(tr("&Edit"));
    m_editMenu->addAction(m_undoAct);
    m_editMenu->addAction(m_redoAct);

    m_streamMenu = menuBar()->addMenu(tr("&Stream"));
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

    m_viewMenu = menuBar()->addMenu(tr("&View"));
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

void MainWindow::createStatusBar()
{
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
    QMessageBox::about(this, tr("About stromx-studio"),
                       tr("The <b>stromx-studio</b> is an editor for stromx streams."));
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
    m_model->start();
    m_startAct->setEnabled(false);
    m_pauseAct->setEnabled(true);
    m_stopAct->setEnabled(true);
    m_redoAct->setEnabled(false);
    m_undoAct->setEnabled(false);
    m_undoStack->activateLimit();
    m_saveAct->setEnabled(false);
    m_saveAsAct->setEnabled(false);
    m_openAct->setEnabled(false);
}

void MainWindow::stop()
{
    m_model->stop();
    m_startAct->setEnabled(false);
    m_pauseAct->setEnabled(false);
    m_stopAct->setEnabled(true);
}

void MainWindow::pause()
{
    m_model->pause();
    m_startAct->setEnabled(true);
    m_pauseAct->setEnabled(false);
    m_stopAct->setEnabled(true);
}

void MainWindow::join()
{
    m_saveAct->setEnabled(true);
    m_saveAsAct->setEnabled(true);
    m_openAct->setEnabled(true);
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
    std::auto_ptr<stromx::core::FileInput> input;
    QString location;
    
    try
    {
        if(extension == "xml")
        {
            location = QFileInfo(filepath).absoluteDir().absolutePath();
            input = std::auto_ptr<stromx::core::FileInput>(new stromx::core::DirectoryFileInput(location.toStdString()));
        }
        else if(extension == "zip" || extension == "stromx")
        {
            location = filepath;
            input = std::auto_ptr<stromx::core::FileInput>(new stromx::core::ZipFileInput(filepath.toStdString()));
        }
    }
    catch(stromx::core::FileAccessFailed&)
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
    catch(stromx::core::FileAccessFailed&)
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
    catch(ReadStreamFailed& e)
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
            readObserverWindowStates(*input, basename);
        }
        else if(extension == "zip" || extension == "stromx")
        {
            readObserverWindowStates(*input, "stream");
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
        msgBox.setInformativeText("Do you want to save your changes?");
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
    m_slowAction->setChecked(false);
    
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
            stromx::core::DirectoryFileOutput output(location.toStdString());
            m_streamEditor->streamEditorScene()->model()->write(output, basename);
            writeObserverWindowStates(output, basename);
        }
        else if(extension == "zip" || extension == "stromx")
        {
            location = filepath;
            stromx::core::ZipFileOutput output(location.toStdString());
            m_streamEditor->streamEditorScene()->model()->write(output, "stream");
            writeObserverWindowStates(output, "stream");
        }
    
        updateCurrentFile(filepath);
        
        // remember the last dir
        QSettings settings("stromx", "stromx-studio");
        settings.setValue("lastStreamSavedDir", QFileInfo(filepath).dir().absolutePath());
    }
    catch(stromx::core::FileAccessFailed&)
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

void MainWindow::loadLibraries()
{
    QSettings settings("stromx", "stromx-studio");
    QString lastDir = settings.value("lastLibrary", QDir::home().absolutePath()).toString();
    
    QStringList files = QFileDialog::getOpenFileNames(
                            this,
                            tr("Select one or more stromx libraries to open"),
                            lastDir,
#ifdef UNIX                          
                            tr("Libraries (*.so)")); 
#endif // UNIX
#ifdef WIN32                          
                            tr("Libraries (*.dll)")); 
#endif // WIN32
    
    // load each library
    // Qt documentation recommends to iterate over a copy of the file list
    QStringList fileList = files;
    foreach(QString file, fileList)
    {
        try
        {
            m_operatorLibraryView->operatorLibraryModel()->loadLibrary(file);
        }
        catch(LoadLibraryFailed&)
        {
            std::cout << "Failed to load '" << file.toStdString() << "'" << std::endl;
        }
    }
    
    // remember the last library
    if(files.size())
    {
        QString lastDir = files.back();
        settings.setValue("lastLibrary", lastDir);
    }
}

void MainWindow::resetLibraries()
{
    QMessageBox msgBox;
    msgBox.setText(tr("Do you want to reset the operator libraries?"));
    msgBox.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
    msgBox.setDefaultButton(QMessageBox::Cancel);
    int ret = msgBox.exec();
    
    if(ret == QMessageBox::Ok)
        m_operatorLibraryView->operatorLibraryModel()->resetLibraries();
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
    m_model->setDelay(isSlow);
}

void MainWindow::emptyRecentFiles()
{
    QSettings settings("stromx", "stromx-studio");
    settings.setValue("recentFileList", QStringList());
    updateRecentFileActions();
}

void MainWindow::createObserverWindow(ObserverModel* observer)
{
    ObserverWindow* observerWindow = new ObserverWindow(observer, this);
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

void MainWindow::readObserverWindowStates(stromx::core::FileInput& input, const QString& basename)
{
    QByteArray data;
    
    try
    {
        input.initialize("", (basename + ".studio.geometry").toStdString());
        input.openFile(stromx::core::InputProvider::BINARY);
        
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
    catch(stromx::core::FileAccessFailed& e)
    {
        // simply ignore errors and do not update the window geometry
        qWarning() << e.what();
    }
    
    // construct a input stream from the data
    QDataStream dataStream(data);
    
    // read the state and geometry of each observer window
    foreach(ObserverWindow* window, m_observerWindows)
    {
        QByteArray data;
        dataStream >> data;
        window->restoreGeometry(data);
        dataStream >> data;
        window->restoreState(data);
        bool visible;
        dataStream >> visible;
        window->setVisible(visible);
    }
}

void MainWindow::writeObserverWindowStates(stromx::core::FileOutput& output, const QString& basename) const
{
    // construct an output stream
    QByteArray data;
    QDataStream dataStream(&data, QIODevice::WriteOnly | QIODevice::Truncate);
      
    // write the state and geometry of each observer window
    foreach(ObserverWindow* window, m_observerWindows)
    {
        dataStream << window->saveGeometry();
        dataStream << window->saveState();
        dataStream << window->isVisible();
    }
    
    try
    {
        output.initialize(basename.toStdString());
        output.openFile("studio.geometry", stromx::core::OutputProvider::BINARY);
        output.file().write(data.data(), data.size());
    }
    catch(stromx::core::FileAccessFailed& e)
    {
        qWarning() << e.what();
        QString error = e.container().empty() 
                        ? tr("The file %1 could not be opened for writing.").arg(QString::fromStdString(e.filename()))
                        : tr("The file %1 in %2 could not be opened for writing.").arg(QString::fromStdString(e.filename()),
                                                                                      QString::fromStdString(e.container()));
        throw WriteStreamFailed(error);
    }
}




