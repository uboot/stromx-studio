#include "StreamModel.h"

#include "AddConnectionCmd.h"
#include "AddOperatorCmd.h"
#include "AddThreadCmd.h"
#include "Config.h"
#include "DeinitializeOperatorCmd.h"
#include "Exception.h"
#include "ExceptionObserver.h"
#include "InitializeOperatorCmd.h"
#include "OperatorLibraryModel.h"
#include "OperatorModel.h"
#include "ConnectionModel.h"
#include "ObserverTreeModel.h"
#include "OperatorData.h"
#include "RemoveConnectionCmd.h"
#include "RemoveOperatorCmd.h"
#include "RemoveThreadCmd.h"
#include "ThreadListModel.h"
#include "ThreadModel.h"
#include <boost/bind.hpp>
#include <QDir>
#include <QFileInfo>
#include <QFutureWatcher>
#include <QtConcurrentRun>
#include <QtDebug>
#include <stromx/core/Description.h>
#include <stromx/core/DirectoryFileInput.h>
#include <stromx/core/FileOutput.h>
#include <stromx/core/Operator.h>
#include <stromx/core/SortInputsAlgorithm.h>
#include <stromx/core/Stream.h>
#include <stromx/core/Thread.h>
#include <stromx/core/XmlReader.h>
#include <stromx/core/XmlWriter.h>
#include <stromx/core/Factory.h>

const quint32 StreamModel::MAGIC_NUMBER = 0x20111202;
const unsigned int StreamModel::DELAY = 100;

StreamModel::StreamModel(QUndoStack* undoStack, OperatorLibraryModel* operatorLibrary, QObject* parent) 
  : QObject(parent),
    m_stream(0),
    m_threadListModel(0),
    m_observerModel(0),
    m_operatorLibrary(operatorLibrary),
    m_undoStack(undoStack),
    m_joinStreamWatcher(0),
    m_exceptionObserver(0)
{
    initializeSubModels();
}

StreamModel::StreamModel(stromx::core::FileInput& input, const QString& basename, QUndoStack* undoStack, OperatorLibraryModel* operatorLibrary, QObject* parent)
  : QObject(parent),
    m_stream(0),
    m_threadListModel(0),
    m_observerModel(0),
    m_operatorLibrary(operatorLibrary),
    m_undoStack(undoStack),
    m_joinStreamWatcher(0),
    m_exceptionObserver(0)
{
    initializeSubModels();
    
    stromx::core::Stream* stream = 0;
    std::string streamFilename = (basename + ".xml").toStdString();
    
    try
    {
        stromx::core::XmlReader reader;
        stream = reader.readStream(input, streamFilename, m_operatorLibrary->factory());
    }
    catch(stromx::core::FileAccessFailed& e)
    {
        qWarning() << e.what();
        QString error = e.container().empty() 
                        ? tr("The file %1 could not be opened for reading.").arg(QString::fromStdString(e.filename()))
                        : tr("The file %1 in %2 could not be opened for reading.").arg(QString::fromStdString(e.filename()),
                                                                                      QString::fromStdString(e.container()));
        throw ReadStreamFailed(error);
    }
    catch(stromx::core::InconsistentFileContent& e)
    {
        qWarning() << e.what();
        QString error = e.container().empty() 
                        ? tr("The content of file %1 is inconsistent.").arg(QString::fromStdString(e.filename()))
                        : tr("The content of file %1 in %2 is inconsistent.").arg(QString::fromStdString(e.filename()),
                                                                               QString::fromStdString(e.container()));
        throw ReadStreamFailed(error);
    }
    catch(stromx::core::InvalidFileFormat& e)
    {
        qWarning() << e.what();
        QString error = e.container().empty() 
                        ? tr("The format of file %1 is invalid.").arg(QString::fromStdString(e.filename()))
                        : tr("The format of file %1 in %2 is invalid.").arg(QString::fromStdString(e.filename()),
                                                                            QString::fromStdString(e.container()));
        throw ReadStreamFailed(error);
    }
    catch(stromx::core::OperatorAllocationFailed& e)
    {
        qWarning() << e.what();
        QString error = tr("Failed to allocate operator of type %1 of package %2.")
                        .arg(QString::fromStdString(e.type()), QString::fromStdString(e.package()));
        throw ReadStreamFailed(error);
    }
    catch(stromx::core::DataAllocationFailed& e)
    {
        qWarning() << e.what();
        QString error = tr("Failed to allocate data of type %1 of package %2.")
                        .arg(QString::fromStdString(e.type()), QString::fromStdString(e.package()));
        throw ReadStreamFailed(error);
    }
    catch(stromx::core::DeserializationError& e)
    {
        qWarning() << e.what();
        QString error = tr("Failed to deserialize data of type %1 in package %2.")
                        .arg(QString::fromStdString(e.type()), QString::fromStdString(e.package()));
        throw ReadStreamFailed(error);
    }
    catch(stromx::core::ParameterError& e)
    {
        qWarning() << e.what();
        const stromx::core::OperatorInfo& op = e.op();
        QString error = tr("Failed to set parameter %1 ('%2') of operator of type %3 in package %4.")
                        .arg(QString("%1").arg(e.parameter().id()),
                             QString::fromStdString(e.parameter().doc().title()),
                             QString::fromStdString(op.type()),
                             QString::fromStdString(op.package()));
        throw ReadStreamFailed(error);
    }
    catch(stromx::core::OperatorError& e)
    {
        qWarning() << e.what();
        const stromx::core::OperatorInfo& op = e.op();
        QString error = tr("Unknown error related to operator of type %1 in package %2: %3")
                        .arg(QString::fromStdString(op.type()), QString::fromStdString(op.package()),
                             QString::fromStdString(e.what()));
        throw ReadStreamFailed(error);
    }
    
    allocateObjects(stream);
}

