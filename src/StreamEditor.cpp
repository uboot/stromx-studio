#include "StreamEditor.h"
#include <QDropEvent>

StreamEditor::StreamEditor(QWidget* parent)
  : QGraphicsView(parent)
{
    setAcceptDrops(true);
}

void StreamEditor::dragEnterEvent(QDragEnterEvent* event)
{
    event->setDropAction(Qt::CopyAction);
    event->accept();
    
//     QGraphicsView::dragEnterEvent(event);
}

void StreamEditor::dropEvent(QDropEvent* event)
{
    event->setDropAction(Qt::CopyAction);
    event->accept();
    
    //QGraphicsView::dropEvent(event);
}
