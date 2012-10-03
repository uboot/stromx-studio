#include "ObtainReadAccessTask.h"

const unsigned int ObtainReadAccessTask::TIMEOUT = 5000;

using namespace stromx::core;

ObtainReadAccessTask::ObtainReadAccessTask(OperatorModel::ConnectorType type, unsigned int id,
                                           const DataContainer& dataContainer, QObject* parent)
  : Task(parent),
    m_type(type),
    m_id(id),
    m_data(dataContainer)
{
}

void ObtainReadAccessTask::run()
{
    try
    {
        m_access = ReadAccess<>(m_data, TIMEOUT);
    }
    catch(Timeout &)
    {
    }
}