void StreamModel::readStudioData(stromx::core::FileInput & input, const QString & basename)
{  
    try
    {
        input.initialize("", (basename + ".studio").toStdString());
        input.openFile(stromx::core::InputProvider::BINARY);
        
        // read all data from the input stream
        QByteArray modelData;
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
        
        // allocate the uninitialized operators and read all model data like 
        // operator positions, thread color etc.
        deserializeModel(modelData);
           
        // read the parameters values of the offlline operators
        std::string parametersFilename = (basename + "_uninitialized.xml").toStdString();
        std::vector<stromx::core::Operator*> uninitializedOperators;
        foreach(OperatorModel* opModel, m_uninitializedOperators)
            uninitializedOperators.push_back(opModel->op());

        stromx::core::XmlReader reader;
        reader.readParameters(input, parametersFilename,
                              m_operatorLibrary->factory(), uninitializedOperators);
    }
    catch(stromx::core::FileAccessFailed& e)
    {
        qWarning() << e.what();
        QString error = e.container().empty() 
                        ? tr("The file %1 could not be opened for reading.").arg(QString::fromStdString(e.filename()))
                        : tr("The file %1 in %2 could not be opened for reading.").arg(QString::fromStdString(e.filename()),
                                                                                      QString::fromStdString(e.container()));
        throw ReadStudioDataFailed(error);
    }
    catch(stromx::core::InconsistentFileContent& e)
    {
        qWarning() << e.what();
        QString error = e.container().empty() 
                        ? tr("The content of file %1 is inconsistent.").arg(QString::fromStdString(e.filename()))
                        : tr("The content of file %1 in %2 is inconsistent.").arg(QString::fromStdString(e.filename()),
                                                                               QString::fromStdString(e.container()));
        throw ReadStudioDataFailed(error);
    }
    catch(stromx::core::InvalidFileFormat& e)
    {
        qWarning() << e.what();
        QString error = e.container().empty() 
                        ? tr("The format of file %1 is invalid.").arg(QString::fromStdString(e.filename()))
                        : tr("The format of file %1 in %2 is invalid.").arg(QString::fromStdString(e.filename()),
                                                                           QString::fromStdString(e.container()));
        throw ReadStudioDataFailed(error);
    }
    catch(stromx::core::OperatorAllocationFailed& e)
    {
        qWarning() << e.what();
        QString error = tr("Failed to allocate operator of type %1 of package %2.")
                        .arg(QString::fromStdString(e.type()), QString::fromStdString(e.package()));
        throw ReadStudioDataFailed(error);
    }
    catch(stromx::core::DataAllocationFailed& e)
    {
        qWarning() << e.what();
        QString error = tr("Failed to allocate data of type %1 of package %2.")
                        .arg(QString::fromStdString(e.type()), QString::fromStdString(e.package()));
        throw ReadStudioDataFailed(error);
    }
    catch(stromx::core::DeserializationError& e)
    {
        qWarning() << e.what();
        QString error = tr("Failed to deserialize data of type %1 in package %2.")
                        .arg(QString::fromStdString(e.type()), QString::fromStdString(e.package()));
        throw ReadStudioDataFailed(error);
    }
}

