#include "StreamEditor.h"

#include <QDropEvent>
#include <QGraphicsView>
#include <stromx/core/Operator.h>
#include "StromxData.h"
#include "StreamEditorScene.h"

StreamEditor::StreamEditor(QWidget* parent)
  : QGraphicsView(parent)
{
    StreamEditorScene* scene = new StreamEditorScene;
    
    scene->setSceneRect(0, 0, 1024, 1024);
    setScene(scene);
    setAcceptDrops(true);
    setDragMode(QGraphicsView::RubberBandDrag);
}

