#include "ThreadListModel.h"

ThreadListModel::ThreadListModel(QObject* parent)
  : QAbstractTableModel(parent)
{

}

int ThreadListModel::rowCount(const QModelIndex& parent) const
{
    return m_threads.size();
}

int ThreadListModel::columnCount(const QModelIndex& parent) const
{
    return 2;
}

QVariant ThreadListModel::data(const QModelIndex& index, int role) const
{
    if(role != Qt::DisplayRole)
        return QVariant();
    
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
        return tr("Name");
    case 1:
        return tr("Color");
    default:
        ;
    }
    
    return QVariant();
}

void ThreadListModel::addThread(ThreadModel* thread)
{
    beginInsertRows(QModelIndex(), m_threads.size() + 1, m_threads.size() + 1);
    m_threads.append(thread);
    endInsertRows();
}

void ThreadListModel::removeThread(ThreadModel* thread)
{
    m_threads.removeAll(thread);
}
