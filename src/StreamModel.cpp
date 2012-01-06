#include "StreamModel.h"

#include "OperatorModel.h"

StreamModel::StreamModel(QObject* parent) 
  : QObject(parent),
    m_stream(0)
{

}

void StreamModel::addOperator(stromx::core::Operator* const op, const QPointF & pos)
{
    OperatorModel* opModel = new OperatorModel(op, this);
    opModel->setPos(pos);
    
    emit operatorAdded(opModel);
}
