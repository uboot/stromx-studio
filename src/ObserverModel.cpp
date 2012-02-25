#include "ObserverModel.h"

#include "InputModel.h"
#include "ObserverTreeModel.h"

ObserverModel::ObserverModel(QObject * parent)
  : QObject(parent)
{
    m_name = "New observer";
}

void ObserverModel::setName(const QString& name)
{
    m_name = name;
    emit nameChanged(name);
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

