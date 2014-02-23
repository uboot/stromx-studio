#include "widget/PropertyView.h"

#include <QHeaderView>
#include "delegate/ItemDelegate.h"
#include "model/PropertyModel.h"

PropertyView::PropertyView(QWidget* parent)
  : QTreeView(parent)
{
    setEditTriggers(QAbstractItemView::AllEditTriggers);
    setAlternatingRowColors(true);
    
#ifdef STROMX_STUDIO_QT4
    header()->setResizeMode(PropertyModel::PROPERTY, QHeaderView::Interactive);
    header()->setResizeMode(PropertyModel::VALUE, QHeaderView::Stretch);
#else
    header()->setSectionResizeMode(PropertyModel::PROPERTY, QHeaderView::Interactive);
    header()->setSectionResizeMode(PropertyModel::VALUE, QHeaderView::Stretch);
#endif // STROMX_STUDIO_QT4
    
    setItemDelegate(new ItemDelegate(this));
}

void PropertyView::setModel(QAbstractItemModel* model)
{
    QTreeView::setModel(model);
}

