#include "OperatorLibrary.h"

#include <QTreeView>
#include "OperatorLibraryModel.h"

OperatorLibrary::OperatorLibrary(QWidget* parent)
  : QDockWidget(parent),
    m_model(0),
    m_treeView(0)
{
    setWindowTitle(tr("Operator Library"));
    setObjectName("OperatorLibrary");
    
    m_model = new OperatorLibraryModel(this);
    m_treeView = new QTreeView;
    m_treeView->setModel(m_model);
    
    setWidget(m_treeView);
}
