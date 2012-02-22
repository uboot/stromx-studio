#include "ObserverTreeModel.h"

#include <QStringList>
#include "InputData.h"
#include "InputModel.h"
#include "ObserverModel.h"
#include "OperatorModel.h"

ObserverTreeModel::ObserverTreeModel(QUndoStack* undoStack, QObject * parent)
  : QAbstractItemModel(parent),
    m_undoStack(undoStack)
{
    setSupportedDragActions(Qt::MoveAction);
}

int ObserverTreeModel::rowCount(const QModelIndex& parent) const
{
    // parent is root
    if(! parent.isValid())
        return m_observers.count();
    
    // parent is an observer
    if(! parent.internalPointer())
        return m_observers[parent.row()]->numInputs();
    
    // parent is an input
    return 0;
}

int ObserverTreeModel::columnCount(const QModelIndex& parent) const
{
    return 2;
}

QVariant ObserverTreeModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if(role != Qt::DisplayRole)
        return QVariant();
        
    if(orientation != Qt::Horizontal)
        return QVariant();
    
    switch(section)
    {
    case 0:
        return tr("Operator");
    case 1:
        return tr("Id");
    default:
        ;
    }
    
    return QVariant();
}

bool ObserverTreeModel::insertRows(int row, int count, const QModelIndex & parent)
{
    if(parent.isValid())
        return false;
    
    Q_ASSERT(count == 1);
    
    doInsertObserver(row, new ObserverModel(this));
    
    return true;
}

void ObserverTreeModel::doInsertObserver(int pos, ObserverModel* observer)
{
    beginInsertRows(QModelIndex(), pos, pos);
    m_observers.insert(pos, observer);
    endInsertRows();
}

bool ObserverTreeModel::removeRows(int row, int count, const QModelIndex & parent)
{
    Q_ASSERT(count == 1);
    
    if(parent.isValid())
        doRemoveInput(parent.row(), row);
    else
        doRemoveObserver(row);
    
    return true;
}

void ObserverTreeModel::doRemoveObserver(int pos)
{
    beginRemoveRows(QModelIndex(), pos, pos);
    m_observers.removeAt(pos);
    endRemoveRows();
}

void ObserverTreeModel::doRemoveInput(int observerPos, int inputPos)
{
    beginRemoveRows(createIndex(observerPos, 0), inputPos, inputPos);
    m_observers[observerPos]->removeInput(inputPos);
    endRemoveRows();
}

QModelIndex ObserverTreeModel::index(int row, int column, const QModelIndex& parent) const
{
    // this is an observer list
    if(! parent.isValid())
        return createIndex(row, column);
    
    // this is an input
    return createIndex(row, column, m_observers[parent.row()]);
}

QModelIndex ObserverTreeModel::parent(const QModelIndex& child) const
{    
    // child is an observer
    if(! child.internalPointer())
        return QModelIndex();
    
    // child is an input
    ObserverModel* observer = reinterpret_cast<ObserverModel*>(child.internalPointer());
    int observerId = m_observers.indexOf(observer);
    return createIndex(observerId, 0);
}

QVariant ObserverTreeModel::data(const QModelIndex& index, int role) const
{
    if(role != Qt::DisplayRole)
        return QVariant();
    
    // this is an observer
    if(! index.parent().isValid())
    {
        if(index.column() == 0)
            return m_observers[index.row()]->name();
        else 
            return QVariant();
    }
    
    // this is an input
    ObserverModel* observer = reinterpret_cast<ObserverModel*>(index.internalPointer());
    const InputModel* input = observer->input(index.row());
    if(index.column() == 0)
        return input->op()->name();
    else
        return input->id();
}

bool ObserverTreeModel::setData(const QModelIndex& index, const QVariant& value, int role)
{
    if(index.isValid() && ! index.parent().isValid() && index.column() == 0)
    {
        QString newName = value.toString();
        
        if(newName.isEmpty())
            return false;
        
        m_observers[index.row()]->setName(newName);
        emit dataChanged(index, index);
    }
    
    return false;
}

Qt::ItemFlags ObserverTreeModel::flags(const QModelIndex& index) const
{
    Qt::ItemFlags flags = QAbstractItemModel::flags(index);
    
    // top level can not accept drags
    if(! index.isValid())
        return flags;
    
    // only parents are editablenew ObserverModel(this)
    if(! index.parent().isValid())
        return flags |= Qt::ItemIsEditable | Qt::ItemIsDropEnabled;
        
    return flags |= Qt::ItemIsDragEnabled;
}

bool ObserverTreeModel::dropMimeData(const QMimeData *data,
     Qt::DropAction action, int row, int column, const QModelIndex &parent)
{
    if(action == Qt::IgnoreAction)
        return true;
    
    if(const InputData* inputData = qobject_cast<const InputData*>(data))
    {
        InputModel* input = inputData->input();
        if(! input)
            input = new InputModel(inputData->op(), inputData->id(), m_undoStack, this);
        doInsertInput(parent.row(), row, input);
        
        return true;
    }
    
    return false;
}

void ObserverTreeModel::doInsertInput(int observerPos, int inputPos, InputModel* input)
{
    beginInsertRows(createIndex(observerPos, 0), inputPos, inputPos);
    m_observers[observerPos]->insertInput(inputPos, input);
    endInsertRows();
}

QStringList ObserverTreeModel::mimeTypes() const
{
    return QStringList("stromx/input");
}

QMimeData* ObserverTreeModel::mimeData(const QModelIndexList& indexes) const
{
    if(indexes.empty())
        return 0;
    
    const QModelIndex & index = indexes[0];
    
    // this is an observer
    if(! index.parent().isValid())
        return 0;
    
    InputModel* input = m_observers[index.parent().row()]->input(index.row());
    return new InputData(input);
}

Qt::DropActions ObserverTreeModel::supportedDropActions() const
{
    return Qt::MoveAction | Qt::CopyAction;
}




