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

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class QAction;
class QUndoStack;
class QMenu;
class ObserverEditor;
class OperatorLibrary;
class PropertyEditor;
class StreamEditor;
class ThreadEditor;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void open();
    void saveAs();
    void closeStream();
    void save();
    void about();
    void start();
    void stop();

private:
    void createActions();
    void createMenus();
    void createToolBars();
    void createStatusBar();
    void createDockWindows();
    
    void readSettings();
    void writeSettings();
    
    QAction* m_openAct;
    QAction* m_saveAct;
    QAction* m_saveAsAct;
    QAction* m_closeAct;
    QAction* m_undoAct;
    QAction* m_redoAct;
    QAction* m_quitAct;
    QAction* m_aboutAct;
    QAction* m_aboutQtAct;
    QAction* m_startAct;
    QAction* m_stopAct;
    
    QUndoStack* m_undoStack;
    
    QMenu* m_fileMenu;
    QMenu* m_editMenu;
    QMenu* m_viewMenu;
    QMenu* m_helpMenu;
    
    QToolBar* m_streamToolBar;
    
    StreamEditor* m_streamEditor;
    ThreadEditor* m_threadEditor;
    ObserverEditor* m_observerEditor;
    OperatorLibrary* m_operatorLibrary;
    PropertyEditor* m_propertyEditor;
};

#endif // MAINWINDOW_H
