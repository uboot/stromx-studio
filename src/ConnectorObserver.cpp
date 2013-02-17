#include "ConnectorObserver.h"

#include <stromx/runtime/Connector.h>
#include <stromx/runtime/DataContainer.h>
#include <QCoreApplication>
#include "event/ConnectorDataEvent.h"
#include "event/ConnectorOccupyEvent.h"

ConnectorObserver::ConnectorObserver(QObject* receiver)
  : m_receiver(receiver),
    m_observeData(false)
{
}

void ConnectorObserver::observe(const stromx::runtime::Connector& connector, const stromx::runtime::DataContainer& data) const
{
    QCoreApplication* application = QCoreApplication::instance();
    OperatorModel::ConnectorType type = connector.type() == stromx::runtime::Connector::INPUT ? 
                                        OperatorModel::INPUT : OperatorModel::OUTPUT;
    ConnectorOccupyEvent* occupyEvent = new ConnectorOccupyEvent(type, connector.id(), data.empty() ? false : true);                                   
    application->postEvent(m_receiver, occupyEvent);
    
    // Next the actual data is observed:
    // First make sure the data is not empty.
    if(data.empty())
        return;
    
    // If receivers are connected to the respective signal of OperatorModel
    // the member m_observeData is true. Here we obtain the flag in a thread-safe
    // way.
    bool observeData = false;
    {
        QMutexLocker lock(&m_mutex);
        observeData = m_observeData;
    }
    
    // The data must be observed only if the the flag is true and the connector
    // is an input (observation of outputs is not supported because it can always
    // be achieved by observing the corresponding input).
    if(observeData && type == OperatorModel::INPUT)
    {
        // get a read access to the data (this might take a while)
        stromx::runtime::ReadAccess<> access(data);
        
        // send an event with the data and the access to the Qt GUI loop
        ConnectorDataEvent* dataEvent = new ConnectorDataEvent(type, connector.id(), access);
        application->postEvent(m_receiver, dataEvent);
    }
}

void ConnectorObserver::setObserveData(bool observe)
{
    QMutexLocker lock(&m_mutex);
    m_observeData = observe;
}


