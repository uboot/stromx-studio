#include "ObserverTreeModel.h"

#include <QStringList>
#include "InputData.h"
#include "ObserverModel.h"

ObserverTreeModel::ObserverTreeModel(QUndoStack* undoStack, QObject * parent)
  : QAbstractItemModel(parent),
    m_undoStack(undoStack)
{
}

int ObserverTreeModel::rowCount(const QModelIndex& parent) const
{
    if(parent.isValid())
        return 0;
    
    return m_observers.count();
}

int ObserverTreeModel::columnCount(const QModelIndex& parent) const
{
    return 1;
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
        return tr("Input");
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
        m_observers.insert(row + i, new ObserverModel(m_undoStack, this));
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
    if(! parent.isValid())
        return createIndex(row, column, m_observers[row]);
    
    return QModelIndex();
}

QModelIndex ObserverTreeModel::parent(const QModelIndex& child) const
{
    return QModelIndex();
}

QVariant ObserverTreeModel::data(const QModelIndex& index, int role) const
{
    if(role != Qt::DisplayRole)
        return QVariant();
    
    if(index.parent().isValid())
        return QVariant();
    
    if(index.column() != 0)
        return QVariant();
    
    ObserverModel* observerList = reinterpret_cast<ObserverModel*>(index.internalPointer());
    return observerList->name();
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
        
        return true;
    }
    
    return false;
}

QStringList ObserverTreeModel::mimeTypes() const
{
    return QStringList("stromx/input");
}