void StreamModel::initializeSubModels()
{
    m_stream = new stromx::core::Stream;
    m_joinStreamWatcher = new QFutureWatcher<void>(this);
    m_threadListModel = new ThreadListModel(this);
    m_observerModel = new ObserverTreeModel(m_undoStack, this);
    
    connect(m_joinStreamWatcher, SIGNAL(finished()), this, SLOT(join()));
}

StreamModel::~StreamModel()
{   
    stop();
    m_joinStreamWatcher->waitForFinished();
    deleteAllData();
}

const QList<ThreadModel*> StreamModel::threads() const
{
    return m_threadListModel->threads();
}

ObserverTreeModel* StreamModel::observerModel() const
{
    return m_observerModel;
}

QAbstractItemModel* StreamModel::threadListModel() const
{ 
    return m_threadListModel;
}

int StreamModel::operatorId(const OperatorModel* op) const
{
    for(int i = 0; i < m_operators.count(); ++i)
    {
        if(op == m_operators[i])
            return i;
    }
    
    return -1;
}

void StreamModel::addOperator(const OperatorData* opData, const QPointF& pos)
{
    m_undoStack->beginMacro(tr("add operator"));
    
    OperatorModel* opModel = new OperatorModel(m_operatorLibrary->newOperator(opData), this);
    opModel->setPos(pos);
    opModel->setName(opData->type());
    
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
    m_undoStack->beginMacro(tr("remove connection"));
    
    // before removing the connection set its thread to 0, i.e. remove it 
    // from its current stromx thread
    connection->setThread(0);
    
    // push the remove connection command on the undo stack
    RemoveConnectionCmd* cmd = new RemoveConnectionCmd(this, connection);
    m_undoStack->push(cmd);
    
    m_undoStack->endMacro();
}

void StreamModel::addThread()
{
    ThreadModel* threadModel = new ThreadModel(this);
    
    AddThreadCmd* cmd = new AddThreadCmd(this, threadModel);
    m_undoStack->push(cmd);
}

void StreamModel::removeThread(ThreadModel* thread)
{
    m_undoStack->beginMacro(tr("remove thread"));
    
    foreach(ConnectionModel* connection, connections())
    {
        if(connection->thread() == thread)
            connection->setThread(0);
    }
    RemoveThreadCmd* cmd = new RemoveThreadCmd(this, thread);
    m_undoStack->push(cmd);
    
    m_undoStack->endMacro();
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
    m_uninitializedOperators.append(op);
    connect(op, SIGNAL(parameterErrorOccurred(stromx::core::ParameterError)), this, SLOT(handleParameterError(stromx::core::ParameterError)));
    
    emit operatorAdded(op);
}

void StreamModel::doRemoveOperator(OperatorModel* op)
{
    Q_ASSERT(! op->isInitialized());
    
    disconnect(op, SIGNAL(parameterErrorOccurred(stromx::core::ParameterError)));
    m_operators.removeAll(op);
    m_uninitializedOperators.removeAll(op);
    emit operatorRemoved(op);
}

