#include "ConnectorDataEvent.h"

ConnectorDataEvent::ConnectorDataEvent(OperatorModel::ConnectorType type, unsigned int id,
                                       const stromx::core::ReadAccess<> & access)
  : QEvent(Type(TYPE)),
    m_type(type),
    m_id(id),
    m_access(access)
{
}
