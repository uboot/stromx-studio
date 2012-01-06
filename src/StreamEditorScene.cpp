#include "StreamEditorScene.h"

#include <QGraphicsSceneDragDropEvent>
#include <stromx/core/Operator.h>
#include "OperatorItem.h"
#include "StreamModel.h"
#include "StromxData.h"

StreamEditorScene::StreamEditorScene(QObject* parent)
  : QGraphicsScene(parent),
    m_model(0)
{
    m_model = new StreamModel(this);
    
    connect(m_model, SIGNAL(operatorAdded(OperatorModel*)), this, SLOT(addOperator(OperatorModel*)));
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

void StreamEditorScene::addOperator(OperatorModel* op)
{
    OperatorItem* opItem = new OperatorItem(op);
    addItem(opItem);
}


