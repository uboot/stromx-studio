#include "InputModel.h"

InputModel::InputModel(OperatorModel* op, unsigned int id, QObject* parent)
  : QObject(parent),
    m_op(op),
    m_id(id)
    
{

}
