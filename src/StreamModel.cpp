#include "StreamModel.h"

#include "OperatorModel.h"
#include "ConnectionModel.h"

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

void StreamModel::addConnection(OperatorModel* sourceOp, unsigned int outputId,
                                OperatorModel* targetOp, unsigned int inputId)
{
    ConnectionModel* connection = new ConnectionModel(targetOp, inputId, this);
    
    emit connectionAdded(connection);
}
