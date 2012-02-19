#include "ObserverListModel.h"

#include "InputModel.h"

ObserverListModel::ObserverListModel(QUndoStack* undoStack, QObject * parent)
  : QAbstractTableModel(parent),
    m_undoStack(undoStack)
{
    m_name = "New observer";
}

int ObserverListModel::rowCount(const QModelIndex& parent) const
{
    return m_observers.count();
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

void ObserverListModel::setName(const QString& name)
{
    m_name = name;
}
