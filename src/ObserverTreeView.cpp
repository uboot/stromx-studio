#include "ObserverTreeView.h"

#include <QAction>
#include <QHeaderView>
#include "ItemDelegate.h"
#include "ObserverModel.h"
#include "ObserverTreeModel.h"

ObserverTreeView::ObserverTreeView(QWidget* parent)
  : QTreeView(parent)
{
    setAlternatingRowColors(true);
    setDropIndicatorShown(true);
    setItemDelegate(new ItemDelegate(this));
    setDragDropMode(QAbstractItemView::DragDrop);
    setSelectionBehavior(QAbstractItemView::SelectRows);
    setSelectionMode(QAbstractItemView::SingleSelection);
}

void ObserverTreeView::setModel(QAbstractItemModel* model)
{
    QTreeView::setModel(model);
    
    header()->setResizeMode(QHeaderView::Stretch);
    connect(selectionModel(), SIGNAL(currentRowChanged(QModelIndex,QModelIndex)), 
            this, SLOT(updateObserverSelected(QModelIndex,QModelIndex)));
}

QAction* ObserverTreeView::createAddObserverAction(QObject* parent)
{
    QAction* action = new QAction(tr("Add observer"), parent);
    action->setStatusTip(tr("Add a new observer window"));
    action->setEnabled(true);
    connect(action, SIGNAL(triggered()), this, SLOT(addObserver()));
    
    return action;
}

QAction* ObserverTreeView::createRemoveObserverAction(QObject* parent)
{
    QAction* action = new QAction(tr("Remove observer"), parent);
    action->setStatusTip(tr("Remove the selected observer"));
    action->setEnabled(false);
    connect(action, SIGNAL(triggered()), this, SLOT(removeSelectedEntry()));
    connect(this, SIGNAL(observerSelectedChanged(bool)), action, SLOT(setEnabled(bool)));
    
    return action;
}

QAction* ObserverTreeView::createRemoveInputAction(QObject* parent)
{
    QAction* action = new QAction(tr("Remove input"), parent);
    action->setStatusTip(tr("Remove the selected input"));
    action->setEnabled(false);
    connect(action, SIGNAL(triggered()), this, SLOT(removeSelectedEntry()));
    connect(this, SIGNAL(inputSelectedChanged(bool)), action, SLOT(setEnabled(bool)));
    
    return action;
}

void ObserverTreeView::addObserver()
{
    model()->insertRow(model()->rowCount());
}

void ObserverTreeView::removeSelectedEntry()
{        
    if(selectionModel())
    {
        QModelIndex index = selectionModel()->currentIndex();
        if(index.isValid())
            model()->removeRows(index.row(), 1, index.parent());
    }
}

void ObserverTreeView::updateObserverSelected(const QModelIndex& current, const QModelIndex& previous)
{
    if(current.isValid())
    {
        if(current.parent().isValid())
        {
            // input is selected
            emit inputSelectedChanged(true);
            emit observerSelectedChanged(false);
        }
        else
        {
            // observer is selected
            emit inputSelectedChanged(false);
            emit observerSelectedChanged(true);
        }
    }
    else
    {
        // nothing is selected
        emit inputSelectedChanged(false);
        emit observerSelectedChanged(false);
    }
}


