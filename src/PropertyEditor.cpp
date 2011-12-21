#include "PropertyEditor.h"

PropertyEditor::PropertyEditor(QWidget* parent)
  : QDockWidget(parent)
{
    setWindowTitle(tr("Properties"));
    setObjectName("PropertyEditor");
}
