#include "model/StreamModel.h"

#include <QDir>
#include <QFileInfo>
#include <QFutureWatcher>
#include <QtConcurrentRun>
#include <QtDebug>
#include <stromx/runtime/Description.h>
#include <stromx/runtime/DirectoryFileInput.h>
#include <stromx/runtime/FileOutput.h>
#include <stromx/runtime/Operator.h>
#include <stromx/runtime/SortInputsAlgorithm.h>
#include <stromx/runtime/Stream.h>
#include <stromx/runtime/Thread.h>
#include <stromx/runtime/XmlReader.h>
#include <stromx/runtime/XmlWriter.h>
#include <stromx/runtime/Factory.h>
#include "Common.h"
#include "Config.h"
#include "Exception.h"
#include "ExceptionObserver.h"
#include "cmd/AddConnectionCmd.h"
#include "cmd/AddOperatorCmd.h"
#include "cmd/AddThreadCmd.h"
#include "cmd/DeinitializeOperatorCmd.h"
#include "cmd/InitializeOperatorCmd.h"
#include "cmd/RemoveConnectionCmd.h"
#include "cmd/RemoveOperatorCmd.h"
#include "cmd/RemoveThreadCmd.h"
#include "cmd/SetStreamSettingsCmd.h"
#include "data/OperatorData.h"
#include "model/OperatorLibraryModel.h"
#include "model/OperatorModel.h"
#include "model/ConnectionModel.h"
#include "model/ObserverTreeModel.h"
#include "model/ThreadListModel.h"
#include "model/ThreadModel.h"

#ifdef __GNUG__
    #include <tr1/functional>
#else
    #include <functional>
#endif

const quint32 StreamModel::MAGIC_NUMBER = 0x20111202;
const bool StreamModel::DEFAULT_DELAY_ACTIVE = false;
const int StreamModel::DEFAULT_DELAY = 100;
const int StreamModel::DEFAULT_ACCESS_TIMEOUT = 5000;

const int StreamModel::STREAM_FORMAT_VERSION_MAJOR = 0;
const int StreamModel::STREAM_FORMAT_VERSION_MINOR = 2;
const int StreamModel::STREAM_FORMAT_VERSION_REVISION = 0;

const stromx::runtime::Version StreamModel::STREAM_FORMAT_V2(0, 2, 0);

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
    createTemplate();
}

StreamModel::StreamModel(stromx::runtime::FileInput& input, const QString& basename, QUndoStack* undoStack, OperatorLibraryModel* operatorLibrary, QObject* parent)
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
    
    stromx::runtime::Stream* stream = 0;
    std::string streamFilename = (basename + ".xml").toStdString();
    
    try
    {
        stromx::runtime::XmlReader reader;
        stream = reader.readStream(input, streamFilename, m_operatorLibrary->factory());
    }
    catch(stromx::runtime::FileAccessFailed& e)
    {
        qWarning() << e.what();
        QString error = e.container().empty() 
                        ? tr("The file %1 could not be opened for reading.").arg(QString::fromStdString(e.filename()))
                        : tr("The file %1 in %2 could not be opened for reading.").arg(QString::fromStdString(e.filename()),
                                                                                      QString::fromStdString(e.container()));
        throw ReadStreamFailed(error);
    }
    catch(stromx::runtime::InconsistentFileContent& e)
    {
        qWarning() << e.what();
        QString error = e.container().empty() 
                        ? tr("The content of file %1 is inconsistent.").arg(QString::fromStdString(e.filename()))
                        : tr("The content of file %1 in %2 is inconsistent.").arg(QString::fromStdString(e.filename()),
                                                                               QString::fromStdString(e.container()));
        throw ReadStreamFailed(error);
    }
    catch(stromx::runtime::InvalidFileFormat& e)
    {
        qWarning() << e.what();
        QString error = e.container().empty() 
                        ? tr("The format of file %1 is invalid.").arg(QString::fromStdString(e.filename()))
                        : tr("The format of file %1 in %2 is invalid.").arg(QString::fromStdString(e.filename()),
                                                                            QString::fromStdString(e.container()));
        throw ReadStreamFailed(error);
    }
    catch(stromx::runtime::OperatorAllocationFailed& e)
    {
        qWarning() << e.what();
        QString error = tr("\
Failed to allocate operator of type <i>%1</i> of package <i>%2</i>. \
It is possible that the library providing this operator has not been loaded. \
Use <b>File -> Load Libraries...</b> to load additional libraries.")
                        .arg(QString::fromStdString(e.type()), QString::fromStdString(e.package()));
        throw ReadStreamFailed(error);
    }
    catch(stromx::runtime::DataAllocationFailed& e)
    {
        qWarning() << e.what();
        QString error = tr("\
Failed to allocate data of type <i>%1</i> of package <i>%2</i>. \
It is possible that the library providing this data type has not been loaded. \
Use <b>File -> Load Libraries...</b> to load additional libraries.")
                        .arg(QString::fromStdString(e.type()), QString::fromStdString(e.package()));
        throw ReadStreamFailed(error);
    }
    catch(stromx::runtime::DeserializationError& e)
    {
        qWarning() << e.what();
        QString error = tr("Failed to deserialize data of type <i>%1</i> in package <i>%2</i>.")
                        .arg(QString::fromStdString(e.type()), QString::fromStdString(e.package()));
        throw ReadStreamFailed(error);
    }
    catch(stromx::runtime::ParameterError& e)
    {
        qWarning() << e.what();
        QString error = tr("Failed to set parameter <i>%1</i> ('%2') of operator of type <i>%3</i> in package <i>%4</i>.")
                        .arg(QString("%1").arg(e.id()),
                             fromStromxTitle(e.title()),
                             QString::fromStdString(e.type()),
                             QString::fromStdString(e.package()));
        throw ReadStreamFailed(error);
    }
    catch(stromx::runtime::OperatorError& e)
    {
        qWarning() << e.what();
        QString error = tr("Unknown error related to operator of type <i>%1</i> in package <i>%2</i>: %3")
                        .arg(QString::fromStdString(e.type()), QString::fromStdString(e.package()),
                             QString::fromStdString(e.what()));
        throw ReadStreamFailed(error);
    }
    
    allocateObjects(stream);
}

