#include "StreamEditorScene.h"

#include <QAction>
#include <QGraphicsSceneDragDropEvent>
#include <QKeyEvent>
#include <QUndoStack>
#include <stromx/core/Operator.h>
#include "ConnectionItem.h"
#include "ConnectionModel.h"
#include "OperatorData.h"
#include "OperatorItem.h"
#include "OperatorModel.h"
#include "StreamModel.h"

StreamEditorScene::StreamEditorScene(QObject* parent)
  : QGraphicsScene(parent),
    m_model(0)
{
    connect(this, SIGNAL(selectionChanged()), this, SLOT(updateSelection()));
}

void StreamEditorScene::setModel(StreamModel* model)
{
    if(m_model)
        m_model->disconnect(this);
        
    m_model = model;
    reset();
    
    if(m_model)
    {
        connect(m_model, SIGNAL(operatorAdded(OperatorModel*)), this, SLOT(addOperator(OperatorModel*)));
        connect(m_model, SIGNAL(operatorRemoved(OperatorModel*)), this, SLOT(removeOperator(OperatorModel*)));
        connect(m_model, SIGNAL(connectionAdded(ConnectionModel*)), this, SLOT(addConnection(ConnectionModel*)));
        connect(m_model, SIGNAL(connectionRemoved(ConnectionModel*)), this, SLOT(removeConnection(ConnectionModel*)));
        connect(m_model, SIGNAL(streamStarted()), this, SLOT(updateSelection()));
        connect(m_model, SIGNAL(streamJoined()), this, SLOT(updateSelection()));
    }
    
    emit modelWasReset(m_model);
}

void StreamEditorScene::dragEnterEvent(QGraphicsSceneDragDropEvent* event)
{
    const OperatorData* data = qobject_cast<const OperatorData*>(event->mimeData());
    
    if(data)
    {  
        event->setDropAction(Qt::CopyAction);
        event->accept();
    }
    else
    {
        event->ignore();
    }
}

void StreamEditorScene::dropEvent(QGraphicsSceneDragDropEvent* event)
{
    const OperatorData* data = qobject_cast<const OperatorData*>(event->mimeData());
    
    if(data)
    {  
        m_model->addOperator(data, event->scenePos());
        event->setDropAction(Qt::CopyAction);
        event->accept();
    }
}

void StreamEditorScene::dragMoveEvent(QGraphicsSceneDragDropEvent* event)
{
    const OperatorData* data = qobject_cast<const OperatorData*>(event->mimeData());
    
    if(data)
    {  
        event->setDropAction(Qt::CopyAction);
        event->accept();
    }
}

QAction* StreamEditorScene::createInitializeAction(QObject* parent)
{
    QAction* action = new QAction(tr("Initialize"), parent);
    action->setStatusTip(tr("Initialize the selected operators"));
    action->setShortcut(tr("Ctrl+I"));
    action->setEnabled(false);
    connect(action, SIGNAL(triggered()), this, SLOT(initialize()));
    connect(this, SIGNAL(initializeEnabledChanged(bool)), action, SLOT(setEnabled(bool)));
    
    return action;
}

QAction* StreamEditorScene::createDeinitializeAction(QObject* parent)
{
    QAction* action = new QAction(tr("Deinitialize"), parent);
    action->setStatusTip(tr("Deinitialize the selected operators"));
    action->setShortcut(tr("Ctrl+Shift+I"));
    action->setEnabled(false);
    connect(action, SIGNAL(triggered()), this, SLOT(deinitialize()));
    connect(this, SIGNAL(deinitializeEnabledChanged(bool)), action, SLOT(setEnabled(bool)));
    
    return action;
}

QAction* StreamEditorScene::createRemoveAction(QObject* parent)
{
    QAction* action = new QAction(tr("Remove items"), parent);
    action->setStatusTip(tr("Remove selected items"));
    action->setEnabled(false);
    connect(action, SIGNAL(triggered()), this, SLOT(removeSelectedItems()));
    connect(this, SIGNAL(removeEnabledChanged(bool)), action, SLOT(setEnabled(bool)));
    
    return action;
}

void StreamEditorScene::reset()
{
    QList<QGraphicsItem*> itemList = items();
    foreach(QGraphicsItem* item, itemList)
    {
        if(items().count(item))
            delete item;
    }
    
    if(m_model)
    {
        foreach(OperatorModel* op, m_model->operators())
            addOperator(op);
        
        foreach(ConnectionModel* connection, m_model->connections())
            addConnection(connection);
    }
}

void StreamEditorScene::addOperator(OperatorModel* op)
{
    OperatorItem* opItem = new OperatorItem(op);
    addItem(opItem);
}

void StreamEditorScene::addConnection(ConnectionModel* connection)
{
    ConnectionItem* connectionItem = new ConnectionItem(connection);
    addItem(connectionItem);
    
    OperatorItem* sourceOp = findOperatorItem(connection->sourceOp());
    OperatorItem* targetOp = findOperatorItem(connection->targetOp());
    
    if(sourceOp && targetOp)
    {
        sourceOp->addOutputConnection(connection->outputId(), connectionItem);
        targetOp->addInputConnection(connection->inputId(), connectionItem);
    }
    else
    {
        Q_ASSERT(sourceOp && targetOp);
    }
}

