#include "StreamModel.h"

#include <QDir>
#include <QFileInfo>
#include <stromx/core/DirectoryFileOutput.h>
#include <stromx/core/Stream.h>
#include <stromx/core/XmlWriter.h>
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
    Q_ASSERT(! op->isInitialized());
    
    m_operators.append(op);
    m_offlineOperators.append(op);
    emit operatorAdded(op);
}

void StreamModel::doRemoveOperator(OperatorModel* op)
{
    Q_ASSERT(! op->isInitialized());
    
    m_operators.removeAll(op);
    m_offlineOperators.removeAll(op);
    emit operatorRemoved(op);
}

void StreamModel::doInitializeOperator(OperatorModel* op)
{
    if(op->isInitialized())
        return;
    
    op->setInitialized(true);
    m_offlineOperators.removeAll(op);
    m_stream->addOperator(op->op());
}

void StreamModel::doDeinitializeOperator(OperatorModel* op)
{
    if(! op->isInitialized())
        return;
    
    m_stream->removeOperator(op->op());
    m_offlineOperators.append(op);
    op->setInitialized(false);
}

void StreamModel::doAddConnection(ConnectionModel* connection)
{
    connection->sourceOp()->addConnection(connection);
    connection->targetOp()->addConnection(connection);
    m_connections.append(connection);
    m_stream->connect(connection->sourceOp()->op(), connection->outputId(),
                      connection->targetOp()->op(), connection->inputId());
    
    emit connectionAdded(connection);
}

void StreamModel::doRemoveConnection(ConnectionModel* connection)
{
    m_stream->disconnect(connection->targetOp()->op(), connection->inputId());
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
    m_stream->removeThread(threadModel->thread());
    threadModel->setThread(0);
    emit threadRemoved(threadModel);
}

void StreamModel::write(const QString& filename) const
{
    QString name = QFileInfo(filename).fileName();
    QString directory = QFileInfo(filename).absoluteDir().absolutePath();
    
    try
    {
        stromx::core::DirectoryFileOutput output(directory.toStdString());
        stromx::core::XmlWriter writer;
        writer.writeStream(output, name.toStdString(), *m_stream);
    }
    catch(stromx::core::Exception& e)
    {
        qWarning(e.what());
    }
}

void StreamModel::read(const QString& filename)
{

}