void StreamModel::readStudioData(stromx::runtime::FileInput & input, const QString & basename)
{  
    try
    {
        input.initialize("", (basename + ".studio").toStdString());
        input.openFile(stromx::runtime::InputProvider::BINARY);
        
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
    }
    catch(stromx::runtime::FileAccessFailed& e)
    {
        qWarning() << e.what();
        QString error = e.container().empty() 
                        ? tr("The file %1 could not be opened for reading.").arg(QString::fromStdString(e.filename()))
                        : tr("The file %1 in %2 could not be opened for reading.").arg(QString::fromStdString(e.filename()),
                                                                                      QString::fromStdString(e.container()));
        throw ReadStudioDataFailed(error);
    }
    catch(stromx::runtime::InconsistentFileContent& e)
    {
        qWarning() << e.what();
        QString error = e.container().empty() 
                        ? tr("The content of file %1 is inconsistent.").arg(QString::fromStdString(e.filename()))
                        : tr("The content of file %1 in %2 is inconsistent.").arg(QString::fromStdString(e.filename()),
                                                                               QString::fromStdString(e.container()));
        throw ReadStudioDataFailed(error);
    }
    catch(stromx::runtime::InvalidFileFormat& e)
    {
        qWarning() << e.what();
        QString error = e.container().empty() 
                        ? tr("The format of file %1 is invalid.").arg(QString::fromStdString(e.filename()))
                        : tr("The format of file %1 in %2 is invalid.").arg(QString::fromStdString(e.filename()),
                                                                           QString::fromStdString(e.container()));
        throw ReadStudioDataFailed(error);
    }
    catch(stromx::runtime::OperatorAllocationFailed& e)
    {
        qWarning() << e.what();
        QString error = tr("Failed to allocate operator of type %1 of package %2.")
                        .arg(QString::fromStdString(e.type()), QString::fromStdString(e.package()));
        throw ReadStudioDataFailed(error);
    }
    catch(stromx::runtime::DataAllocationFailed& e)
    {
        qWarning() << e.what();
        QString error = tr("Failed to allocate data of type %1 of package %2.")
                        .arg(QString::fromStdString(e.type()), QString::fromStdString(e.package()));
        throw ReadStudioDataFailed(error);
    }
    catch(stromx::runtime::DeserializationError& e)
    {
        qWarning() << e.what();
        QString error = tr("Failed to deserialize data of type %1 in package %2.")
                        .arg(QString::fromStdString(e.type()), QString::fromStdString(e.package()));
        throw ReadStudioDataFailed(error);
    }
}