void StreamModel::doInitializeOperator(OperatorModel* op)
{
    if(op->isInitialized())
        return;
    
    try
    {
        op->setInitialized(true);
        
        // Move the operator from the uninitialized operators to the
        // initialized ones. It must also moved within m_operators to
        // make sure that the order of m_operators is in sync with the
        // order of m_initializedOperators and m_uninitializedOperators.
        m_operators.removeAll(op);
        m_uninitializedOperators.removeAll(op);
        m_operators.insert(m_initializedOperators.count(), op);
        m_initializedOperators.append(op);
        
        m_stream->addOperator(op->op());
        connect(op, SIGNAL(streamAccessTimedOut()), this, SIGNAL(accessTimedOut()));
    }
    catch(stromx::core::OperatorError& e)
    {
        if(m_exceptionObserver)
            m_exceptionObserver->observe(stromx::core::ExceptionObserver::INITIALIZATION, e, 0);
    }
}

void StreamModel::doDeinitializeOperator(OperatorModel* op)
{
    if(! op->isInitialized())
        return;
    
    m_stream->removeOperator(op->op());
    
    try
    {
        disconnect(op, SIGNAL(streamAccessTimedOut()));
        op->setInitialized(false);
        
        // Move the operator from the initialized operators to the
        // uninitialized ones. It must also moved within m_operators to
        // make sure that the order of m_operators is in sync with the
        // order of m_initializedOperators and m_uninitializedOperators.
        m_operators.removeAll(op);
        m_initializedOperators.removeAll(op);
        m_operators.append(op);
        m_uninitializedOperators.append(op);
    }
    catch(stromx::core::OperatorError& e)
    {
        m_stream->addOperator(op->op());
        if(m_exceptionObserver)
            m_exceptionObserver->observe(stromx::core::ExceptionObserver::DEINITIALIZATION, e, 0);
    }
}

void StreamModel::doAddConnection(ConnectionModel* connection)
{
    connection->connectToOperators();
    m_connections.append(connection);
    m_stream->connect(connection->sourceOp()->op(), connection->outputId(),
                      connection->targetOp()->op(), connection->inputId());
    
    emit connectionAdded(connection);
}

void StreamModel::doRemoveConnection(ConnectionModel* connection)
{
    // connections must be removed from all threads before removed
    Q_ASSERT(! connection->thread()); 
    
    m_stream->disconnect(connection->targetOp()->op(), connection->inputId());
    
    connection->disconnectFromOperators();
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

void StreamModel::write(stromx::core::FileOutput & output, const QString& basename) const
{
    try
    {
        stromx::core::XmlWriter writer;
        writer.writeStream(output, basename.toStdString(), *m_stream);
        
        std::vector<const stromx::core::Operator*> uninitializedOperators;
        foreach(OperatorModel* opModel, m_uninitializedOperators)
            uninitializedOperators.push_back(opModel->op());
        writer.writeParameters(output, (basename + "_uninitialized").toStdString(), uninitializedOperators);
        
        QByteArray modelData;
        serializeModel(modelData);
        output.initialize(basename.toStdString());
        output.openFile("studio", stromx::core::OutputProvider::BINARY);
        output.file().write(modelData.data(), modelData.size());
    }
    catch(stromx::core::FileAccessFailed& e)
    {
        qWarning() << e.what();
        QString error = e.container().empty() 
                        ? tr("The file %1 could not be opened for writing.").arg(QString::fromStdString(e.filename()))
                        : tr("The file %1 in %2 could not be opened for writing.").arg(QString::fromStdString(e.filename()),
                                                                                      QString::fromStdString(e.container()));
        throw WriteStreamFailed(error);
    }
    catch(stromx::core::SerializationError & e)
    {
        qWarning() << e.what();
        QString error = tr("Failed to serialize data of type %1 in package %2.").arg(QString::fromStdString(e.type()),
                                                                                      QString::fromStdString(e.package()));
        throw WriteStreamFailed(error);
    }
}

void StreamModel::deleteAllData()
{
    // backup all models
    QList<ConnectionModel*> connections = m_connections;
    QList<OperatorModel*> operators = m_operators;
    QList<ThreadModel*> threads = m_threadListModel->threads();
    
    // backup the uninitialized operators
    QList<stromx::core::Operator*> uninitializedOperators;
    foreach(OperatorModel* op, m_uninitializedOperators)
        uninitializedOperators.append(op->op());
    
    // clear the lists
    m_connections.clear();
    m_operators.clear();
    m_uninitializedOperators.clear();
    m_initializedOperators.clear();
    m_threadListModel->removeAllThreads();
    
    // delete the models
    foreach(ConnectionModel* connection, connections)
        delete connection;
    
    foreach(ThreadModel* thread, threads)
        delete thread;
    
    foreach(OperatorModel* op, operators)
        delete op;

    // delete the uninitialized operators
    foreach(stromx::core::Operator* op, uninitializedOperators)
        delete op;
    
    // delete the stream
    delete m_stream;
    m_stream = 0;
}

void StreamModel::allocateObjects(stromx::core::Stream* stream)
{
    // get the new data
    m_stream = stream;
    
    for(std::vector<stromx::core::Operator*>::const_iterator iter = m_stream->operators().begin();
        iter != m_stream->operators().end();
        ++iter)
    {
        OperatorModel* op = new OperatorModel(*iter, this);
        m_operators.append(op);
        m_initializedOperators.append(op);
        connect(op, SIGNAL(streamAccessTimedOut()), this, SIGNAL(accessTimedOut()));
        connect(op, SIGNAL(parameterErrorOccurred(stromx::core::ParameterError)), this, SLOT(handleParameterError(stromx::core::ParameterError)));
    }
    
    foreach(OperatorModel* opModel, m_initializedOperators)
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
                connection->connectToOperators();
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
        std::vector<stromx::core::Input>::const_iterator start = (*threadIter)->inputSequence().begin();
        std::vector<stromx::core::Input>::const_iterator end = (*threadIter)->inputSequence().end();
        for(std::vector<stromx::core::Input>::const_iterator inputIter = start; inputIter != end; ++inputIter)
        {
            ConnectionModel* connectionModel = findConnectionModel(*inputIter);
            if(connectionModel)
                connectionModel->setThread(threadModel);
        }
    }
}

