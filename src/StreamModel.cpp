#include "StreamModel.h"

#include <stromx/core/Stream.h>
#include "OperatorModel.h"
#include "ConnectionModel.h"
#include "ThreadListModel.h"
#include "ThreadModel.h"

StreamModel::StreamModel(QObject* parent) 
  : QObject(parent),
    m_stream(0),
    m_threadListModel(0)
{
    m_stream = new stromx::core::Stream;
    m_threadListModel = new ThreadListModel(this);
}

StreamModel::~StreamModel()
{
    delete m_stream;
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
    ConnectionModel* connection = new ConnectionModel(sourceOp, outputId,
                                                      targetOp, inputId, this);
    
    sourceOp->addConnection(connection);
    targetOp->addConnection(connection);
    
    emit connectionAdded(connection);
}

void StreamModel::removeOperator(OperatorModel* op)
{
    if(op->isInitialized())
        op->setInitialized(false);
    
    emit operatorRemoved(op);
    
    delete op;
}

void StreamModel::removeConnection(ConnectionModel* connection)
{
    if(connection->sourceOp())
        connection->sourceOp()->removeConnection(connection);
    
    if(connection->targetOp())
        connection->targetOp()->removeConnection(connection);
    
    emit connectionRemoved(connection);
    
    delete connection;
}

void StreamModel::addThread()
{
    stromx::core::Thread* thread = m_stream->addThread();
    ThreadModel* threadModel = new ThreadModel(thread, this);
    m_threadListModel->addThread(threadModel);
    
    emit threadAdded(threadModel);
}


