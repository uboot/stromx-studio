#include "SelectionModel.h"

SelectionModel::SelectionModel(QObject* parent)
  : PropertyModel(parent)
{
}

bool SelectionModel::isValid() const
{
    return false;
}

void SelectionModel::setSelection(QList< QAbstractItemModel* > selection)
{

}

int SelectionModel::rowCount(const QModelIndex& index) const
{
    return 0;
}

QModelIndex SelectionModel::index(int row, int column, const QModelIndex& parent) const
{
    return QModelIndex();
}

QModelIndex SelectionModel::parent(const QModelIndex& child) const
{
    return QModelIndex();
}

QVariant SelectionModel::data(const QModelIndex& index, int role) const
{
    return QVariant();
}

Qt::ItemFlags SelectionModel::flags(const QModelIndex& index) const
{
    return QAbstractItemModel::flags(index);
}

bool SelectionModel::setData(const QModelIndex& index, const QVariant& value, int role)
{
    return false;
}
