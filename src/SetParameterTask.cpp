#include "SetParameterTask.h"

#include <stromx/core/Operator.h>
#include <stromx/core/OperatorException.h>

const unsigned int SetParameterTask::TIMEOUT = 5000;

using namespace stromx::core;


SetParameterTask::SetParameterTask(stromx::core::Operator* op, unsigned int id,
                                   const stromx::core::DataRef & value, QObject* parent)
  : Task(parent),
    m_op(op),
    m_id(id),
    m_data(value),
    m_errorCode(NO_ERROR)
{
}

void SetParameterTask::run()
{
    try
    {
        m_op->setParameter(m_id, m_data, TIMEOUT);
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
