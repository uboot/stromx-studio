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

#ifndef OBSERVEREDITOR_H
#define OBSERVEREDITOR_H

#include <QDockWidget>

class QAbstractItemModel;
class QModelIndex;
class QTreeView;

class ObserverEditor : public QDockWidget
{
    Q_OBJECT

public:
    explicit ObserverEditor(QWidget *parent = 0);
    
    void setModel(QAbstractItemModel* model);
    
    QAction* createAddObserverAction(QObject* parent);
    QAction* createRemoveObserverAction(QObject* parent);
    QAction* createRemoveInputAction(QObject* parent);
    
signals:
    void observerSelectedChanged(bool selected);
    void inputSelectedChanged(bool selected);
    
private slots:
    void addObserverList();
    void removeSelectedEntry();
    void updateObserverSelected(const QModelIndex& current, const QModelIndex& previous);
    
private:
    QTreeView* m_observerView;
};

#endif // OBSERVEREDITOR_H
