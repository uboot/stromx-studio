#include "ConnectorObserver.h"

#include <stromx/core/Connector.h>
#include <stromx/core/DataContainer.h>
#include <QCoreApplication>
#include "ConnectorOccupyEvent.h"

ConnectorObserver::ConnectorObserver(OperatorModel* opModel)
  : m_opModel(opModel),
    m_application(QCoreApplication::instance())
{
}

void ConnectorObserver::observe(const stromx::core::Connector& connector, const stromx::core::DataContainer& data) const
{
    OperatorModel::ConnectorType type = connector.type() == stromx::core::Connector::INPUT ? 
                                        OperatorModel::INPUT : OperatorModel::OUTPUT;
    QEvent* event = new ConnectorOccupyEvent(type, connector.id(), ! data.empty());                                   
    m_application->postEvent(m_opModel, event);
}
