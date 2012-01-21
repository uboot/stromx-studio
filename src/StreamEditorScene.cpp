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
    m_undoStack(0),
    m_model(0)
{
    connect(this, SIGNAL(selectionChanged()), this, SLOT(processSelection()));
    connect(this, SIGNAL(selectionChanged()), this, SLOT(enableInitializeAction()));
    connect(this, SIGNAL(selectionChanged()), this, SLOT(enableDeinitializeAction()));
}

void StreamEditorScene::setUndoStack(QUndoStack* undoStack)
{
    m_undoStack = undoStack;
}

void StreamEditorScene::setModel(StreamModel* model)
{
    if(m_model)
        m_model->disconnect(this);
    
    foreach(QGraphicsItem* item, items())
        delete item;
        
    m_model = model;
    // TODO: add existing operators and connections to the scene
    
    if(m_model)
    {
        connect(m_model, SIGNAL(operatorAdded(OperatorModel*)), this, SLOT(addOperator(OperatorModel*)));
        connect(m_model, SIGNAL(operatorRemoved(OperatorModel*)), this, SLOT(removeOperator(OperatorModel*)));
        connect(m_model, SIGNAL(connectionAdded(ConnectionModel*)), this, SLOT(addConnection(ConnectionModel*)));
        connect(m_model, SIGNAL(connectionRemoved(ConnectionModel*)), this, SLOT(removeConnection(ConnectionModel*)));  
    }
}

void StreamEditorScene::dragEnterEvent(QGraphicsSceneDragDropEvent* event)
{
    const OperatorData* data = qobject_cast<const OperatorData*>(event->mimeData());
    
    if(data)
    {  
        event->setDropAction(Qt::CopyAction);
        event->accept();
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
        delete connectionItem;
    }
}

void StreamEditorScene::initialize()
{
    beginMacro("initialize operators");
    foreach(QGraphicsItem* item, selectedItems())
    {
        if(OperatorItem* opItem = qgraphicsitem_cast<OperatorItem*>(item))
            m_model->initializeOperator(opItem->model());
    }
    endMacro();
}

void StreamEditorScene::deinitialize()
{
    beginMacro("deinitialize operators");
    foreach(QGraphicsItem* item, selectedItems())
    {
        if(OperatorItem* opItem = qgraphicsitem_cast<OperatorItem*>(item))
            m_model->deinitializeOperator(opItem->model());
    }
    endMacro();
}

void StreamEditorScene::processSelection()
{
    if(selectedItems().size() == 1)
    {
        QGraphicsItem* item = selectedItems()[0];
        QAbstractTableModel* model;
        
        if(OperatorItem* opItem = qgraphicsitem_cast<OperatorItem*>(item))
            model = opItem->model();
        
        if(ConnectionItem* connectionItem = qgraphicsitem_cast<ConnectionItem*>(item))
            model = connectionItem->model();
         
        if(model)
        {
            emit selectedModelChanged(model);
            return;
        }
    }
    
    emit selectedModelChanged(0);
}

void StreamEditorScene::enableInitializeAction()
{
    emit initializeEnabledChanged(isOperatorSelection());
}

void StreamEditorScene::enableDeinitializeAction()
{
    emit deinitializeEnabledChanged(isOperatorSelection());
}

bool StreamEditorScene::isOperatorSelection() const
{
    if(selectedItems().size() == 0)
        return false;
    
    foreach(QGraphicsItem* item, selectedItems())
    {
        if(! qgraphicsitem_cast<OperatorItem*>(item))
            return false;
    }
    
    return true;
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
    {   
        beginMacro("remove objects");
        foreach(QGraphicsItem* item, selectedItems())
        { 
            // items have been deleted because they were dependent on other deleted items
            // check the existence of each item separately           
            if(items().contains(item))
            {
                if(OperatorItem* opItem = qgraphicsitem_cast<OperatorItem*>(item))
                    m_model->removeOperator(opItem->model());
            
                if(ConnectionItem* connectionItem = qgraphicsitem_cast<ConnectionItem*>(item))
                    m_model->removeConnection(connectionItem->model());
            }
        }
        endMacro();
    }
    else
    {
        QGraphicsScene::keyPressEvent(keyEvent);
    }
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

void StreamEditorScene::beginMacro(const QString& text)
{
    if(m_undoStack)
        m_undoStack->beginMacro(text);
}

void StreamEditorScene::endMacro()
{
    if(m_undoStack)
        m_undoStack->endMacro();
}


