#include "GetParameterTask.h"

#include <QFutureWatcher>
#include <QtConcurrentRun>
#include <stromx/core/Operator.h>
#include <stromx/core/OperatorException.h>

#ifdef __GNUG__
    #include <tr1/functional>
#else
    #include <functional>
#endif

const unsigned int GetParameterTask::TIMEOUT = 100;

using namespace stromx::core;

GetParameterTask::GetParameterTask(const stromx::core::Operator* op, unsigned int id, QObject* parent)
  : QObject(parent),
    m_op(op),
    m_id(id),
    m_errorCode(NO_ERROR),
    m_watcher(0)
{
    connect(m_watcher, SIGNAL(finished()), this, SLOT(handleFutureFinished()));
    m_watcher->setFuture(QtConcurrent::run(std::tr1::bind(&runTask, this)));
}

GetParameterTask::~GetParameterTask()
{
    m_watcher->waitForFinished();
}

void GetParameterTask::handleFutureFinished()
{
    emit finished();
    delete this;
}

void GetParameterTask::runTask(GetParameterTask* task)
{
    try
    {
        task->m_result = task->m_op->getParameter(task->m_id, TIMEOUT);
    }
    catch(stromx::core::Timeout &)
    {
        task->m_errorCode = TIMED_OUT;
    }
    catch(stromx::core::OperatorError& e)
    {
        task->m_errorCode = EXCEPTION;
        task->m_errorData = ErrorData(e, ErrorData::PARAMETER_ACCESS);
    }
    catch(stromx::core::Exception& e)
    {
        Q_ASSERT(false);
    }
}
