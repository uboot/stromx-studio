#include "StreamModel.h"

#include <stromx/core/Stream.h>
#include "AddConnectionCmd.h"
#include "AddOperatorCmd.h"
#include "AddThreadCmd.h"
#include "DeinitializeOperatorCmd.h"
#include "InitializeOperatorCmd.h"
#include "OperatorLibraryModel.h"
#include "OperatorModel.h"
#include "ConnectionModel.h"
#include "RemoveConnectionCmd.h"
#include "RemoveOperatorCmd.h"
#include "RemoveThreadCmd.h"
#include "ThreadListModel.h"
#include "ThreadModel.h"

StreamModel::StreamModel(QUndoStack* undoStack, OperatorLibraryModel* operatorLibrary, QObject* parent) 
  : QObject(parent),
    m_stream(0),
    m_threadListModel(0),
    m_operatorLibrary(operatorLibrary),
    m_undoStack(undoStack)
{
    m_stream = new stromx::core::Stream;
    m_threadListModel = new ThreadListModel(this);
}

StreamModel::~StreamModel()
{
    delete m_stream;
}

void StreamModel::addOperator(const OperatorData* opData, const QPointF& pos)
{
    m_undoStack->beginMacro(tr("add operator"));
    
    OperatorModel* opModel = new OperatorModel(m_operatorLibrary->newOperator(opData), this);
    opModel->setPos(pos);
    
    AddOperatorCmd* cmd = new AddOperatorCmd(this, opModel);
    m_undoStack->push(cmd);
    
    m_undoStack->endMacro();
}

void StreamModel::removeOperator(OperatorModel* op)
{
    m_undoStack->beginMacro(tr("remove operator"));
    
    if(op->isInitialized())
        deinitializeOperator(op);
    
    RemoveOperatorCmd* cmd = new RemoveOperatorCmd(this, op);
    m_undoStack->push(cmd);
    
    m_undoStack->endMacro();
}

void StreamModel::addConnection(OperatorModel* sourceOp, unsigned int outputId,
                                OperatorModel* targetOp, unsigned int inputId)
{
    ConnectionModel* connection = new ConnectionModel(sourceOp, outputId,
                                                      targetOp, inputId, this);
    
    AddConnectionCmd* cmd = new AddConnectionCmd(this, connection);
    m_undoStack->push(cmd);
}

void StreamModel::removeConnection(ConnectionModel* connection)
{
    RemoveConnectionCmd* cmd = new RemoveConnectionCmd(this, connection);
    m_undoStack->push(cmd);
}

void StreamModel::addThread()
{
    ThreadModel* threadModel = new ThreadModel(0, this);
    
    AddThreadCmd* cmd = new AddThreadCmd(this, threadModel);
    m_undoStack->push(cmd);
}

void StreamModel::removeThread(ThreadModel* thread)
{
    RemoveThreadCmd* cmd = new RemoveThreadCmd(this, thread);
    m_undoStack->push(cmd);
}

void StreamModel::initializeOperator(OperatorModel* op)
{
    InitializeOperatorCmd* cmd = new InitializeOperatorCmd(this, op);
    m_undoStack->push(cmd);
}

void StreamModel::deinitializeOperator(OperatorModel* op)
{
    m_undoStack->beginMacro(tr("deinitialize operator"));
    
    // before deinitialization remove all connections
    foreach(ConnectionModel* connection, op->connections())
        removeConnection(connection);
      
    DeinitializeOperatorCmd* cmd = new DeinitializeOperatorCmd(this, op);
    m_undoStack->push(cmd);
    
    m_undoStack->endMacro();
}

void StreamModel::doAddOperator(OperatorModel* op)
{
    m_operators.append(op);
    emit operatorAdded(op);
}

void StreamModel::doRemoveOperator(OperatorModel* op)
{
    m_operators.removeAll(op);
    emit operatorRemoved(op);
}

void StreamModel::doInitializeOperator(OperatorModel* op)
{
    op->setInitialized(true);
}

void StreamModel::doDeinitializeOperator(OperatorModel* op)
{
    op->setInitialized(false);
}

void StreamModel::doAddConnection(ConnectionModel* connection)
{
    connection->sourceOp()->addConnection(connection);
    connection->targetOp()->addConnection(connection);
    m_connections.append(connection);
    
    emit connectionAdded(connection);
}

void StreamModel::doRemoveConnection(ConnectionModel* connection)
{
    if(connection->sourceOp())
        connection->sourceOp()->removeConnection(connection);
    
    if(connection->targetOp())
        connection->targetOp()->removeConnection(connection);
    
    m_connections.removeAll(connection);
    
    emit connectionRemoved(connection);
}

void StreamModel::doAddThread(ThreadModel* threadModel)
{
    stromx::core::Thread* thread = m_stream->addThread();
    threadModel->setThread(thread);
    m_threadListModel->addThread(threadModel);
    emit threadAdded(threadModel);
}

void StreamModel::doRemoveThread(ThreadModel* threadModel)
{
    m_threadListModel->removeThread(threadModel);
    threadModel->setThread(0);
    emit threadRemoved(threadModel);
}






