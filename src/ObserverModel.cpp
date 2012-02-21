#include "ObserverModel.h"

#include "InputModel.h"

ObserverModel::ObserverModel(QObject * parent)
  : QObject(parent)
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
    InputModel* input = m_inputs[position];
    m_inputs.removeAt(position);
    delete input;
}

const InputModel* ObserverModel::input(int position)
{
    return m_inputs[position];
}

