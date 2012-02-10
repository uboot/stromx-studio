#include "ObserverTreeModel.h"

int ObserverTreeModel::rowCount(const QModelIndex& parent) const
{
    return m_lists.count();
}

int ObserverTreeModel::columnCount(const QModelIndex& parent) const
{
    return 3;
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
        return tr("Name");
    case 1:
        return tr("Input");
    case 2:
        return tr("Color");
    default:
        ;
    }
    
    return QVariant();
}

QModelIndex ObserverTreeModel::index(int row, int column, const QModelIndex& parent) const
{
    return QModelIndex();
}

QModelIndex ObserverTreeModel::parent(const QModelIndex& child) const
{
    return QModelIndex();
}

QVariant ObserverTreeModel::data(const QModelIndex& index, int role) const
{
    return QVariant();
}

void ObserverTreeModel::addObserverList(ObserverList* list)
{

}

void ObserverTreeModel::removeObserverList(ObserverList* list)
{

}
