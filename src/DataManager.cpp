#include "DataManager.h"

#include <stromx/core/ReadAccess.h>

#include "AbstractDataVisualizer.h"
#include "InputModel.h"
#include "ObserverModel.h"
#include "OperatorModel.h"

DataManager::DataManager(ObserverModel* observer, AbstractDataVisualizer* visualizer, QObject* parent)
  : QObject(parent),
    m_observer(observer),
    m_visualizer(visualizer)
{
    connect(m_observer, SIGNAL(inputAdded(InputModel*,int)), this, SLOT(addInputLayer(InputModel*,int)));
    connect(m_observer, SIGNAL(inputMoved(InputModel*,int,int)), this, SLOT(moveInputLayer(InputModel*,int,int)));
    connect(m_observer, SIGNAL(inputRemoved(InputModel*,int)), this, SLOT(removeInputLayer(InputModel*,int)));
    
    int i = 0;
    foreach(InputModel* input, observer->inputs())
    {
        m_inputs.append(input);
        connectInput(input);
        visualizer->addLayer(i);
        i++;
    }
}

void DataManager::addInputLayer(InputModel* input, int pos)
{
    connectInput(input);
    m_inputs.insert(pos, input);
    m_visualizer->addLayer(pos);
}

void DataManager::moveInputLayer(InputModel* input, int srcPos, int destPos)
{
    m_inputs.removeAt(srcPos);
    m_inputs.insert(destPos, input);
    m_visualizer->moveLayer(srcPos, destPos);
}

void DataManager::removeInputLayer(InputModel* input, int pos)
{
    m_visualizer->removeLayer(pos);
    m_inputs.removeAt(pos);
    disconnectInput(input);
}

void DataManager::updateLayerData(OperatorModel::ConnectorType type, unsigned int id, stromx::core::DataContainer data)
{
    if(data.empty())
        return;
    
    QObject* source = sender();
    if(OperatorModel* op = qobject_cast<OperatorModel*>(source))
    {
        stromx::core::ReadAccess<> access(data);
        
        for(int layer = 0; layer < m_inputs.count(); ++layer)
        {
            InputModel* input = m_inputs[layer];
            if(input->op() == op && input->id() == id)
                m_visualizer->setData(layer, access());
        }
    }
}

void DataManager::connectInput(InputModel* input)
{
    // connect only if not connection to this operator exists
    connect(input->op(), SIGNAL(connectorDataChanged(OperatorModel::ConnectorType,uint,stromx::core::DataContainer)),
            this, SLOT(updateLayerData(OperatorModel::ConnectorType,uint,stromx::core::DataContainer)), Qt::UniqueConnection);
}

void DataManager::disconnectInput(InputModel* input)
{
    OperatorModel* op = input->op();
    
    // look if any other input requires a connection to op
    // in this case the connections must not be disconnected
    foreach(InputModel* input, m_inputs)
    {
        if(input->op() == op)
            return;
    }
    
    // this was the only input connected to op
    input->op()->disconnect(this);
}

