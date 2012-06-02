#include "PropertyModel.h"

PropertyModel::PropertyModel(QObject* parent)
  : QAbstractTableModel(parent)
{
}

int PropertyModel::columnCount(const QModelIndex& index) const
{
    return 2;
}

QVariant PropertyModel::headerData(int section, Qt::Orientation orientation, int role) const
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
