#include "ObtainReadAccessTask.h"

using namespace stromx::core;

ObtainReadAccessTask::ObtainReadAccessTask(OperatorModel::ConnectorType type, unsigned int id,
                                           const DataContainer& dataContainer, int timeout, QObject* parent)
  : Task(parent),
    m_type(type),
    m_id(id),
    m_data(dataContainer),
    m_timeout(timeout)
{
}

void ObtainReadAccessTask::run()
{
    try
    {
        m_access = ReadAccess<>(m_data, m_timeout);
    }
    catch(Timeout &)
    {
    }
}

