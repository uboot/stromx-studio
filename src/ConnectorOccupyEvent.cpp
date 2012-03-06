#include "ConnectorOccupyEvent.h"

ConnectorOccupyEvent::ConnectorOccupyEvent(OperatorModel::ConnectorType type, unsigned int id, bool isOccupied)
  : QEvent(QEvent::User),
    m_type(type),
    m_id(id)
{

}
