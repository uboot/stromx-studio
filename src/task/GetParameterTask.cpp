#include "task/GetParameterTask.h"

#include <stromx/runtime/Operator.h>
#include <stromx/runtime/OperatorException.h>

using namespace stromx::runtime;

GetParameterTask::GetParameterTask(const stromx::runtime::Operator* op, unsigned int id,
                                   int timeout, QObject* parent)
  : Task(parent),
    m_op(op),
    m_id(id),
    m_errorCode(NO_ERROR),
    m_timeout(timeout)
{
}

void GetParameterTask::run()
{
    try
    {
        m_result = m_op->getParameter(m_id, m_timeout);
    }
    catch(stromx::runtime::Timeout &)
    {
        m_errorCode = TIMED_OUT;
    }
    catch(stromx::runtime::OperatorError& e)
    {
        m_errorCode = EXCEPTION;
        m_errorData = ErrorData(e, ErrorData::PARAMETER_ACCESS);
    }
    catch(stromx::runtime::Exception&)
    {
        Q_ASSERT(false);
    }
}
