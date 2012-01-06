#include "OperatorModel.h"

OperatorModel::OperatorModel(stromx::core::Operator* const op, QObject* parent)
  : QAbstractListModel(parent),
    m_op(op)
{

}

int OperatorModel::rowCount(const QModelIndex& index) const
{
    return 0;
}

QVariant OperatorModel::data(const QModelIndex& index, int role) const
{
    return QVariant();
}

void OperatorModel::setPos(const QPointF& pos)
{
    if(m_pos != pos)
    {
        m_pos = pos;
        emit posChanged(m_pos);
    }
}
