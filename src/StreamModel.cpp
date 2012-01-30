#include "StreamModel.h"

#include <QDir>
#include <QFileInfo>
#include <stromx/core/Description.h>
#include <stromx/core/DirectoryFileInput.h>
#include <stromx/core/DirectoryFileOutput.h>
#include <stromx/core/Operator.h>
#include <stromx/core/Stream.h>
#include <stromx/core/Thread.h>
#include <stromx/core/XmlReader.h>
#include <stromx/core/XmlWriter.h>
#include "AddConnectionCmd.h"
#include "AddOperatorCmd.h"
#include "AddThreadCmd.h"
#include "DeinitializeOperatorCmd.h"
#include "Exception.h"
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
    m_onlineOperators.append(op);
    m_stream->addOperator(op->op());
}

void StreamModel::doDeinitializeOperator(OperatorModel* op)
{
    if(! op->isInitialized())
        return;
    
    m_stream->removeOperator(op->op());
    m_onlineOperators.removeAll(op);
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
    QString baseName = QFileInfo(filename).baseName();
    QString directory = QFileInfo(filename).absoluteDir().absolutePath();
    
    try
    {
        stromx::core::DirectoryFileOutput output(directory.toStdString());
        stromx::core::XmlWriter writer;
        writer.writeStream(output, name.toStdString(), *m_stream);
        
        QByteArray modelData;
        serializeModel(modelData);
        output.initialize(baseName.toStdString());
        output.openFile("studio", stromx::core::OutputProvider::BINARY);
        output.file().write(modelData.data(), modelData.size());
    }
    catch(stromx::core::Exception& e)
    {
        qWarning(e.what());
        throw WriteStreamFailed();
    }
}

void StreamModel::read(const QString& filename)
{
    QString name = QFileInfo(filename).fileName();
    QString baseName = QFileInfo(filename).baseName();
    QString directory = QFileInfo(filename).absoluteDir().absolutePath();
    
    stromx::core::Stream* stream = 0;
    
    QByteArray modelData;
    
    try
    {
        stromx::core::DirectoryFileInput input(directory.toStdString());
        stromx::core::XmlReader reader;
        
        try
        {
            stream = reader.readStream(input, name.toStdString(), *m_operatorLibrary->factory());
        }
        catch(stromx::core::Exception& e)
        {
            qWarning(e.what());
            throw WriteStreamFailed();
        }
        
        bool failedToReadModel = false;
        
        try
        {
            input.initialize("", (baseName + ".studio").toStdString());
            input.openFile(stromx::core::InputProvider::BINARY);
            
            // read all data from the input stream
            int dataSize = 0;
            const int CHUNK_SIZE = 10;
            while(! input.file().eof())
            {
                modelData.resize(modelData.size() + CHUNK_SIZE);
                char* dataPtr = modelData.data() + dataSize;
                input.file().read(dataPtr, CHUNK_SIZE);
                dataSize += (int)(input.file().gcount());
            }
            modelData.resize(dataSize);
        }
        catch(stromx::core::Exception& e)
        {
            qWarning(e.what());
            failedToReadModel = true;
        }
        
        updateStream(stream);
        deserializeModel(modelData);
        
        // inform the clients
        emit modelWasReset();
        
        if(failedToReadModel)
            throw ReadModelFailed();
    
    }
    catch(stromx::core::Exception& e)
    {
        qWarning(e.what());
        throw WriteStreamFailed();
    }
}

