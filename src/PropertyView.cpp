#include "PropertyView.h"

#include <QHeaderView>
#include "ItemDelegate.h"

PropertyView::PropertyView(QWidget* parent)
  : QTreeView(parent)
{
    setEditTriggers(QAbstractItemView::AllEditTriggers);
    setAlternatingRowColors(true);
    header()->setResizeMode(QHeaderView::Stretch);
    
    setItemDelegate(new ItemDelegate(this));
}

void PropertyView::setModel(QAbstractItemModel* model)
{
    QTreeView::setModel(model);
}

