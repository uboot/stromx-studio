#include "ObserverModel.h"

#include "InputModel.h"

ObserverModel::ObserverModel(QUndoStack* undoStack, QObject * parent)
  : QObject(parent),
    m_undoStack(undoStack)
{
    m_name = "New observer";
}

void ObserverModel::setName(const QString& name)
{
    m_name = name;
}

void ObserverModel::insertInput(int position, OperatorModel* op, unsigned int id)
{
    InputModel* input = new InputModel(op, id, m_undoStack, this);
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

