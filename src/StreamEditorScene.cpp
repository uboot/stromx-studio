#include "StreamEditorScene.h"

#include <QAction>
#include <QGraphicsSceneDragDropEvent>
#include <QKeyEvent>
#include <stromx/core/Operator.h>
#include "ConnectionItem.h"
#include "ConnectionModel.h"
#include "OperatorItem.h"
#include "OperatorModel.h"
#include "StreamModel.h"
#include "StromxData.h"

StreamEditorScene::StreamEditorScene(QObject* parent)
  : QGraphicsScene(parent),
    m_model(0)
{
    m_model = new StreamModel(this);
    
    connect(m_model, SIGNAL(operatorAdded(OperatorModel*)), this, SLOT(addOperator(OperatorModel*)));
    connect(m_model, SIGNAL(connectionAdded(ConnectionModel*)), this, SLOT(addConnection(ConnectionModel*)));
    connect(this, SIGNAL(selectionChanged()), this, SLOT(showSelectedModel()));
    connect(this, SIGNAL(selectionChanged()), this, SLOT(enableInitializeAction()));
}

void StreamEditorScene::dragEnterEvent(QGraphicsSceneDragDropEvent* event)
{
    const QMimeData* data = qobject_cast<const StromxData*>(event->mimeData());
    
    if(data)
    {  
        event->setDropAction(Qt::CopyAction);
        event->accept();
    }
}

void StreamEditorScene::dropEvent(QGraphicsSceneDragDropEvent* event)
{
    const StromxData* data = qobject_cast<const StromxData*>(event->mimeData());
    
    if(data)
    {  
        for(QSet<stromx::core::Operator*>::const_iterator iter = data->operators().begin();
            iter != data->operators().end();
            ++iter)
        {  
            m_model->addOperator(*iter, event->scenePos());
        }
        
        event->setDropAction(Qt::CopyAction);
        event->accept();
    }
}

void StreamEditorScene::dragMoveEvent(QGraphicsSceneDragDropEvent* event)
{
    const StromxData* data = qobject_cast<const StromxData*>(event->mimeData());
    
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
    QGraphicsItem* item = 0;
    foreach(item, selectedItems())
    {
        if(OperatorItem* opItem = qgraphicsitem_cast<OperatorItem*>(item))
            opItem->model()->setInitialized(true);
    }
}

void StreamEditorScene::showSelectedModel()
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
    if(selectedItems().size() == 0)
    {
        emit initializeEnabledChanged(false);
        return;
    }
        
    bool selectionIsValid = true;
    
    QGraphicsItem* item = 0;
    foreach(item, selectedItems())
    {
        if(! qgraphicsitem_cast<OperatorItem*>(item))
            selectionIsValid = false;
    }
    
    emit initializeEnabledChanged(selectionIsValid);
}

OperatorItem* StreamEditorScene::findOperatorItem(OperatorModel* opModel) const
{
    if(! opModel)
        return 0;
    
    QGraphicsItem* item = 0;
    foreach(item, items())
    {
        if(OperatorItem* opItem = qgraphicsitem_cast<OperatorItem*>(item))
        {
            if(opItem->model() == opModel)
                return opItem;
        }
    }
}

void StreamEditorScene::keyPressEvent(QKeyEvent* keyEvent)
{
    if(keyEvent->matches(QKeySequence::Delete))
    {    
        QGraphicsItem* item = 0;
        foreach(item, selectedItems())
        {        
            if(OperatorItem* opItem = qgraphicsitem_cast<OperatorItem*>(item))
                m_model->removeOperator(opItem->model());
        
            if(ConnectionItem* connectionItem = qgraphicsitem_cast<ConnectionItem*>(item))
                m_model->removeConnection(connectionItem->model());
        }
    }
    else
    {
        QGraphicsScene::keyPressEvent(keyEvent);
    }
}




