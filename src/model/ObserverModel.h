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

/**
 * \brief Model of a list of input observers.
 * 
 * An observer model contains a list of input models. The data arriving at the operator
 * inputs corresponding to these input models are displayed by a common visualizer.
 * The position of the input models in the list determine the display order in the
 * visualizer (assuming the visualizer supports some kind of layered drawing).
 */
class ObserverModel : public QAbstractTableModel
{
    Q_OBJECT
    
    friend class RenameObserverCmd;

public:
    /** Constructs an observer model. */
    ObserverModel(QUndoStack* undoStack, ObserverTreeModel* parent);
    
    /** 
     * Returns the name of the observer model. This is also the title of the window
     * which displays the observer.
     */
    const QString & name() const { return m_name; }
    
    /**
     * Sets the name of the observer model. This is also the title of the window
     * which displays the observer.
     */
    void setName(const QString & name);
    
    /** 
     * Returns the input model at \c position. Returns 0 if there is no input model
     * at the requested position.
     */
    InputModel* input(int position);
    
    /** Returns the list of inputs of the observer. */
    const QList<InputModel*> inputs() const { return m_inputs; }
    
    /** Returns the number of inputs of the observer. */
    int numInputs() const { return m_inputs.count(); }
    
    /** Inserts \c input at \c position. */
    void insertInput(int position, InputModel* input);
    
    /** Removes the input at \c position. */
    void removeInput(int position);
    
    /** Returns the observer tree model this model belongs to. */
    ObserverTreeModel* parentModel() const { return m_parent; }
    
    /** Returns the undo stack. */
    QUndoStack* undoStack() const { return m_undoStack; }
    
    virtual QModelIndex index(int row, int column, const QModelIndex& parent) const;
    virtual QVariant data(const QModelIndex & index, int role = Qt::DisplayRole) const;
    virtual bool setData(const QModelIndex& index, const QVariant& value, int role = Qt::EditRole);
    virtual QVariant headerData(int section, Qt::Orientation orientation, int role) const;
    virtual int rowCount(const QModelIndex & parent = QModelIndex()) const;
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
    
    /** An input was removed from the model. */
    void inputRemoved(InputModel* input, int pos);
    
    /** An input was moved within the model. */
    void inputMoved(InputModel* input, int srcPos, int destPos);
    
    /** An input was added to the model. */
    void inputAdded(InputModel* input, int pos);
    
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
     * Emits data changed. This slot is connected to the respective signal
     * of the parent model.
     */
    void handleDataChanged(const QModelIndex & topLeft, const QModelIndex & bottomRight);
    
    /**
     * Emits input added. This slot is connected to the respective signal
     * of the parent model.
     */
    void handleInputAdded(InputModel* input, ObserverModel* observer, int pos);
    
    /**
     * Emits input removed. This slot is connected to the respective signal
     * of the parent model.
     */
    void handleInputRemoved(InputModel* input, ObserverModel* observer, int pos);
    
    /**
     * Emits input moved. This slot is connected to the respective signal
     * of the parent model.
     */
    void handleInputMoved(InputModel* input, ObserverModel* srcObserver, int srcPos,
                          ObserverModel* destObserver, int destPos);
    
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