void StreamModel::initializeSubModels()
{
    m_stream = new stromx::runtime::Stream;
    m_joinStreamWatcher = new QFutureWatcher<void>(this);
    m_threadListModel = new ThreadListModel(this);
    m_observerModel = new ObserverTreeModel(m_undoStack, this);
    
    connect(m_joinStreamWatcher, SIGNAL(finished()), this, SIGNAL(streamJoined()));
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
    
    stromx::runtime::OperatorKernel* kernel = m_operatorLibrary->newOperator(opData);
    stromx::runtime::Operator* op = m_stream->addOperator(kernel);
    m_stream->hideOperator(op);
    OperatorModel* opModel = new OperatorModel(op, this);
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
    
    m_undoStack->beginMacro(tr("add connection"));
    AddConnectionCmd* cmd = new AddConnectionCmd(this, connection);
    m_undoStack->push(cmd);
    
    if(m_threadListModel->threads().length() > 0)
    {
        ThreadModel* thread = m_threadListModel->threads().first();
        connection->setThread(thread);
    }
    m_undoStack->endMacro();
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
    stromx::runtime::Thread* thread = m_stream->addThread();
    ThreadModel* threadModel = new ThreadModel(thread, this);
    threadModel->setName(tr("New thread"));
    threadModel->setColor(*(colorTable().begin()));
    m_stream->hideThread(threadModel->thread());
    
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

int StreamModel::accessTimeout() const
{
    int accessTimeout = m_settings.value("accessTimeout", DEFAULT_ACCESS_TIMEOUT).toInt();
    return accessTimeout;
}

bool StreamModel::delayActive() const
{
    bool delayActive = m_settings.value("delayActive", DEFAULT_DELAY_ACTIVE).toBool();
    return delayActive;
}

int StreamModel::delayDuration() const
{
    int delayDuration = m_settings.value("delayDuration", DEFAULT_DELAY).toInt();
    return delayDuration;
}

void StreamModel::doAddOperator(OperatorModel* op)
{
    Q_ASSERT(! op->isInitialized());
    
    m_stream->showOperator(op->op());
    m_operators.append(op);
    connect(op, SIGNAL(parameterErrorOccurred(ErrorData)), this, SLOT(handleParameterError(ErrorData)));
    
    emit operatorAdded(op);
}

void StreamModel::doRemoveOperator(OperatorModel* op)
{
    Q_ASSERT(! op->isInitialized());
    
    disconnect(op, SIGNAL(parameterErrorOccurred(ErrorData)));
    m_operators.removeAll(op);
    m_stream->hideOperator(op->op());
    
    emit operatorRemoved(op);
}

void StreamModel::doInitializeOperator(OperatorModel* op)
{
    if(op->isInitialized())
        return;
    
    op->beginChangeInitialized();
    try
    {
        m_stream->initializeOperator(op->op());
    }
    catch(stromx::runtime::OperatorError& e)
    {
        if(m_exceptionObserver)
            m_exceptionObserver->observe(stromx::runtime::ExceptionObserver::INITIALIZATION, e, 0);
    }
    op->endChangeInitialized();
}

void StreamModel::doDeinitializeOperator(OperatorModel* op)
{
    if(! op->isInitialized())
        return;
    
    op->beginChangeInitialized();
    try
    {
        m_stream->deinitializeOperator(op->op());
    }
    catch(stromx::runtime::OperatorError& e)
    {
        if(m_exceptionObserver)
            m_exceptionObserver->observe(stromx::runtime::ExceptionObserver::DEINITIALIZATION, e, 0);
    }
    op->endChangeInitialized();
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
    m_stream->showThread(threadModel->thread());
    m_threadListModel->addThread(threadModel);
    emit threadAdded(threadModel);
}

void StreamModel::doRemoveThread(ThreadModel* threadModel)
{
    m_stream->hideThread(threadModel->thread());
    m_threadListModel->removeThread(threadModel);
    emit threadRemoved(threadModel);
}

void StreamModel::doOverwriteSettings(const QMap< QString, QVariant >& settings)
{
    if (settings.keys().contains("delayActive"))
    {
        bool active = settings["delayActive"].toBool();
        if(active)
            m_stream->setDelay(delayDuration());
        else
            m_stream->setDelay(0);
        m_settings["delayActive"] = active;
        emit delayActiveChanged(active);
    }
    
    if (settings.keys().contains("delayDuration"))
    {
        int delay = settings["delayDuration"].toInt();
        
        if(delayActive())
            m_stream->setDelay(delay);
        m_settings["delayDuration"] = delay;
        emit delayDurationChanged(delay);
    }
    
    if (settings.keys().contains("accessTimeout"))
    {
        int timeout = settings["accessTimeout"].toInt();
        
        m_settings["accessTimeout"] = timeout;
        emit accessTimeoutChanged(timeout);
    }
}

void StreamModel::doSetSettings(const QMap< QString, QVariant >& settings)
{
    bool delayActiveWillChange = settings.value("delayActive", DEFAULT_DELAY_ACTIVE).toBool() != delayActive();
    bool delayDurationWillChange = settings.value("delayDuration", DEFAULT_DELAY).toInt() != delayDuration();
    bool accessTimeoutWillChange = settings.value("accessTimeout", DEFAULT_ACCESS_TIMEOUT).toInt() != accessTimeout();
    
    m_settings = settings;
    
    bool active = delayActive();
    if(active)
        m_stream->setDelay(delayDuration());
    else
        m_stream->setDelay(0);
    
    
    int delay = delayDuration();
    if(delayActive())
        m_stream->setDelay(delay);
    
    if (delayActiveWillChange)
        emit delayActiveChanged(delayActive());
    
    if (delayDurationWillChange)
        emit delayDurationChanged(delayDuration());
    
    if (accessTimeoutWillChange)
        emit accessTimeoutChanged(accessTimeout());
}

void StreamModel::write(stromx::runtime::FileOutput & output, const QString& basename) const
{
    try
    {
        stromx::runtime::XmlWriter writer;
        writer.writeStream(output, basename.toStdString(), *m_stream);
        
        QByteArray modelData;
        serializeModel(modelData);
        output.initialize(basename.toStdString());
        output.openFile("studio", stromx::runtime::OutputProvider::BINARY);
        output.file().write(modelData.data(), modelData.size());
    }
    catch(stromx::runtime::FileAccessFailed& e)
    {
        qWarning() << e.what();
        QString error = e.container().empty() 
                        ? tr("The file %1 could not be opened for writing.").arg(QString::fromStdString(e.filename()))
                        : tr("The file %1 in %2 could not be opened for writing.").arg(QString::fromStdString(e.filename()),
                                                                                      QString::fromStdString(e.container()));
        throw WriteStreamFailed(error);
    }
    catch(stromx::runtime::SerializationError & e)
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
    
    // clear the lists
    m_connections.clear();
    m_operators.clear();
    m_threadListModel->removeAllThreads();
    
    // delete the models
    foreach(ConnectionModel* connection, connections)
        delete connection;
    
    foreach(ThreadModel* thread, threads)
        delete thread;
    
    foreach(OperatorModel* op, operators)
        delete op;
    
    // delete the stream
    delete m_stream;
    m_stream = 0;
}

void StreamModel::allocateObjects(stromx::runtime::Stream* stream)
{
    // get the new data
    m_stream = stream;
    
    for(std::vector<stromx::runtime::Operator*>::const_iterator iter = m_stream->operators().begin();
        iter != m_stream->operators().end();
        ++iter)
    {
        OperatorModel* op = new OperatorModel(*iter, this);
        m_operators.append(op);
        
        connect(op, SIGNAL(operatorAccessTimedOut()), this, SIGNAL(accessTimedOut()));
        connect(op, SIGNAL(parameterErrorOccurred(ErrorData)), this, SLOT(handleParameterError(ErrorData)));
    }
    
    foreach(OperatorModel* opModel, m_operators)
    {
        if (! opModel->isInitialized())
            continue;
        
        stromx::runtime::Operator* op = opModel->op();
        
        for(std::vector<const stromx::runtime::Description*>::const_iterator inputIter = op->info().inputs().begin();
            inputIter != op->info().inputs().end();
            ++inputIter)
        {
            stromx::runtime::Output output = m_stream->connectionSource(op, (*inputIter)->id());
            
            if(output.valid())
            {
                OperatorModel* source = findOperatorModel(output.op());
                ConnectionModel* connection = new ConnectionModel(source, output.id(), opModel, (*inputIter)->id(), this);
                connection->connectToOperators();
                m_connections.append(connection);
            }
        }
    }
    
    for(std::vector<stromx::runtime::Thread*>::const_iterator iter = m_stream->threads().begin();
        iter != m_stream->threads().end();
        ++iter)
    {
        ThreadModel* thread = new ThreadModel(*iter, this);
        m_threadListModel->addThread(thread);
    }
    
    
    for(std::vector<stromx::runtime::Thread*>::const_iterator threadIter = m_stream->threads().begin();
        threadIter != m_stream->threads().end();
        ++threadIter)
    {
        ThreadModel* threadModel = findThreadModel(*threadIter);
        std::vector<stromx::runtime::Input>::const_iterator start = (*threadIter)->inputSequence().begin();
        std::vector<stromx::runtime::Input>::const_iterator end = (*threadIter)->inputSequence().end();
        for(std::vector<stromx::runtime::Input>::const_iterator inputIter = start; inputIter != end; ++inputIter)
        {
            ConnectionModel* connectionModel = findConnectionModel(*inputIter);
            if(connectionModel)
                connectionModel->setThread(threadModel);
        }
    }
}

OperatorModel* StreamModel::findOperatorModel(const stromx::runtime::Operator* op) const
{
    foreach(OperatorModel* opModel, m_operators)
    {
        if(opModel->op() == op)
            return opModel;
    }
    
    return 0;
}

ConnectionModel* StreamModel::findConnectionModel(const stromx::runtime::Input& input) const
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

ThreadModel* StreamModel::findThreadModel(const stromx::runtime::Thread* thread) const
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
    dataStream << quint32(STREAM_FORMAT_VERSION_MAJOR);
    dataStream << quint32(STREAM_FORMAT_VERSION_MINOR);
    dataStream << quint32(STREAM_FORMAT_VERSION_REVISION);
    
    dataStream.setVersion(QDataStream::Qt_4_7);
    
    // The uninitialized operators. They were instantiated by stromx-
    // studio in earlier versions but are part of the stromx stream now.
    dataStream << qint32(0);  // number of uninitialized operators
    
    // The model data (i.e. positions) of the operators. They were stored by
    // stromx-studio in earlier versions but are part of the stromx stream now.
    dataStream << qint32(0); // number of model data of initialized operators
    dataStream << qint32(0);  // number of model data of uninitialized operators
    
    // The model data (i.e. colors) of the threads.  They were stored by
    // stromx-studio in earlier versions but are part of the stromx stream now.
    dataStream << qint32(0);
    
    dataStream << m_observerModel;
    
    dataStream << m_settings;
}

