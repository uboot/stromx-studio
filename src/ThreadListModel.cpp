#include "ThreadListModel.h"

#include "ThreadModel.h"

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
    return NUM_COLUMNS;
}

QVariant ThreadListModel::data(const QModelIndex& index, int role) const
{
    if(role != Qt::DisplayRole)
        return QVariant();
    
    switch(index.column())
    {
    case 0:
        return m_threads[index.row()]->name();
    case 1:
        return m_threads[index.row()]->color();
    default:
        ;
    }
    
    return QVariant();
}

bool ThreadListModel::setData(const QModelIndex& index, const QVariant& value, int role)
{
    if(index.isValid() && index.column() == 0)
    {
        QString newName = value.toString();
        
        if(newName.isEmpty())
            return false;
        
        ThreadModel* thread = reinterpret_cast<ThreadModel*>(index.internalPointer());
        thread->setName(newName);
        emit dataChanged(index, index);
    }
    
    return false;
}

Qt::ItemFlags ThreadListModel::flags(const QModelIndex& index) const
{
    Qt::ItemFlags flags = QAbstractItemModel::flags(index);
    if(index.column() == 0)
        return flags |= Qt::ItemIsEditable;
        
    return flags;
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
    connect(thread, SIGNAL(changed(ThreadModel*)), this, SLOT(updateThread(ThreadModel*)));
    endInsertRows();
}

void ThreadListModel::removeThread(ThreadModel* thread)
{
    int i = m_threads.indexOf(thread);
    
    if(i >= 0)
    {
        beginRemoveRows(QModelIndex(), i, i);
        m_threads.removeAt(i);
        endRemoveRows();
    }
}

ThreadModel* ThreadListModel::thread(const QModelIndex& index) const
{
    if(index.row() >= 0 && index.row() < m_threads.size())
        return m_threads[index.row()];
    else
        return 0;
}

QModelIndex ThreadListModel::index(int row, int column, const QModelIndex & parent) const
{
    if(row >= 0 && row < m_threads.size())
        return createIndex(row, column, m_threads[row]);
    else
        return QModelIndex();
}

void ThreadListModel::removeAllThreads()
{
    beginResetModel();
    m_threads.clear();
    endResetModel();
}

void ThreadListModel::updateThread(ThreadModel* thread)
{
    int pos = m_threads.indexOf(thread);
    if(pos >= 0)
        emit dataChanged(createIndex(pos, 0), createIndex(pos, NUM_COLUMNS - 1));
}

QDataStream& operator<<(QDataStream& stream, const ThreadListModel* threadList)
{
    return stream << threadList->m_threads;
}

QDataStream& operator>>(QDataStream& stream, ThreadListModel* threadList)
{
    return stream >> threadList->m_threads;
}



