/* 
*  Copyright 2012 Matthias Fuchs
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

#ifndef OBSERVERWINDOW_H
#define OBSERVERWINDOW_H

#include <QMainWindow>

class LimitUndoStack;
class DataVisualizer;
class ObserverModel;
class ObserverView;

/**
 * \brief Window which displays data of an observer.
 * 
 * An observer windows displays the data of the input observers of
 * an observer model. Whenever the data at a given input changes the 
 * display is updated.
 */
class ObserverWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    /** Constructs an observer windows from an observer model. */
    explicit ObserverWindow(ObserverModel* observer, LimitUndoStack* undoStack, QWidget* parent);
    
    /** Returns the observer view of the window. */
    ObserverView* observerView() const { return m_observerView; }
    
    /** Returns an action which shows this window when triggered. */
    QAction* showAction() const { return m_showAct; }
    
    /** Returns a pointer to the data visualizer in the observer window. */
    DataVisualizer* visualizer() const { return m_visualizer; }
    
private slots:
    /**
     * Changes the window title to \c name. This also updates the display
     * text of the action which shows the window.
     */
    void updateWindowTitle(const QString & name);
    
    /** Creates the actions of the observer window. */
    void createActions();
    
    /** Creates and populates the menus of the observer window. */
    void createMenus();
    
private:
    ObserverView* m_observerView;
    QAction* m_showAct;
    DataVisualizer* m_visualizer;
    QAction* m_undoAct;
    QAction* m_redoAct;
    QAction* m_resetZoomAct;
    LimitUndoStack* m_undoStack;
};

#endif // OBSERVERWINDOW_H