void StreamModel::deserializeModel(const QByteArray& data)
{
    QDataStream dataStream(data);
    quint32 magicNumber = 0;
    qint32 count = 0;
    QList<OperatorModel*> xmlOperators = m_operators;
    QList<OperatorModel*> stromxStudioOperators;
    quint32 versionMajor = 0;
    quint32 versionMinor = 0;
    quint32 versionRevision = 0;
    
    dataStream >> magicNumber;
    if(magicNumber != MAGIC_NUMBER)
        throw ReadStudioDataFailed(tr("This is not a stromx-studio file"));
    
    dataStream >> versionMajor;
    dataStream >> versionMinor;
    dataStream >> versionRevision;
    stromx::runtime::Version streamFormatVersion(versionMajor, versionMinor, versionRevision);
    
    dataStream.setVersion(QDataStream::Qt_4_7);

    dataStream >> count;
    stromx::runtime::Factory* factory = m_operatorLibrary->factory();
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
        stromx::runtime::OperatorKernel* kernel = factory->newOperator(package.toStdString(), type.toStdString());
        stromx::runtime::Operator* op = m_stream->addOperator(kernel);
        opModel = new OperatorModel(op, this);
        m_operators.append(opModel);
        stromxStudioOperators.append(opModel);
    }
    
    // Deserialize the model data of the operator and thread models below. In 
    // files with file format v2 and higher this data is not stored anymore. 
    // I.e. do not throw an exception if the data is missing in this case.
    
    if (streamFormatVersion < STREAM_FORMAT_V2)
    {   
        dataStream >> count;
        if (count != xmlOperators.count())
            throw ReadStudioDataFailed(tr("Number of XML serialized operators does not match the stromx-studio data."));
        foreach(OperatorModel* op, xmlOperators)
            dataStream >>  op;
    
        dataStream >> count;
        if(streamFormatVersion < STREAM_FORMAT_V2 && count != stromxStudioOperators.count())
            throw ReadStudioDataFailed(tr("Number of stromx-studio serialized operators does not match stromx-studio data."));
        foreach(OperatorModel* op, stromxStudioOperators)
            dataStream >> op;
        
        dataStream >> count;
        if(streamFormatVersion < STREAM_FORMAT_V2 && count != m_threadListModel->threads().count())
            throw ReadStudioDataFailed(tr("Number of threads does not match stromx-studio data."));
        foreach(ThreadModel* thread, m_threadListModel->threads())
            dataStream >> thread;
    }
    else
    {
        dataStream >> count;
        if (count)
            throw ReadStudioDataFailed(tr("Number of XML serialized operators must be 0."));
        
        dataStream >> count;
        if (count)
            throw ReadStudioDataFailed(tr("Number of stromx-studio serialized operators must be 0."));
        
        dataStream >> count;
        if (count)
            throw ReadStudioDataFailed(tr("Number of threads must be 0."));
    }
    
    if (streamFormatVersion < STREAM_FORMAT_V2)
    {
        readVersion01(dataStream, m_observerModel);
    }
    else
    {
        dataStream >> m_observerModel;
    }
    
    QMap<QString, QVariant> settings;
    dataStream >> settings;
    doSetSettings(settings);
}

