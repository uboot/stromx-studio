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
    
    beginInsertRows(QModelIndex(), row, row + count - 1);
    for(int i = 0; i < count; ++i)
        m_observers.insert(row + i, new ObserverModel(this));
    endInsertRows();
    
    return true;
}

bool ObserverTreeModel::removeRows(int row, int count, const QModelIndex & parent)
{
    if(parent.isValid())
        return false;
    
    beginRemoveRows(QModelIndex(), row, row + count - 1);
    for(int i = 0; i < count; ++i)
        m_observers.removeAt(row);
    endRemoveRows();
    
    return true;
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
    
    // only parents are editable
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
        ObserverModel* observer = m_observers[parent.row()];
        beginInsertRows(parent, row, row);
        InputModel* input = new InputModel(inputData->op(), inputData->id(), m_undoStack, this);
        observer->insertInput(row, input);
        endInsertRows();
        
        return true;
    }
    
    return false;
}

QStringList ObserverTreeModel::mimeTypes() const
{
    return QStringList("stromx/input");
}


