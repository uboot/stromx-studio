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
#include "OperatorItem.h"

class QAction;
class QAbstractTableModel;
class QGraphicsSceneDragDropEvent;
class QUndoStack;
class ConnectionModel;
class OperatorModel;
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
    
signals:
    void initializeEnabledChanged(bool enabled);
    void deinitializeEnabledChanged(bool enabled);
    void removeEnabledChanged(bool enabled);
    void selectedModelChanged(QAbstractItemModel* model);
    
    /** The model of the stream editor was reset, i.e. setModel() was called. */
    void modelWasReset(StreamModel* model);
   
protected:
    void keyPressEvent(QKeyEvent* keyEvent);
    
private slots:
    void reset();
    void updateSelection();
    void addOperator(OperatorModel* op);
    void removeOperator(OperatorModel* op);
    void addConnection(ConnectionModel* connection);
    void removeConnection(ConnectionModel* connection);
    void initialize();
    void deinitialize();
    void removeSelectedItems();
    
private:
    OperatorItem* findOperatorItem(OperatorModel* opModel) const;
    ConnectionItem* findConnectionItem(ConnectionModel* connectionModel) const;
    bool isOperatorSelection() const;
    
    StreamModel* m_model;
};

#endif // STREAMEDITORSCENE_H