bool StreamModel::start()
{
    switch(m_stream->status())
    {
    case stromx::runtime::Stream::INACTIVE:
        {
            // sort inputs before starting
            stromx::runtime::SortInputsAlgorithm sort;
            sort.apply(*m_stream);
        }
        try
        {
            m_stream->start();
        }
        catch(stromx::runtime::OperatorError& e)
        {
            // report any errors to the error observer
            if(m_exceptionObserver)
                m_exceptionObserver->observe(stromx::runtime::ExceptionObserver::ACTIVATION, e, 0);
            
            // return with error
            return false;
        }
        
        break;
    case stromx::runtime::Stream::PAUSED:
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
    if(m_stream->status() == stromx::runtime::Stream::INACTIVE)
        return true;
    
    // stop the stream
    m_stream->stop();
    emit streamStopped();
    
    // start the thread which waits for the stream to finish
    QFuture<void> future = QtConcurrent::run(std::tr1::bind(&stromx::runtime::Stream::join, m_stream));
    m_joinStreamWatcher->setFuture(future);
    
    return true;
}

bool StreamModel::join()
{
    m_joinStreamWatcher->waitForFinished();
    return true;
}

bool StreamModel::isActive() const
{
    return m_stream->status() != stromx::runtime::Stream::INACTIVE;
}

void StreamModel::setDelayActive(bool active)
{
    if (active != delayActive())
    {
        QMap<QString, QVariant> settings;
        settings["delayActive"] = active;
        QUndoCommand* cmd = new SetStreamSettingsCmd(this, settings);
        m_undoStack->push(cmd);
    }
}

void StreamModel::setDelayDuration(int delay)
{
    int truncatedDelay = delay >= 0 ? delay : 0;
    
    if(truncatedDelay != delayDuration())
    {
        QMap<QString, QVariant> settings;
        settings["delayDuration"] = truncatedDelay;
        QUndoCommand* cmd = new SetStreamSettingsCmd(this, settings);
        m_undoStack->push(cmd);
    }
}

void StreamModel::setAccessTimeout(int timeout)
{
    int truncatedTimeout = timeout >= 0 ? timeout : 0;

    if(truncatedTimeout != accessTimeout())
    {
        QMap<QString, QVariant> settings;
        settings["accessTimeout"] = truncatedTimeout;
        QUndoCommand* cmd = new SetStreamSettingsCmd(this, settings);
        m_undoStack->push(cmd);
    }
}

void StreamModel::setExceptionObserver(ExceptionObserver* observer)
{
    Q_ASSERT(observer);
    
    if(m_exceptionObserver)
        m_stream->removeObserver(m_exceptionObserver);
    
    m_stream->addObserver(observer);
    m_exceptionObserver = observer;
}

void StreamModel::handleParameterError(const ErrorData& data)
{
    if(m_exceptionObserver)
    {
        m_exceptionObserver->sendErrorData(data);
    }
}

void StreamModel::createTemplate()
{
    // add one thread
    stromx::runtime::Thread* thread = m_stream->addThread();
    m_stream->hideThread(thread);
    ThreadModel* threadModel = new ThreadModel(thread, this);
    threadModel->setName(tr("New thread"));
    threadModel->setColor(*(colorTable().begin()));
    doAddThread(threadModel);
    
    // add one observer
    m_observerModel->insertRow(m_observerModel->rowCount());
}






