#include "PropertyModel.h"

PropertyModel::PropertyModel(QObject* parent)
  : QAbstractItemModel(parent)
{
}

int PropertyModel::columnCount(const QModelIndex& index) const
{
    return NUM_COLUMNS;
}

QVariant PropertyModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if(orientation == Qt::Horizontal && role == Qt::DisplayRole)
    {
        if(section == PROPERTY)
            return tr("Property");
        else
            return tr("Value");
    }
    
    return QVariant();
}
