#include "OperatorLibrary.h"

#include "OperatorLibraryList.h"
#include "OperatorLibraryModel.h"

OperatorLibrary::OperatorLibrary(QWidget* parent)
  : QDockWidget(parent),
    m_list(0)
{
    setWindowTitle(tr("Operator Library"));
    setObjectName("OperatorLibrary");
    
    m_model = new OperatorLibraryModel(this);
    
    m_list = new OperatorLibraryList;
    m_list->setOperatorLibraryModel(m_model);
    
    setWidget(m_list);
}


OperatorLibraryModel* const OperatorLibrary::model() const
{ 
    return m_model;
}