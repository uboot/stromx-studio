#include "OperatorLibrary.h"

#include "OperatorLibraryList.h"

OperatorLibrary::OperatorLibrary(QWidget* parent)
  : QDockWidget(parent),
    m_list(0)
{
    setWindowTitle(tr("Operator Library"));
    setObjectName("OperatorLibrary");
    
    m_list = new OperatorLibraryList;
    
    setWidget(m_list);
}


OperatorLibraryModel* const OperatorLibrary::model() const
{ 
    return m_list->model();
}