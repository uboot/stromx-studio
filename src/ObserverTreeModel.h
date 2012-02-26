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
#include "StreamModel.h"

class QUndoStack;
class InputModel;
class ObserverModel;

/** Represents all observers of a stream model. */
class ObserverTreeModel : public QAbstractItemModel
{
    Q_OBJECT
    
    friend class InsertObserverCmd;
    friend class RemoveObserverCmd;
    friend class InsertInputCmd;
    friend class RemoveInputCmd;
    friend QDataStream & operator<< (QDataStream & stream, const ObserverTreeModel * op);
    friend QDataStream & operator>> (QDataStream & stream, ObserverTreeModel * op);
    
public:
    /** Constructs an observer tree model. */
    ObserverTreeModel(QUndoStack* undoStack, StreamModel * parent);
    
    /** Returns the current observers. */
    const QList<ObserverModel*> observers() const { return m_observers; }

    virtual QModelIndex index(int row, int column, const QModelIndex & parent) const;
    virtual QModelIndex parent(const QModelIndex & child) const;
    virtual QVariant data(const QModelIndex & index, int role = Qt::DisplayRole) const;
    virtual QVariant headerData(int section, Qt::Orientation orientation, int role) const;
    
    /** Pushes an insert observer command on the undo stack. */
    virtual bool insertRows(int row, int count, const QModelIndex & parent = QModelIndex());
    
    /** 
     * Pushes a remove observer or an remove input command on the undo stack. The type of 
     * command depends on the specified row (either observer or input row).
     */
    virtual bool removeRows(int row, int count, const QModelIndex & parent = QModelIndex());
    
    virtual bool setData(const QModelIndex& index, const QVariant& value, int role = Qt::EditRole);
    virtual Qt::ItemFlags flags(const QModelIndex& index) const;
    virtual int rowCount(const QModelIndex & parent) const;
    virtual int columnCount(const QModelIndex & parent) const;
    virtual QStringList mimeTypes() const;
    QMimeData* mimeData ( const QModelIndexList & indexes ) const;
    
    /**
     * If \c data is of type InputData a new input model is allocated and an insert input 
     * command is pushed on the undo stack.
     */
    virtual bool dropMimeData(const QMimeData *data, Qt::DropAction action, int row, int column, const QModelIndex &parent);
    virtual Qt::DropActions supportedDropActions () const;
    
    /** Returns the undo stack. */
    QUndoStack* undoStack() const { return m_undoStack; }
    
signals:
    /** An observer was added to the model. */
    void observerAdded(ObserverModel* observer);
    
    /** An observer was removed from the model. */
    void observerRemoved(ObserverModel* observer);
    
private slots:
    /** Removes all inputs which are the target of the removed connection. */
    void removeConnectedInputs(ConnectionModel* connection);
    
    /** Updates the displayed observer name if the slot is invoked by an ObserverModel. */
    void updateObserverName(const QString & name);
    
private:
    void doInsertObserver(int pos, ObserverModel* observer);
    void doRemoveObserver(int pos);
    void doInsertInput(int observerPos, int inputPos, InputModel* input);
    void doRemoveInput(int observerPos, int inputPos);
    
    QList<ObserverModel*> m_observers;
    QUndoStack* m_undoStack;
    StreamModel* m_stream;
    
    bool m_isMovingInput;
};

QDataStream & operator<< (QDataStream & stream, const ObserverTreeModel * model);
QDataStream & operator>> (QDataStream & stream, ObserverTreeModel * model);

#endif // OBSERVERTREEMODEL_H
