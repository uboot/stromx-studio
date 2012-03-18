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
    
    foreach(InputModel* input, observer->inputs())
    {
        m_inputs.append(input);
        connectInput(input);
    }
}


void DataManager::addInputLayer(InputModel* input, int pos)
{
    connectInput(input);
    m_inputs.insert(pos, input);
}

void DataManager::moveInputLayer(InputModel* input, int srcPos, int destPos)
{
    m_inputs.removeAt(srcPos);
    m_inputs.insert(destPos, input);
}

void DataManager::removeInputLayer(InputModel* input, int pos)
{
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
        
        for(int i = 0; i < m_inputs.count(); ++i)
        {
            if(m_inputs[i]->op() == op)
                m_visualizer->setData(i, access());
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

