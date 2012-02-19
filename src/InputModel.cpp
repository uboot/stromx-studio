#include "InputModel.h"

#include "ObserverListModel.h"

InputModel::InputModel(OperatorModel* op, unsigned int id, ObserverListModel* parent)
  : QObject(parent),
    m_op(op),
    m_id(id),
    m_listModel(parent)
    
{

}
