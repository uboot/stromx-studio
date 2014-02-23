#include "widget/PropertyView.h"

#include <QHeaderView>
#include "delegate/ItemDelegate.h"
#include "model/PropertyModel.h"

PropertyView::PropertyView(QWidget* parent)
  : QTreeView(parent)
{
    setEditTriggers(QAbstractItemView::AllEditTriggers);
    setAlternatingRowColors(true);
    header()->setSectionResizeMode(PropertyModel::PROPERTY, QHeaderView::Interactive);
    header()->setSectionResizeMode(PropertyModel::VALUE, QHeaderView::Stretch);
    
    setItemDelegate(new ItemDelegate(this));
}

void PropertyView::setModel(QAbstractItemModel* model)
{
    QTreeView::setModel(model);
}

