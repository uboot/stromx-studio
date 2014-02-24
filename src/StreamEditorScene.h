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

#ifndef STREAMEDITORSCENE_H
#define STREAMEDITORSCENE_H

#include <QGraphicsScene>
#include "item/OperatorItem.h"

class QAction;
class QAbstractTableModel;
class QGraphicsSceneDragDropEvent;
class QUndoStack;
class ConnectionModel;
class OperatorModel;
class PropertyModel;
class SelectionModel;
class StreamModel;
class ThreadModel;

/** 
 * \brief Visual editor for stream models.
 * 
 * A stream editor scene visualizes the contents of a stream model
 * and allows the user to edit them. It is derived from QGraphicsScene,
 * i.e. it must be embedded into a QGraphicsView widget to be displayed.
 */
class StreamEditorScene : public QGraphicsScene
{
    Q_OBJECT

public:
    /** Constructs a stream editor scene. */
    explicit StreamEditorScene(QObject *parent = 0);
    
    virtual void dropEvent(QGraphicsSceneDragDropEvent* event);
    virtual void dragMoveEvent(QGraphicsSceneDragDropEvent* event);
    virtual void dragEnterEvent(QGraphicsSceneDragDropEvent* event);
    
    QAction* createInitializeAction(QObject* parent);
    QAction* createDeinitializeAction(QObject* parent);
    QAction* createRemoveAction(QObject* parent);
    
    /** Returns the current stream model. */
    StreamModel* model() const { return m_model; }
    
    /** Sets the current stream model to \c model. The previous model is \em not deleted. */
    void setModel(StreamModel* model);
    
    /** Returns the current selection model. */
    SelectionModel* selectionModel() const { return m_selectionModel; }
    
signals:
    void initializeEnabledChanged(bool enabled);
    void deinitializeEnabledChanged(bool enabled);
    void removeEnabledChanged(bool enabled);
    void selectedModelChanged(PropertyModel* model);
    
    /** The model of the stream editor was reset, i.e. setModel() was called. */
    void modelWasReset(StreamModel* model);
   
protected:
    void keyPressEvent(QKeyEvent* keyEvent);
    
private slots:
    /** 
     * Removes all items from the view and re-populates it with new
     * items for the current stream model.
     */
    void reset();
    
    /**
     * Updates the property view and all actions to the currently selected items.
     * Issues signals to announce any changes due to a new selection.
     */
    void updateSelection();
    
    /** Adds an operator item for the model \c op to the scene. */
    void addOperator(OperatorModel* op);
    
    /** Removes the operator item representing the model \c op from the scene. */
    void removeOperator(OperatorModel* op);
    
    /** Adds a connection item for the model \c connection to the scene. */
    void addConnection(ConnectionModel* connection);
    
    /** Removes the connection item representing the model \c connection from the scene. */
    void removeConnection(ConnectionModel* connection);
    
    /** Initializes the currently selected operators. */
    void initialize();
    
    /** Deinitializes the currently selected operators. */
    void deinitialize();
    
    /** 
     * Removes the currently selected items. This removes the items from the scene
     * and the models the represent from the stream model.
     */
    void removeSelectedItems();
    
private:
    OperatorItem* findOperatorItem(OperatorModel* opModel) const;
    ConnectionItem* findConnectionItem(ConnectionModel* connectionModel) const;
    bool isOperatorSelection() const;
    
    StreamModel* m_model;
    SelectionModel* m_selectionModel;
    unsigned int m_numOperators;
};

#endif // STREAMEDITORSCENE_H