OperatorModel* StreamModel::findOperatorModel(const stromx::core::Operator* op) const
{
    foreach(OperatorModel* opModel, m_operators)
    {
        if(opModel->op() == op)
            return opModel;
    }
    
    return 0;
}

ConnectionModel* StreamModel::findConnectionModel(const stromx::core::Input& input) const
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

ThreadModel* StreamModel::findThreadModel(const stromx::core::Thread* thread) const
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
    
    dataStream << quint32(MAGIC_NUMBER);
    dataStream << quint32(STROMX_STUDIO_VERSION_MAJOR);
    dataStream << quint32(STROMX_STUDIO_VERSION_MINOR);
    dataStream << quint32(STROMX_STUDIO_VERSION_PATCH);
    
    dataStream.setVersion(QDataStream::Qt_4_7);
    
    dataStream << qint32(m_uninitializedOperators.count());
    foreach(OperatorModel* op, m_uninitializedOperators)
    {
        const stromx::core::OperatorInfo & info = op->op()->info();
        dataStream << QString::fromStdString(info.package());
        dataStream << QString::fromStdString(info.type());
        dataStream << quint32(info.version().major());
        dataStream << quint32(info.version().minor());
        dataStream << quint32(info.version().revision());
    }
    
    dataStream << qint32(m_initializedOperators.count());
    foreach(OperatorModel* op, m_initializedOperators)
        dataStream << op;
    
    dataStream << qint32(m_uninitializedOperators.count());
    foreach(OperatorModel* op, m_uninitializedOperators)
        dataStream << op;
    
    dataStream << qint32(m_threadListModel->threads().count());
    foreach(ThreadModel* thread, m_threadListModel->threads())
        dataStream << thread;
    
    dataStream << m_observerModel;
}

