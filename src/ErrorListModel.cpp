#include "ErrorListModel.h"

ErrorListModel::ErrorListModel(QObject* parent)
  : QAbstractTableModel(parent)
{
}

int ErrorListModel::columnCount(const QModelIndex& parent) const
{
    return 0;
}

int ErrorListModel::rowCount(const QModelIndex& parent) const
{
    return 0;
}

QVariant ErrorListModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    return QVariant();
}

QVariant ErrorListModel::data(const QModelIndex& index, int role) const
{
    return QVariant();
}

