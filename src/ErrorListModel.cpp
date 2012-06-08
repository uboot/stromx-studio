#include "ErrorListModel.h"

ErrorListModel::ErrorListModel(QObject* parent)
  : QAbstractTableModel(parent)
{
}

int ErrorListModel::columnCount(const QModelIndex& parent) const
{
}

int ErrorListModel::rowCount(const QModelIndex& parent) const
{
}

Qt::ItemFlags ErrorListModel::flags(const QModelIndex& index) const
{
}

QVariant ErrorListModel::headerData(int section, Qt::Orientation orientation, int role) const
{
}

QVariant ErrorListModel::data(const QModelIndex& index, int role) const
{
}