void StreamModel::updateStream(stromx::core::Stream* stream)
{
    // clear the undo stack
    m_undoStack->clear();
    
    // backup all models
    QList<ConnectionModel*> connections = m_connections;
    QList<OperatorModel*> operators = m_operators;
    QList<ThreadModel*> threads = m_threadListModel->threads();
    
    // clear the lists
    m_connections.clear();
    m_operators.clear();
    m_offlineOperators.clear();
    m_onlineOperators.clear();
    m_threadListModel->removeAllThreads();
    
    // inform the clients
    emit modelWasReset();
    
    // delete the models
    foreach(ConnectionModel* connection, connections)
        delete connection;
    
    foreach(ThreadModel* thread, threads)
        delete thread;
    
    foreach(OperatorModel* op, operators)
        delete op;

    // reset the stream
    delete m_stream;
    m_stream = stream;
    
    for(std::vector<stromx::core::Operator*>::const_iterator iter = m_stream->operators().begin();
        iter != m_stream->operators().end();
        ++iter)
    {
        OperatorModel* op = new OperatorModel(*iter, this);
        m_operators.append(op);
        m_onlineOperators.append(op);
    }
    
    foreach(OperatorModel* opModel, m_onlineOperators)
    {
        stromx::core::Operator* op = opModel->op();
        
        for(std::vector<const stromx::core::Description*>::const_iterator inputIter = op->info().inputs().begin();
            inputIter != op->info().inputs().end();
            ++inputIter)
        {
            stromx::core::Output output = m_stream->connectionSource(op, (*inputIter)->id());
            
            if(output.valid())
            {
                OperatorModel* source = findOperatorModel(output.op());
                ConnectionModel* connection = new ConnectionModel(source, output.id(), opModel, (*inputIter)->id(), this);
                m_connections.append(connection);
            }
        }
    }
    
    for(std::vector<stromx::core::Thread*>::const_iterator iter = m_stream->threads().begin();
        iter != m_stream->threads().end();
        ++iter)
    {
        ThreadModel* thread = new ThreadModel(*iter, this);
        m_threadListModel->addThread(thread);
    }
    
    
    for(std::vector<stromx::core::Thread*>::const_iterator threadIter = m_stream->threads().begin();
        threadIter != m_stream->threads().end();
        ++threadIter)
    {
        ThreadModel* threadModel = findThreadModel(*threadIter);
        for(std::vector<stromx::core::Input>::const_iterator inputIter = (*threadIter)->inputSequence().begin();
            inputIter != (*threadIter)->inputSequence().end();
            ++inputIter)
        {
            ConnectionModel* connectionModel = findConnectionModel(*inputIter);
            connectionModel->setThread(threadModel);
        }
    }
}

OperatorModel* StreamModel::findOperatorModel(const stromx::core::Operator* op)
{
    foreach(OperatorModel* opModel, m_operators)
    {
        if(opModel->op() == op)
            return opModel;
    }
    
    return 0;
}

ConnectionModel* StreamModel::findConnectionModel(const stromx::core::Input& input)
{
    foreach(ConnectionModel* connectionModel, m_connections)
    {
        if(connectionModel->targetOp()->op() == input.op()
            && connectionModel->inputId() == input.id())
        {
            return connectionModel;
        }
    }
    
    return 0;
}

ThreadModel* StreamModel::findThreadModel(const stromx::core::Thread* thread)
{
    foreach(ThreadModel* threadModel, m_threadListModel->threads())
    {
        if(threadModel->thread() == thread)
            return threadModel;
    }
    
    return 0;
}

void StreamModel::serializeModel(QByteArray& data) const
{
    QDataStream dataStream(&data, QIODevice::WriteOnly | QIODevice::Truncate);
    
    foreach(OperatorModel* op, m_onlineOperators)
        dataStream << op;
    
    foreach(OperatorModel* op, m_offlineOperators)
        dataStream << op;
    
    foreach(ThreadModel* thread, m_threadListModel->threads())
        dataStream << thread;
}

void StreamModel::deserializeModel(const QByteArray& data)
{
    QDataStream dataStream(data);
    QList<ThreadModel*> threads;
    
    foreach(OperatorModel* op, m_onlineOperators)
        dataStream >>  op;
    
    foreach(OperatorModel* op, m_offlineOperators)
        dataStream >> op;
    
    foreach(ThreadModel* thread, m_threadListModel->threads())
        dataStream >> thread;
}






