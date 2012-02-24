#include "ObserverModel.h"

#include "InputModel.h"
#include "ObserverTreeModel.h"

ObserverModel::ObserverModel(ObserverTreeModel * parent)
  : QObject(parent),
    m_observerTreeModel(parent)
{
    m_name = "New observer";
}

void ObserverModel::setName(const QString& name)
{
    m_name = name;
}

void ObserverModel::insertInput(int position, InputModel* input)
{
    m_inputs.insert(position, input);
}

void ObserverModel::removeInput(int position)
{
    m_inputs.removeAt(position);
}

InputModel* ObserverModel::input(int position)
{
    return m_inputs[position];
}

QDataStream& operator<<(QDataStream& stream, const ObserverModel* model)
{
    stream << qint32(model->m_inputs.count());
    foreach(InputModel* model, model->m_inputs)
        stream << model;
    
    return stream;
}

QDataStream& operator>>(QDataStream& stream, ObserverModel* model)
{
    qint32 count = 0;
    
    stream >> count;
    for(int i = 0; i < count; ++i)
    {
        InputModel* input = new InputModel(model->m_observerTreeModel->undoStack(), model->m_observerTreeModel);
        stream >> input;
        model->m_inputs.append(input);
    }
    
    return stream;
}
