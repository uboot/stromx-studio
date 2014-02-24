#include "widget/PropertyView.h"

#include <QHeaderView>
#include "delegate/ItemDelegate.h"
#include "model/PropertyModel.h"

PropertyView::PropertyView(QWidget* parent)
  : QTreeView(parent)
{
    setEditTriggers(QAbstractItemView::AllEditTriggers);
    setAlternatingRowColors(true);
    
    setItemDelegate(new ItemDelegate(this));
}

void PropertyView::setModel(PropertyModel* model)
{
    QTreeView::setModel(model);
    
    if (model)
    {
#ifdef STROMX_STUDIO_QT4
        header()->setResizeMode(PropertyModel::PROPERTY, QHeaderView::Interactive);
        header()->setResizeMode(PropertyModel::VALUE, QHeaderView::Stretch);
#else
        header()->setSectionResizeMode(PropertyModel::PROPERTY, QHeaderView::Interactive);
        header()->setSectionResizeMode(PropertyModel::VALUE, QHeaderView::Stretch);
#endif // STROMX_STUDIO_QT4
    }
}

