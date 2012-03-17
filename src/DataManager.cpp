#include "DataManager.h"

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
        addInput(input);
}


void DataManager::addInputLayer(InputModel* input, int pos)
{
    addInput(input);
}

void DataManager::moveInputLayer(InputModel* input, int srcPos, int destPos)
{
    
}

void DataManager::removeInputLayer(InputModel* input, int pos)
{
    removeInput(input);
}

void DataManager::updateLayerData(OperatorModel::ConnectorType type, unsigned int id, stromx::core::DataContainer data)
{

}

void DataManager::addInput(InputModel* input)
{
    connect(input->op(), SIGNAL(connectorDataChanged(OperatorModel::ConnectorType,uint,stromx::core::DataContainer)),
            this, SLOT(updateLayerData(OperatorModel::ConnectorType,uint,stromx::core::DataContainer)));
    m_currentInputs.insert(input);
}

void DataManager::removeInput(InputModel* input)
{
    input->op()->disconnect(this);
    m_currentInputs.remove(input);
}

