#include "ObserverModel.h"

#include "InputModel.h"
#include "ObserverTreeModel.h"
#include "RenameObserverCmd.h"

ObserverModel::ObserverModel(QUndoStack* undoStack, ObserverTreeModel* parent)
  : QSortFilterProxyModel(parent),
    m_undoStack(undoStack),
    m_parent(parent)
{
    m_name = "New observer";
    setSourceModel(parent);
}

void ObserverModel::setName(const QString& name)
{
    if(name != m_name)
    {
        QUndoCommand* cmd = new RenameObserverCmd(this, name);
        m_undoStack->push(cmd);
    }
}

void ObserverModel::doSetName(const QString& name)
{
    m_name = name;
    emit nameChanged(name);
    emit changed(this);
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

