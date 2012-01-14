#include "ThreadListModel.h"

ThreadListModel::ThreadListModel(QObject* parent)
  : QAbstractTableModel(parent)
{

}

int ThreadListModel::rowCount(const QModelIndex& parent) const
{
    return 0;
}

int ThreadListModel::columnCount(const QModelIndex& parent) const
{
    return 3;
}

QVariant ThreadListModel::data(const QModelIndex& index, int role) const
{
    return QVariant();
}

QVariant ThreadListModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if(role != Qt::DisplayRole)
        return QVariant();
        
    if(orientation != Qt::Horizontal)
        return QVariant();
    
    switch(section)
    {
    case 0:
        return tr("Thread");
    case 1:
        return tr("Name");
    case 2:
        return tr("Color");
    default:
        ;
    }
    
    return QVariant();
}
