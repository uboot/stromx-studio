#include "SetParameterTask.h"

#include <stromx/runtime/Operator.h>
#include <stromx/runtime/OperatorException.h>

using namespace stromx::runtime;

SetParameterTask::SetParameterTask(stromx::runtime::Operator* op, unsigned int id,
                                   const stromx::runtime::DataRef & value, int timeout, QObject* parent)
  : Task(parent),
    m_op(op),
    m_id(id),
    m_data(value),
    m_errorCode(NO_ERROR),
    m_timeout(timeout)
{
}

void SetParameterTask::run()
{
    try
    {
        m_op->setParameter(m_id, m_data, m_timeout);
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
