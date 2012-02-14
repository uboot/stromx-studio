#include "ObserverListModel.h"

ObserverListModel::ObserverListModel(QObject * parent)
  : QAbstractTableModel(parent)
{
}

int ObserverListModel::rowCount(const QModelIndex& parent) const
{
    return m_observer.count();
}

int ObserverListModel::columnCount(const QModelIndex& parent) const
{
    return 2;
}

QVariant ObserverListModel::headerData(int section, Qt::Orientation orientation, int role) const
{    
    if(role != Qt::DisplayRole)
        return QVariant();
        
    if(orientation != Qt::Horizontal)
        return QVariant();
    
    switch(section)
    {
    case 0:
        return tr("Input");
    case 1:
        return tr("Color");
    default:
        ;
    }
    
    return QVariant();
}

QVariant ObserverListModel::data(const QModelIndex& index, int role) const
{
    return QVariant();
}

void ObserverListModel::addObserver(Observer* observer)
{

}

void ObserverListModel::removeObserver(Observer* observer)
{

}
