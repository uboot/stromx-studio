/* 
*  Copyright 2011 xxx yyy
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

#ifndef OBSERVERTREEVIEW_H
#define OBSERVERTREEVIEW_H

#include <QTreeView>

class QAbstractItemModel;
class QModelIndex;

/** 
 * \brief Tree view to display observer tree models.
 * 
 * This view displays an observer tree. It provides functions to add
 * observers and inputs and move them around within the tree.
 */
class ObserverTreeView : public QTreeView
{
    Q_OBJECT

public:
    /** Constructs and observer tree view. */
    explicit ObserverTreeView(QWidget *parent = 0);
    
    void setModel(QAbstractItemModel* model);
    
    /** Returns an action which adds an observer to the tree when triggered. */
    QAction* createAddObserverAction(QObject* parent);
    
    /** Returns an action which removes the selected observer from the tree when triggered. */
    QAction* createRemoveObserverAction(QObject* parent);
    
    /** Returns an action which removes the selected input from the tree when triggered. */
    QAction* createRemoveInputAction(QObject* parent);
    
signals:
    /** This signal is emitted if an observer is selected or deselected in the view. */
    void observerSelectedChanged(bool selected);
    
    /** This signal is emitted if an input is selected or deselected in the view. */
    void inputSelectedChanged(bool selected);
    
protected:
    void contextMenuEvent(QContextMenuEvent* event);
    
private slots:
    void addObserver();
    void editObserverName();
    void removeSelectedEntry();
    void updateObserverSelected(const QModelIndex& current, const QModelIndex& previous);
    
private:
    QAction* m_addObserverAct;
    QAction* m_removeInputAct;
    QAction* m_removeObserverAct;
    QAction* m_editObserverNameAct;
};

#endif // OBSERVERTREEVIEW_H
