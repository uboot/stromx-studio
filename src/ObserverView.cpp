#include "ObserverView.h"

#include <QHeaderView>
#include "ItemDelegate.h"
#include "ObserverModel.h"

ObserverView::ObserverView(ObserverModel* observer, QWidget* parent)
  : QTableView(parent),
    m_observer(observer)
{
    Q_ASSERT(m_observer);
    
    setModel(observer);
    setShowGrid(false);
    setAlternatingRowColors(true);
    setDropIndicatorShown(true);
    setDragDropMode(QAbstractItemView::DragDrop);
    setSelectionBehavior(QAbstractItemView::SelectRows);
    setSelectionMode(QAbstractItemView::SingleSelection);
    horizontalHeader()->setResizeMode(QHeaderView::Stretch);
    verticalHeader()->setDefaultSectionSize(ItemDelegate::ROW_HEIGHT);
    verticalHeader()->hide();
    setItemDelegate(new ItemDelegate(this));
    setEditTriggers(QAbstractItemView::AllEditTriggers);
}
