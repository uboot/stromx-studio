#include "ObserverEditor.h"

#include <QTreeView>
#include "ObserverTreeModel.h"

ObserverEditor::ObserverEditor(QWidget* parent)
  : QDockWidget(parent),
    m_model(0),
    m_observerView(0)
{
    setWindowTitle("Observers");
    setObjectName("ObserverEditor");
    
    m_observerView = new QTreeView;
    setWidget(m_observerView);
}

void ObserverEditor::setModel(QAbstractItemModel* model)
{
    m_observerView->setModel(model);
}
