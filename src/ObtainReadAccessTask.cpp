#include "ObtainReadAccessTask.h"

#include <QFutureWatcher>
#include <QtConcurrentRun>

#ifdef __GNUG__
    #include <tr1/functional>
#else
    #include <functional>
#endif

const unsigned int ObtainReadAccessTask::TIMEOUT = 100;

using namespace stromx::core;

ObtainReadAccessTask::ObtainReadAccessTask(OperatorModel::ConnectorType type, unsigned int id,
                                           const DataContainer& dataContainer, QObject* parent)
  : QObject(parent),
    m_type(type),
    m_id(id),
    m_watcher(new QFutureWatcher< ReadAccess<> >(this))
{
    connect(m_watcher, SIGNAL(finished()), this, SLOT(handleFutureFinished()));
    m_watcher->setFuture(QtConcurrent::run(std::tr1::bind(&getReadAccess, dataContainer)));
}

ObtainReadAccessTask::~ObtainReadAccessTask()
{
    m_watcher->waitForFinished();
}

void ObtainReadAccessTask::handleFutureFinished()
{
    m_access = m_watcher->result();
    
    emit finished();
    delete this;
}

ReadAccess<> ObtainReadAccessTask::getReadAccess(const DataContainer& container)
{
    try
    {
        return ReadAccess<>(container, TIMEOUT);
    }
    catch(Timeout &)
    {
        return ReadAccess<>();
    }
}
