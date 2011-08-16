#include "StreamEditor.h"

#include <QDropEvent>
#include <stromx/core/Operator.h>
#include "StromxData.h"

StreamEditor::StreamEditor(QWidget* parent)
  : QGraphicsView(parent)
{
    setAcceptDrops(true);
}

void StreamEditor::dragEnterEvent(QDragEnterEvent* event)
{
    const QMimeData* data = qobject_cast<const StromxData*>(event->mimeData());
    
    if(data)
    {  
        event->setDropAction(Qt::CopyAction);
        event->accept();
    }
}

void StreamEditor::dropEvent(QDropEvent* event)
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
