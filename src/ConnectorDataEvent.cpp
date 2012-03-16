#include "ConnectorDataEvent.h"

ConnectorDataEvent::ConnectorDataEvent(OperatorModel::ConnectorType type, unsigned int id, const stromx::core::DataContainer& data)
  : QEvent(Type(TYPE)),
    m_type(type),
    m_id(id),
    m_data(data)
{
}
