#include "GetParameterTask.h"

#include <stromx/core/Operator.h>
#include <stromx/core/OperatorException.h>

using namespace stromx::core;

GetParameterTask::GetParameterTask(const stromx::core::Operator* op, unsigned int id,
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
    catch(stromx::core::Timeout &)
    {
        m_errorCode = TIMED_OUT;
    }
    catch(stromx::core::OperatorError& e)
    {
        m_errorCode = EXCEPTION;
        m_errorData = ErrorData(e, ErrorData::PARAMETER_ACCESS);
    }
    catch(stromx::core::Exception& e)
    {
        Q_ASSERT(false);
    }
}
