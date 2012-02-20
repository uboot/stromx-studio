#include "ObserverModel.h"

#include "InputModel.h"

ObserverModel::ObserverModel(QUndoStack* undoStack, QObject * parent)
  : QAbstractTableModel(parent),
    m_undoStack(undoStack)
{
    m_name = "New observer";
}

int ObserverModel::rowCount(const QModelIndex& parent) const
{
    return m_observers.count();
}

int ObserverModel::columnCount(const QModelIndex& parent) const
{
    return 2;
}

QVariant ObserverModel::headerData(int section, Qt::Orientation orientation, int role) const
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

QVariant ObserverModel::data(const QModelIndex& index, int role) const
{
    return QVariant();
}

void ObserverModel::setName(const QString& name)
{
    m_name = name;
}
