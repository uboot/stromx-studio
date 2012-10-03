#include "GetParameterTask.h"

#include <stromx/core/Operator.h>
#include <stromx/core/OperatorException.h>

const unsigned int GetParameterTask::TIMEOUT = 1000;

using namespace stromx::core;

GetParameterTask::GetParameterTask(const stromx::core::Operator* op, unsigned int id, QObject* parent)
  : Task(parent),
    m_op(op),
    m_id(id),
    m_errorCode(NO_ERROR)
{
}

void GetParameterTask::run()
{
    try
    {
        m_result = m_op->getParameter(m_id, TIMEOUT);
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
