#include "DataManager.h"

#include <stromx/runtime/ReadAccess.h>

#include "AbstractDataVisualizer.h"
#include "model/InputModel.h"
#include "model/ObserverModel.h"
#include "model/OperatorModel.h"

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
    // move all layers behind pos to the back
    for(int i = m_inputs.count(); i > pos; --i)
        m_visualizer->moveLayer(i - 1, i);
    
    // add a new layer in the front
    m_visualizer->addLayer(pos);
    
    // remember the input and connect to it
    m_inputs.insert(pos, input);
    connectInput(input);
}

void DataManager::moveInputLayer(InputModel* /*input*/, int srcPos, int destPos)
{
    if(srcPos == destPos)
        return;
    
    // temporarily move the source behind all layers
    m_visualizer->moveLayer(srcPos, m_inputs.count());
    
    // move the layers between source and destination
    if(srcPos < destPos)
    {
        for(int i = srcPos; i < destPos; ++i)
            m_visualizer->moveLayer(i + 1, i);
    }
    else
    {
        for(int i = srcPos; i > destPos; --i)
            m_visualizer->moveLayer(i - 1, i);
    }
    
    // move the source back to the destination and remove the temporary layer
    m_visualizer->moveLayer(m_inputs.count(), destPos);
    m_visualizer->removeLayer(m_inputs.count());
    
    // update the input list
    if(srcPos < destPos)
    {
        m_inputs.insert(destPos + 1, m_inputs[srcPos]);
        m_inputs.removeAt(srcPos);
    }
    else
    {
        m_inputs.insert(destPos, m_inputs[srcPos]);
        m_inputs.removeAt(srcPos + 1);
    }
}

void DataManager::removeInputLayer(InputModel* input, int pos)
{
    // remove the input and disconnect from it
    m_inputs.removeAt(pos);
    disconnectInput(input);
    
    // move all layer behind pos to the front
    for(int i = pos; i < m_inputs.count(); ++i)
        m_visualizer->moveLayer(pos + 1, pos);
    
    // remove the last layer
    m_visualizer->removeLayer(m_inputs.count());
}

void DataManager::updateLayerData(OperatorModel::ConnectorType /*type*/, unsigned int id,
                                  stromx::runtime::ReadAccess<> access)
{
    if(access.empty())
        return;
    
    QObject* source = sender();
    if(OperatorModel* op = qobject_cast<OperatorModel*>(source))
    {
        for(int layer = 0; layer < m_inputs.count(); ++layer)
        {
            InputModel* input = m_inputs[layer];
            if(input->op() == op && input->id() == id)
                m_visualizer->setData(layer, access(), input->visualizationState());
        }
    }
}

void DataManager::connectInput(InputModel* input)
{
    // connect only if no connection to this operator exists
    connect(input->op(), SIGNAL(connectorDataChanged(OperatorModel::ConnectorType,uint,stromx::runtime::ReadAccess<>)),
            this, SLOT(updateLayerData(OperatorModel::ConnectorType,uint,stromx::runtime::ReadAccess<>)), Qt::UniqueConnection);
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

