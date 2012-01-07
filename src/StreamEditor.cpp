#include "StreamEditor.h"

#include <QGraphicsView>
#include "StreamEditorScene.h"

StreamEditor::StreamEditor(QWidget* parent)
  : QGraphicsView(parent)
{
    m_scene = new StreamEditorScene;
    
    m_scene->setSceneRect(0, 0, 1024, 1024);
    setScene(m_scene);
    setAcceptDrops(true);
    setDragMode(QGraphicsView::RubberBandDrag);
}
