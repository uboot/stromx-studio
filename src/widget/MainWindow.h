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

namespace stromx
{
    namespace runtime
    { 
        class FileInput;
        class FileOutput;
    }
}

class QAction;
class QMenu;
class DocumentationWindow;
class ErrorListView;
class FindPackagesDialog;
class LimitUndoStack;
class ObserverTreeView;
class ObserverModel;
class ObserverWindow;
class OperatorLibraryView;
class PropertyView;
class SettingsDialog;
class StreamEditor;
class StreamModel;
class ThreadListView;

/** \brief Application main window. */
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
    
public slots:
    /** Displays a window with packages found on the system. */
    void findPackages();
    
protected:
    virtual void closeEvent(QCloseEvent* event);
    virtual void customEvent(QEvent* event);

private slots:
    /** Attempts to close the current stream and displays an open file dialog. */
    bool open();
    
    /** Displays a save file dialog to save the current stream in a new file. */
    bool saveAs();
    
    /** Closes the current stream and loads an empty one. */
    bool closeStream();
    
    /** 
     * Saves the current stream. If no file is associated with the stream a
     * save file dialog is displayed.
     */
    bool save();
    
    /** 
     * Opens one of the recently opened files. The file to open is determined from the sender
     * of the signal which triggers this slot.
     */
    bool openRecentFile();
    
    /** Displays an open file dialog and loads the selected operator packages. */
    void loadPackages();
    
    /** Unloads all dynamically loaded operator packages. */
    void resetPackages();
    
    /** Displays a window containing information about this application. */
    void about();
    
    /** Starts the current stream and sets the corresponding actions to active and/or inactive. */
    void start();
    
    /** Pauses the current stream and sets the corresponding actions to active and/or inactive. */
    void pause();
    
    /** 
     * Tells the current stream to stop and sets the corresponding actions to active and/or inactive.
     * Note that the stream is only stopped after join() is called by the thread which waits
     * for the stream to join.
     */
    void stop();
    
    /** Sets all actions to active and/or inactive to reflect the fact that the stream has stopped */
    void join();
    
    /** 
     * Update the window title such that it shows the current file name and reflects the current
     * state of the undo stack.
     */
    void updateWindowTitle(bool undoStackIsClean);
    
    /** 
     * Activates or deactivates slow processing. If slow processing is active each stream waits
     * a predefined amount of time after setting data to an input.
     */ 
    void setSlowProcessing(bool isSlow);
    
    /** Clears the list of recently opened files. */
    void emptyRecentFiles();
    
    /** Creates a window for \c observer. */
    void createObserverWindow(ObserverModel* observer);
    
    /** Destroys the window \c observer. */
    void destroyObserverWindow(ObserverModel* observer);
    
    /** 
     * Removes all current observer windows and creates a new window
     * for each observer in the stream \c model. 
     */
    void resetObserverWindows(StreamModel* model);
    
    /** 
     * Opens a dialog which informs the user about a timeout and asks
     * whether the stream should be stopped.
     */
    void handleAccessTimeout();

private:
    enum
    { 
        /** The maximal number of recently opened files remembered. */
        MAX_RECENT_FILES = 10
    };
    
    /** Strips the path from the input file path. */
    static QString strippedName(const QString &fullFileName);
    
    /** Creates the actions of the main window. */
    void createActions();
    
    /** Creates and populates the menus of the main window. */
    void createMenus();
    
    /** Creates and populates the tool bars of the main window. */
    void createToolBars();
    
    /** Creates and populates the dock widgets of the main window. */
    void createDockWidgets();
    
    /** 
     * Sets the current stream model. All views are updated accordingly.
     * The parameter \c model must be non-zero but can be a default constructed
     * stream model.
     */
    void setModel(StreamModel* model);
    
    /**
     * Reads and applies the window geometry settings. */
    void readSettings();
    
    /** Writes the window geometry settings. */
    void writeSettings();
    
    /** 
     * Writes the currently opened file to file at \c filepath.
     * Returns true if the file was successfully written and false otherwise.
     */
    bool writeFile(const QString & filepath);
    
    /** 
     * Opens the file at \c filepath and reads a stream from it. If successful 
     * it replaces the currently displayed stream by the new one and returns true.
     * Returns false if the stream could not be read.
     */
    bool readFile(const QString & filepath);
    
    /**
     * Updates the recent file actions according to the list of the recently opened 
     * files (the list is stored as an application setting).
     */
    void updateRecentFileActions();
    
    /** Remembers the current file as being saved or opened and updates the undo stack. */
    void updateCurrentFile(const QString & filepath);
    
    /** Saves the stream before it is going to be closed. */
    bool saveBeforeClosing();
    
    /** Saves the geometry and view properties of the stream scene and the observer windows. */
    void writeWindowStates(stromx::runtime::FileOutput & output, const QString & basename) const;
    
    /** Reads and geometry and view properties of the stream scene and the observer windows. */
    void readWindowStates(stromx::runtime::FileInput & input, const QString & basename);
    
    QAction* m_openAct;
    QAction* m_saveAct;
    QAction* m_saveAsAct;
    QAction* m_closeAct;
    QAction* m_loadPackagesAct;
    QAction* m_resetPackagesAct;
    QAction* m_findPackagesAct;
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
    QAction* m_slowAction;
    QAction* m_emptyRecentFilesAct;
    QAction* m_viewMenuSeparatorAct;
    QAction* m_showOperatorLibraryAct;
    QAction* m_showPropertyViewAct;
    QAction* m_showObserverTreeViewAct;
    QAction* m_showErrorListViewAct;
    QAction* m_resetZoomAct;
    QAction* m_showSettingsAct;
    
    LimitUndoStack* m_undoStack;
    
    QMenu* m_fileMenu;
    QMenu* m_editMenu;
    QMenu* m_viewMenu;
    QMenu* m_helpMenu;
    QMenu* m_streamMenu;
    
    QToolBar* m_streamToolBar;
    
    QDockWidget* m_operatorLibraryDockWidget;
    QDockWidget* m_observerDockWidget;
    QDockWidget* m_propertyDockWidget;
    QDockWidget* m_errorDockWidget;
    
    StreamEditor* m_streamEditor;
    ThreadListView* m_threadListView;
    ObserverTreeView* m_observerTreeView;
    OperatorLibraryView* m_operatorLibraryView;
    PropertyView* m_propertyView;
    ErrorListView* m_errorListView;
    
    StreamModel* m_model;
     
    QString m_currentFile;
    bool m_timeoutMessageIsActive;
    QList<ObserverWindow*> m_observerWindows;
    DocumentationWindow* m_docWindow;
    SettingsDialog* m_settingsDialog;
    FindPackagesDialog* m_findPackagesDialog;
};

#endif // MAINWINDOW_H