void StreamModel::deserializeModel(const QByteArray& data)
{
    QDataStream dataStream(data);
    quint32 magicNumber = 0;
    qint32 count = 0;
    quint32 versionMajor = 0;
    quint32 versionMinor = 0;
    quint32 versionPatch = 0;
    
    dataStream >> magicNumber;
    if(magicNumber != MAGIC_NUMBER)
        throw ReadStudioDataFailed(tr("This is not a stromx-studio file"));
    
    dataStream >> versionMajor;
    dataStream >> versionMinor;
    dataStream >> versionPatch;
    
    dataStream.setVersion(QDataStream::Qt_4_7);

    dataStream >> count;
    stromx::core::Factory& factory = m_operatorLibrary->factory();
    for(int i = 0; i < count; ++i)
    {
        QString package, type;
        quint32 version;
        
        dataStream >> package;
        dataStream >> type;
        dataStream >> version;
        dataStream >> version;
        dataStream >> version;
        
        OperatorModel* opModel = 0;
        stromx::core::Operator* op = factory.newOperator(package.toStdString(), type.toStdString());
        opModel = new OperatorModel(op, this);
        m_uninitializedOperators.append(opModel);
        m_operators.append(opModel);
    }
    
    dataStream >> count;
    if(count != m_initializedOperators.count())
        throw ReadStudioDataFailed(tr("Number of initialized operators does not match the stromx-studio data."));
    foreach(OperatorModel* op, m_initializedOperators)
        dataStream >>  op;
    
    dataStream >> count;
    if(count != m_uninitializedOperators.count())
        throw ReadStudioDataFailed(tr("Number of uninitialized operators does not match stromx-studio data."));
    foreach(OperatorModel* op, m_uninitializedOperators)
        dataStream >> op;
    
    dataStream >> count;
    if(count != m_threadListModel->threads().count())
        throw ReadStudioDataFailed(tr("Number of threads does not match stromx-studio data."));
    foreach(ThreadModel* thread, m_threadListModel->threads())
        dataStream >> thread;
    
    dataStream >> m_observerModel;
}

bool StreamModel::start()
{
    switch(m_stream->status())
    {
    case stromx::core::Stream::INACTIVE:
        {
            // sort inputs before starting
            stromx::core::SortInputsAlgorithm sort;
            sort.apply(*m_stream);
        }
        try
        {
            m_stream->start();
        }
        catch(stromx::core::OperatorError& e)
        {
            // report any errors to the error observer
            if(m_exceptionObserver)
                m_exceptionObserver->observe(stromx::core::ExceptionObserver::ACTIVATION, e, 0);
            
            // return with error
            return false;
        }
        
        break;
    case stromx::core::Stream::PAUSED:
        m_stream->resume();
        break;
    default:
        Q_ASSERT(false);
    }
    
    emit streamStarted();
    
    return true;
}

bool StreamModel::pause()
{
    m_stream->pause();
    emit streamPaused();
    
    return true;
}

bool StreamModel::stop()
{
    // do nothing if the stream is inactive
    if(m_stream->status() == stromx::core::Stream::INACTIVE)
        return true;
    
    // stop the stream
    m_stream->stop();
    emit streamStopped();
    
    // start the thread which waits for the stream to finish
    QFuture<void> future = QtConcurrent::run(boost::bind(&stromx::core::Stream::join, m_stream));
    m_joinStreamWatcher->setFuture(future);
    
    return true;
}

void StreamModel::join()
{
    // inform others that the stream has finished
    emit streamJoined();
}

bool StreamModel::isActive() const
{
    return m_stream->status() != stromx::core::Stream::INACTIVE;
}

void StreamModel::setDelay(bool active)
{
    if(active)
        m_stream->setDelay(DELAY);
    else
        m_stream->setDelay(0);
}

void StreamModel::setExceptionObserver(ExceptionObserver* observer)
{
    Q_ASSERT(observer);
    
    if(m_exceptionObserver)
        m_stream->removeObserver(m_exceptionObserver);
    
    m_stream->addObserver(observer);
    m_exceptionObserver = observer;
}

void StreamModel::handleParameterError(const stromx::core::ParameterError& e)
{
    if(m_exceptionObserver)
    {
        m_exceptionObserver->observe(
            stromx::core::ExceptionObserver::PARAMETER_ACCESS, e, 0);
    }
}





