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

#ifndef OBSERVERLISTMODEL_H
#define OBSERVERLISTMODEL_H

#include <QAbstractTableModel>

class QUndoStack;
class ObserverModel;
class ObserverTreeModel;
class Observer;

class ObserverListModel : public QAbstractTableModel
{
    Q_OBJECT
    
public:
    ObserverListModel(QUndoStack* undoStack, QObject * parent);
    
    virtual int rowCount(const QModelIndex & parent) const;
    virtual int columnCount(const QModelIndex & parent) const;
    virtual QVariant data(const QModelIndex & index, int role = Qt::DisplayRole) const;
    virtual QVariant headerData(int section, Qt::Orientation orientation, int role) const;
    
    const QString & name() const { return m_name; }
    void setName(const QString & name);
    
    void addObserver(Observer* observer);
    void removeObserver(Observer* observer);
    
private:
    QList<ObserverModel*> m_observer;
    QString m_name;
    QUndoStack* m_undoStack;
};

#endif // OBSERVERLISTMODEL_H
