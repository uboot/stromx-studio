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

#ifndef OBSERVERMODEL_H
#define OBSERVERMODEL_H

#include <QAbstractTableModel>

class QUndoStack;
class InputModel;
class ObserverTreeModel;

class ObserverModel : public QAbstractTableModel
{
    Q_OBJECT
    
    friend class RenameObserverCmd;

public:
    ObserverModel(QUndoStack* undoStack, ObserverTreeModel* parent);
    
    const QString & name() const { return m_name; }
    void setName(const QString & name);
    
    InputModel* input(int position);
    const QList<InputModel*> inputs() const { return m_inputs; }
    int numInputs() const { return m_inputs.count(); }
    void insertInput(int position, InputModel* input);
    void removeInput(int position);
    
    virtual QModelIndex index(int row, int column, const QModelIndex& parent) const;
    virtual QVariant data(const QModelIndex & index, int role = Qt::DisplayRole) const;
    virtual QVariant headerData(int section, Qt::Orientation orientation, int role) const;
    virtual int rowCount(const QModelIndex & parent) const;
    virtual int columnCount(const QModelIndex & parent) const;
    virtual QStringList mimeTypes() const;
    QMimeData* mimeData ( const QModelIndexList & indexes ) const;
    virtual bool dropMimeData(const QMimeData *data, Qt::DropAction action, int row, int column, const QModelIndex &parent);
    virtual Qt::DropActions supportedDropActions () const;
    virtual Qt::ItemFlags flags(const QModelIndex& index) const;
    
signals:
    /** The name of the observer changed. */
    void nameChanged(const QString & name);
    
    /** A property of the observer changed. */
    void changed(ObserverModel* observer);
    
private slots:
    /** 
     * Calls beginInsertRows(). This slot is connected to the respective signal
     * of the parent model.
     */
    void handleRowsAboutToBeInserted(const QModelIndex & parent, int start, int end);
    
    /** 
     * Calls endInsertRows(). This slot is connected to the respective signal
     * of the parent model. 
     */
    void handleRowsInserted(const QModelIndex & parent, int start, int end);
    
    /** 
     * Calls beginRemoveRows(). This slot is connected to the respective signal
     * of the parent model.
     */
    void handleRowsAboutToBeRemoved(const QModelIndex & parent, int start, int end);
    
    /** 
     * Calls endRemoveRows(). This slot is connected to the respective signal
     * of the parent model. 
     */
    void handleRowsRemoved(const QModelIndex & parent, int start, int end);
    
    /**
     * Emits data changed. This slot is connected to the respective signale
     * of the parent model.
     */
    void handleDataChanged(const QModelIndex & topLeft, const QModelIndex & bottomRight);
    
private:
    /** Sets the name. */
    void doSetName(const QString & name);
    
    /** 
     * Returns true if \c parentModelIndex in the parent model refers to an
     * index of this observer.
     */
    bool concernsThisObserver(const QModelIndex & parentModelIndex) const;
    
    
    
    QUndoStack* m_undoStack;
    ObserverTreeModel* m_parent;
    QList<InputModel*> m_inputs;
    QString m_name;
};

#endif // OBSERVERMODEL_H