void StreamEditorScene::initialize()
{
    m_model->undoStack()->beginMacro("initialize operators");
    foreach(QGraphicsItem* item, selectedItems())
    {
        if(OperatorItem* opItem = qgraphicsitem_cast<OperatorItem*>(item))
            m_model->initializeOperator(opItem->model());
    }
    m_model->undoStack()->endMacro();
}

void StreamEditorScene::deinitialize()
{
    m_model->undoStack()->beginMacro("deinitialize operators");
    
    QList<QGraphicsItem*> items(selectedItems());
    foreach(QGraphicsItem* item, items)
    {
        // the item might have been removed while an operator was deinitialized
        if(! selectedItems().contains(item))
            continue;
        
        if(OperatorItem* opItem = qgraphicsitem_cast<OperatorItem*>(item))
            m_model->deinitializeOperator(opItem->model());
    }
    m_model->undoStack()->endMacro();
}

void StreamEditorScene::updateSelection()
{
    // enable the remove action
    if(selectedItems().size() > 0 && ! m_model->isActive())
        emit removeEnabledChanged(true);
    else
        emit removeEnabledChanged(false);
       
    // show selected model in the property editor
    if(selectedItems().size() == 1)
    {
        QGraphicsItem* item = selectedItems()[0];
        QAbstractTableModel* model = 0;
        
        if(OperatorItem* opItem = qgraphicsitem_cast<OperatorItem*>(item))
            model = opItem->model();
        
        if(ConnectionItem* connectionItem = qgraphicsitem_cast<ConnectionItem*>(item))
            model = connectionItem->model();
         
        // update the model if (1) a new operator was selected
        // or (2) the stream was activated which requires the
        // parameter of the shown operator to be initialized
        if(model)
            emit selectedModelChanged(model);
    }
    else
    {
        emit selectedModelChanged(0);
    }
    
    // activate the initialize action only if least one 
    // operator is selected and the stream is not active
    bool active = isOperatorSelection() && ! m_model->isActive();
    emit initializeEnabledChanged(active);
    emit deinitializeEnabledChanged(active);
}

bool StreamEditorScene::isOperatorSelection() const
{
    if(selectedItems().size() == 0)
        return false;
    
    bool foundOperator = false;
    foreach(QGraphicsItem* item, selectedItems())
    {
        if(qgraphicsitem_cast<OperatorItem*>(item))
            foundOperator = true;
    }
    
    return foundOperator;
}

OperatorItem* StreamEditorScene::findOperatorItem(OperatorModel* opModel) const
{
    if(! opModel)
        return 0;
    
    foreach(QGraphicsItem* item, items())
    {
        if(OperatorItem* opItem = qgraphicsitem_cast<OperatorItem*>(item))
        {
            if(opItem->model() == opModel)
                return opItem;
        }
    }
    
    return 0;
}

ConnectionItem* StreamEditorScene::findConnectionItem(ConnectionModel* connectionModel) const
{
    if(! connectionModel)
        return 0;
    
    foreach(QGraphicsItem* item, items())
    {
        if(ConnectionItem* connectionItem = qgraphicsitem_cast<ConnectionItem*>(item))
        {
            if(connectionItem->model() == connectionModel)
                return connectionItem;
        }
    }

    return 0;
}

void StreamEditorScene::keyPressEvent(QKeyEvent* keyEvent)
{
    if(keyEvent->matches(QKeySequence::Delete))
        removeSelectedItems();
    else
        QGraphicsScene::keyPressEvent(keyEvent);
}

void StreamEditorScene::removeSelectedItems()
{
    // can not remove objects while stream is active
    if(m_model->isActive())
        return;
    
    // return immediatly if no objects are selected
    // otherwise, an empty remove action would be pushed to the undo stack
    if(! selectedItems().count())
        return;
    
    m_model->undoStack()->beginMacro("remove objects");
    QList<QGraphicsItem*> itemList = selectedItems();
    
    // remove all selected connections first
    foreach(QGraphicsItem* item, selectedItems())
    { 
        // items have been deleted because they were dependent on other deleted items
        // check the existence of each item separately           
        if(items().contains(item))
        {
            if(ConnectionItem* connectionItem = qgraphicsitem_cast<ConnectionItem*>(item))
                m_model->removeConnection(connectionItem->model());
        }
    }
    
    // remove operators
    foreach(QGraphicsItem* item, selectedItems())
    { 
        // items have been deleted because they were dependent on other deleted items
        // check the existence of each item separately           
        if(items().contains(item))
        {
            if(OperatorItem* opItem = qgraphicsitem_cast<OperatorItem*>(item))
                m_model->removeOperator(opItem->model());
        }
    }
    
    m_model->undoStack()->endMacro();
}

void StreamEditorScene::removeOperator(OperatorModel* op)
{
    if(OperatorItem* item = findOperatorItem(op))
        delete item;
}

void StreamEditorScene::removeConnection(ConnectionModel* connection)
{
    if(ConnectionItem* item = findConnectionItem(connection))
    {
        OperatorModel* targetOp = item->model()->targetOp();
        OperatorModel* sourceOp = item->model()->sourceOp();
        
        if(OperatorItem* opItem = findOperatorItem(targetOp))
            opItem->removeConnection(item);
        
        if(OperatorItem* opItem = findOperatorItem(sourceOp))
            opItem->removeConnection(item);
            
        delete item;
    }
}



