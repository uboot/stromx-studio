#include "ConnectionModel.h"

#include "StreamModel.h"

ConnectionModel::ConnectionModel(OperatorModel* op, unsigned int id, StreamModel* parent)
  : QAbstractTableModel(parent),
    m_op(op),
    m_id(id),
    m_stream(parent),
    m_thread(0)
{

}

int ConnectionModel::columnCount(const QModelIndex& index) const
{
    return 0;
}

int ConnectionModel::rowCount(const QModelIndex& index) const
{
    return 0;
}

QVariant ConnectionModel::data(const QModelIndex& index, int role) const
{
    return QVariant();
}

void ConnectionModel::setThread(ThreadModel* thread)
{
    if(m_thread != thread)
    {
        m_thread = thread;
        emit threadChanged(m_thread);
    }
}

