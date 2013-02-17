#include "PropertyView.h"

#include <QHeaderView>
#include "ItemDelegate.h"
#include "model/PropertyModel.h"

PropertyView::PropertyView(QWidget* parent)
  : QTreeView(parent)
{
    setEditTriggers(QAbstractItemView::AllEditTriggers);
    setAlternatingRowColors(true);
    header()->setResizeMode(PropertyModel::PROPERTY, QHeaderView::Interactive);
    header()->setResizeMode(PropertyModel::VALUE, QHeaderView::Stretch);
    
    setItemDelegate(new ItemDelegate(this));
}

void PropertyView::setModel(QAbstractItemModel* model)
{
    QTreeView::setModel(model);
}

