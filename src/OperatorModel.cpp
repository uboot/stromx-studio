#include "OperatorModel.h"
#include <stromx/core/Operator.h>

OperatorModel::OperatorModel(stromx::core::Operator* const op, QObject* parent)
  : QAbstractTableModel(parent),
    m_op(op)
{

}

int OperatorModel::rowCount(const QModelIndex& index) const
{
    return 1;
}

int OperatorModel::columnCount(const QModelIndex& index) const
{
    return 2;
}

QVariant OperatorModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if(orientation == Qt::Horizontal && role == Qt::DisplayRole)
    {
        if(section == 0)
            return tr("Property");
        else
            return tr("Value");
    }
    
    return QVariant();
}

QVariant OperatorModel::data(const QModelIndex& index, int role) const
{
    if(role != Qt::DisplayRole)
        return QVariant();
    
    if(m_op)
    {
        if(index.column() == 0)
            return tr("Type");
        else
            return QVariant(QString::fromStdString(m_op->info().type()));
    }
    
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
