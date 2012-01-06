#include "StreamEditor.h"

#include <QDropEvent>
#include <QGraphicsView>
#include <stromx/core/Operator.h>
#include "OperatorItem.h"
#include "OperatorModel.h"
#include "StromxData.h"
#include "StreamEditorScene.h"

StreamEditor::StreamEditor(QWidget* parent)
  : QGraphicsView(parent)
{
    m_scene = new StreamEditorScene;
    
    m_scene->setSceneRect(0, 0, 1024, 1024);
    setScene(m_scene);
    setAcceptDrops(true);
    setDragMode(QGraphicsView::RubberBandDrag);
    
    connect(m_scene, SIGNAL(selectionChanged()), this, SLOT(showSelectedModel()));
}

void StreamEditor::showSelectedModel()
{
    if(m_scene->selectedItems().size() == 1)
    {
        if(OperatorItem* item = qgraphicsitem_cast<OperatorItem*>(m_scene->selectedItems()[0]))
        {
            emit selectedModelChanged(item->op());
            return;
        }
    }
    
    emit selectedModelChanged(0);
}
