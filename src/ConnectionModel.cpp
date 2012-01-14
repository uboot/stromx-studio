#include "ConnectionModel.h"

#include "OperatorModel.h"
#include "StreamModel.h"

ConnectionModel::ConnectionModel(OperatorModel* sourceOp, unsigned int outputId,
                                 OperatorModel* targetOp, unsigned int inputId, StreamModel * stream)
  : QAbstractTableModel(stream),
    m_sourceOp(sourceOp),
    m_outputId(outputId),
    m_targetOp(targetOp),
    m_inputId(inputId),
    m_stream(stream),
    m_thread(0)
{
}

int ConnectionModel::columnCount(const QModelIndex& index) const
{
    return 2;
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

