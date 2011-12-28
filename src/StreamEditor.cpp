#include "StreamEditor.h"

#include <QDropEvent>
#include <QGraphicsView>
#include <stromx/core/Operator.h>
#include "StromxData.h"
#include "StreamEditorScene.h"

StreamEditor::StreamEditor(QWidget* parent)
  : QScrollArea(parent)
{
    QGraphicsView* view = new QGraphicsView;
    StreamEditorScene* scene = new StreamEditorScene;
    
    scene->setSceneRect(0, 0, 400, 400);
    view->setScene(scene);
    view->setAcceptDrops(true);
    view->setDragMode(QGraphicsView::RubberBandDrag);
    setWidget(view);
}

