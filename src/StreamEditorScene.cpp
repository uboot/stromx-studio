#include "StreamEditorScene.h"

#include <QGraphicsSceneDragDropEvent>
#include <stromx/core/Operator.h>
#include "StromxData.h"

StreamEditorScene::StreamEditorScene(QObject* parent)
  : QGraphicsScene(parent)
{
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
            delete *iter;
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
