#include "ObserverView.h"

#include <QContextMenuEvent>
#include <QHeaderView>
#include <QMenu>
#include "ItemDelegate.h"
#include "ObserverModel.h"
#include "ObserverTreeModel.h"

ObserverView::ObserverView(ObserverModel* observer, QWidget* parent)
  : QTableView(parent),
    m_observer(observer),
    m_removeInputAct(0)
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
    
    m_removeInputAct = createRemoveInputAction(this);
}

void ObserverView::contextMenuEvent(QContextMenuEvent* event)
{
    QMenu menu(this);
    menu.addAction(m_removeInputAct);
    menu.exec(event->globalPos());
}

QAction* ObserverView::createRemoveInputAction(QObject* parent)
{
    QAction* action = new QAction(tr("Remove input"), parent);
    action->setStatusTip(tr("Remove the selected input"));
    connect(action, SIGNAL(triggered()), this, SLOT(removeInput()));
    
    return action;
}

void ObserverView::removeInput()
{
    if(selectionModel())
    {
        QModelIndex index = selectionModel()->currentIndex();
        if(index.isValid())
            model()->parentModel()->removeInput(model(), index.row());
    }
}


