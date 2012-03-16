#include "ConnectorObserver.h"

#include <stromx/core/Connector.h>
#include <stromx/core/DataContainer.h>
#include <QCoreApplication>
#include "ConnectorDataEvent.h"
#include "ConnectorOccupyEvent.h"

ConnectorObserver::ConnectorObserver(OperatorModel* opModel)
  : m_opModel(opModel),
    m_application(QCoreApplication::instance()),
    m_observeData(false)
{
}

void ConnectorObserver::observe(const stromx::core::Connector& connector, const stromx::core::DataContainer& data) const
{
    OperatorModel::ConnectorType type = connector.type() == stromx::core::Connector::INPUT ? 
                                        OperatorModel::INPUT : OperatorModel::OUTPUT;
    ConnectorOccupyEvent* occupyEvent = new ConnectorOccupyEvent(type, connector.id(), data.empty() ? false : true);                                   
    m_application->postEvent(m_opModel, occupyEvent);
    
    // send data event only if receivers are connected to the respective signal of OperatorModel
    QMutexLocker lock(&m_mutex);
    if(m_observeData)
    {
        ConnectorDataEvent* dataEvent = new ConnectorDataEvent(type, connector.id(), data);
        m_application->postEvent(m_opModel, dataEvent);
    }
    
    // uncomment for slow motion:
    // usleep(100000);
}

void ConnectorObserver::setObserveData(bool observe)
{
    QMutexLocker lock(&m_mutex);
    m_observeData = observe;
}

