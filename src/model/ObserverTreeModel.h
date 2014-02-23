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
#include "InputModel.h"

class QUndoStack;
class InputModel;
class ObserverModel;

/** 
 * \brief List of observers models.
 *
 * An observer tree model contains a list of observers each of which
 * consists of a sequence of input models.
 * Observers can be added and removed from an observer tree model using 
 * the respective functions of QAbstractItemModel to insert and remove
 * rows.
 */
class ObserverTreeModel : public QAbstractItemModel
{
    Q_OBJECT
    
    friend class InsertObserverCmd;
    friend class RemoveObserverCmd;
    friend class InsertInputCmd;
    friend class MoveInputCmd;
    friend class RemoveInputCmd;
    friend QDataStream & operator<< (QDataStream & stream, const ObserverTreeModel * op);
    friend QDataStream & operator>> (QDataStream & stream, ObserverTreeModel * op);
    friend QDataStream & readVersion01 (QDataStream & stream, ObserverTreeModel * op);
    
public:
    /** Constructs an observer tree model. */
    ObserverTreeModel(QUndoStack* undoStack, StreamModel * parent);
    
    /** Returns the current observers. */
    const QList<ObserverModel*> observers() const { return m_observers; }
    
    /** Returns the stream model this observer tree belongs to. */
    StreamModel* streamModel() const { return m_stream; }

    virtual QModelIndex index(int row, int column, const QModelIndex & parent) const;
    virtual QModelIndex parent(const QModelIndex & child) const;
    virtual QVariant data(const QModelIndex & index, int role = Qt::DisplayRole) const;
    
    /** 
     * Pushes an remove input command on the undo stack. The 
     * command removes the input at the position \c pos of \c observer.
     */
    void removeInput(ObserverModel* observer, int pos);
    
    /** 
     *
     * Pushes an insert observer command on the undo stack and returns true if 
     * the position in the view refers to a top-level element, i.e. \c parent is invalid. 
     * Returns false if \c parent is valid.
     */
    virtual bool insertRows(int row, int count, const QModelIndex & parent = QModelIndex());
    
    /** 
     * Pushes a remove observer or an remove input command on the undo stack. The type of 
     * command depends on the specified row (either observer or input row).
     */
    virtual bool removeRows(int row, int count, const QModelIndex & parent = QModelIndex());
    
    virtual bool setData(const QModelIndex& index, const QVariant& value, int role = Qt::EditRole);
    virtual Qt::ItemFlags flags(const QModelIndex& index) const;
    virtual int rowCount(const QModelIndex & parent = QModelIndex()) const;
    virtual int columnCount(const QModelIndex & parent = QModelIndex()) const;
    virtual QStringList mimeTypes() const;
    
    /**
     * Returns an InputData object for InputModel the first index of \c indexes points to.
     * If the first index does not refer to an InputModel 0 is returned.
     */
    QMimeData* mimeData ( const QModelIndexList & indexes ) const;
    
    /**
     * If \c data is of type InputData a new input model is allocated and an insert input 
     * command is pushed on the undo stack.
     */
    virtual bool dropMimeData(const QMimeData *data, Qt::DropAction action, int row, int column, const QModelIndex &parent);
    virtual Qt::DropActions supportedDropActions () const;
    virtual Qt::DropActions supportedDragActions () const;
    
    /** Returns the undo stack. */
    QUndoStack* undoStack() const { return m_undoStack; }
    
    /** 
     * Returns the model index for the input at position \c inputPos of
     * \c observer.
     */
    QModelIndex inputIndex(const ObserverModel* observer, const int inputPos);
    
    /** 
     * Returns the model index for the observer at position \c observerPos.
     */
    QModelIndex observerIndex(const int observerPos);
    
signals:
    /** An observer was added to the model. */
    void observerAdded(ObserverModel* observer);
    
    /** An observer was removed from the model. */
    void observerRemoved(ObserverModel* observer);
    
    /** An input was removed from the model. */
    void inputRemoved(InputModel* input, ObserverModel* observer, int pos);
    
    /** An input was moved within the model. */
    void inputMoved(InputModel* input, ObserverModel* srcObserver, int srcPos,
                    ObserverModel* destObserver, int destPos);
    
    /** An input was added to the model. */
    void inputAdded(InputModel* input, ObserverModel* observer, int pos);
    
private slots:
    /** Removes all inputs which are the target of the removed connection. */
    void removeConnectedInputs(ConnectionModel* connection);
    
    /** Updates the displayed data of \c observer. */
    void updateObserver(ObserverModel* observer);
    
    /** Updates the displayed data of \c input. */
    void updateInput(InputModel* input);
    
    /** Distributes the \c dataChanged signal to the observer models. */
    void handleDataChanged(const QModelIndex& topLeft, const QModelIndex& bottomRight);
    
private:
    /** 
     * The number of columns of the model. Note that the number returned  by columnCount()
     * is smaller than this value because the model also provides data for the
     * class ObserverModel which is not displayed by the tree view.
     */
    const static int NUM_VISIBLE_COLUMNS = 2;
    
    void doInsertObserver(int pos, ObserverModel* observer);
    void doRemoveObserver(int pos);
    void doInsertInput(int observerPos, int inputPos, InputModel* input);
    void doMoveInput(int srcObserverPos, int srcInputPos, int destObserverPos, int destInputPos, InputModel* input);
    void doRemoveInput(int observerPos, int inputPos);
    
    QList<ObserverModel*> m_observers;
    QUndoStack* m_undoStack;
    StreamModel* m_stream;
    
    bool m_suppressRemoveRows;
};

QDataStream & operator<< (QDataStream & stream, const ObserverTreeModel * model);
QDataStream & operator>> (QDataStream & stream, ObserverTreeModel * model);
QDataStream & readVersion01 (QDataStream & stream, ObserverTreeModel * model);

#endif // OBSERVERTREEMODEL_H
