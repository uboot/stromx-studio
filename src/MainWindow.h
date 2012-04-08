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


/*
* TODO
* 
* The code for the managing the recent files is taken from the "Recent Files Example"
* in the Qt documentation. This should be mentioned at an appropriate place.
*/ 

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class QAction;
class QMenu;
class LimitUndoStack;
class ObserverEditor;
class ObserverModel;
class ObserverWindow;
class OperatorLibrary;
class PropertyEditor;
class StreamEditor;
class StreamModel;
class ThreadEditor;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    /**
     * Constructs a main window containing an operator library, a stream editor,
     * a property editor, a thread list and an observer editor.
     */
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    
protected:
    virtual void closeEvent(QCloseEvent* event);

private slots:
    bool open();
    bool saveAs();
    bool closeStream();
    bool save();
    bool openRecentFile();
    void loadLibraries();
    void resetLibraries();
    void about();
    void start();
    void pause();
    void stop();
    void join();
    void updateWindowTitle(bool undoStackIsClean);
    
    /** Creates a window for \c observer. */
    void createObserverWindow(ObserverModel* observer);
    
    /** Destroys the window \c observer. */
    void destroyObserverWindow(ObserverModel* observer);
    
    /** 
     * Removes all current observer windows and creates a new window
     * for each observer in the stream \c model. 
     */
    void resetObserverWindows(StreamModel* model);

private:
    enum { MAX_RECENT_FILES = 10 };
    static QString strippedName(const QString &fullFileName);
    
    void createActions();
    void createMenus();
    void createToolBars();
    void createStatusBar();
    
    void setModel(StreamModel* model);
    void readSettings();
    void writeSettings();
    void writeFile(const QString & filepath);
    void readFile(const QString & filepath);
    void updateRecentFileActions();
    
    /** Remembers the current file as being saved or opened and updates the undo stack. */
    void updateCurrentFile(const QString & filepath);
    
    /** Saves the stream before it is going to be closed. */
    bool saveBeforeClosing();
    
    QAction* m_openAct;
    QAction* m_saveAct;
    QAction* m_saveAsAct;
    QAction* m_closeAct;
    QAction* m_loadLibrariesAct;
    QAction* m_resetLibrariesAct;
    QAction* m_undoAct;
    QAction* m_redoAct;
    QAction* m_quitAct;
    QAction* m_aboutAct;
    QAction* m_aboutQtAct;
    QAction* m_startAct;
    QAction* m_pauseAct;
    QAction* m_stopAct;
    QAction* m_initializeAct;
    QAction* m_deinitializeAct;
    QAction* m_addThreadAct;
    QAction* m_removeThreadAct;
    QAction* m_removeSelectedItemsAct;
    QAction* m_recentFilesSeparatorAct;
    QAction* m_recentFileActs[MAX_RECENT_FILES];
    QAction* m_addObserverAct;
    QAction* m_removeObserverAct;
    QAction* m_removeInputAct;
    QAction* m_observerSeparatorAct;
    
    LimitUndoStack* m_undoStack;
    
    QMenu* m_fileMenu;
    QMenu* m_editMenu;
    QMenu* m_viewMenu;
    QMenu* m_helpMenu;
    QMenu* m_streamMenu;
    
    QToolBar* m_streamToolBar;
    
    StreamEditor* m_streamEditor;
    ThreadEditor* m_threadEditor;
    ObserverEditor* m_observerEditor;
    OperatorLibrary* m_operatorLibrary;
    PropertyEditor* m_propertyEditor;
    
    StreamModel* m_model;
     
    QString m_currentFile;
    QList<ObserverWindow*> m_observerWindows;
};

#endif // MAINWINDOW_H
