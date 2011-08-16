#include "StreamEditor.h"

#include <QDropEvent>
#include <stromx/core/Operator.h>
#include "StromxData.h"
#include "StreamEditorScene.h"

StreamEditor::StreamEditor(QWidget* parent)
  : QGraphicsView(parent)
{
    StreamEditorScene* scene = new StreamEditorScene;
    
    setScene(scene);
    setAcceptDrops(true);
}

