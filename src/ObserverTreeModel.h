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

#ifndef OBSERVERTREEMODEL_H
#define OBSERVERTREEMODEL_H

#include <QAbstractItemModel>

class QUndoStack;
class ObserverListModel;

class ObserverTreeModel : public QAbstractItemModel
{
    Q_OBJECT
    
public:
    ObserverTreeModel(QUndoStack* undoStack, QObject * parent);

    virtual QModelIndex index(int row, int column, const QModelIndex & parent) const;
    virtual QModelIndex parent(const QModelIndex & child) const;
    virtual QVariant data(const QModelIndex & index, int role = Qt::DisplayRole) const;
    virtual QVariant headerData(int section, Qt::Orientation orientation, int role) const;
    virtual bool insertRows(int row, int count, const QModelIndex & parent = QModelIndex());
    virtual bool removeRows(int row, int count, const QModelIndex & parent = QModelIndex());
    virtual bool setData(const QModelIndex& index, const QVariant& value, int role);
    virtual Qt::ItemFlags flags(const QModelIndex& index) const;
    virtual bool dropMimeData(const QMimeData *data, Qt::DropAction action, int row, int column, const QModelIndex &parent);
    virtual int rowCount(const QModelIndex & parent) const;
    virtual int columnCount(const QModelIndex & parent) const;
    virtual QStringList mimeTypes() const;
    
private:
    QList<ObserverListModel*> m_lists;
    QUndoStack* m_undoStack;
};

#endif // OBSERVERTREEMODEL_H
