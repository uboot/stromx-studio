#include "ObserverModel.h"

#include "InputModel.h"
#include "ObserverTreeModel.h"
#include "RenameObserverCmd.h"

ObserverModel::ObserverModel(QUndoStack* undoStack, ObserverTreeModel* parent)
  : QAbstractProxyModel(parent),
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

QModelIndex ObserverModel::mapFromSource(const QModelIndex& sourceIndex) const
{
    if(! sourceIndex.isValid())
        return QModelIndex();
    
    // filter observer lists
    if(! sourceIndex.parent().isValid())
        return QModelIndex();
    
    ObserverModel* observer = reinterpret_cast<ObserverModel*>(sourceIndex.internalPointer());
    
    // filter input if its parent is not this observer
    if(observer != this)
        return QModelIndex();
    
    return createIndex(sourceIndex.row(), sourceIndex.column(), sourceIndex.internalPointer());
}

QModelIndex ObserverModel::mapToSource(const QModelIndex& proxyIndex) const
{
    if(! proxyIndex.isValid())
        return createIndex(observerPos(), proxyIndex.column());
    
    return createIndex(proxyIndex.row(), proxyIndex.column(), const_cast<ObserverModel*>(this));
}

int ObserverModel::rowCount(const QModelIndex& parent) const
{
    revert();
    return numInputs();
}

QModelIndex ObserverModel::parent(const QModelIndex& child) const
{
    return m_parent->parent(child);
}

int ObserverModel::observerPos() const
{
    return m_parent->observers().indexOf(const_cast<ObserverModel*>(this));
}

QModelIndex ObserverModel::index(int row, int column, const QModelIndex& parent) const
{
    return createIndex(row, column, const_cast<ObserverModel*>(this));
}

int ObserverModel::columnCount(const QModelIndex& parent) const
{
    return 2;
}









