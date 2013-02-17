#include "ObserverTreeView.h"

#include <QAction>
#include <QContextMenuEvent>
#include <QHeaderView>
#include <QMenu>
#include "ItemDelegate.h"
#include "model/ObserverModel.h"
#include "model/ObserverTreeModel.h"

ObserverTreeView::ObserverTreeView(QWidget* parent)
  : QTreeView(parent),
    m_addObserverAct(0),
    m_removeInputAct(0),
    m_removeObserverAct(0),
    m_editObserverNameAct(0)
{
    setAlternatingRowColors(true);
    setDropIndicatorShown(true);
    setItemDelegate(new ItemDelegate(this));
    setDragDropMode(QAbstractItemView::DragDrop);
    setSelectionBehavior(QAbstractItemView::SelectRows);
    setSelectionMode(QAbstractItemView::SingleSelection);
    
    m_addObserverAct = createAddObserverAction(this);
    m_removeInputAct = createRemoveInputAction(this);
    m_removeObserverAct = createRemoveObserverAction(this);
    
    m_editObserverNameAct = new QAction(tr("Edit observer name"), this);
    connect(m_editObserverNameAct, SIGNAL(triggered(bool)), this, SLOT(editObserverName()));
}

void ObserverTreeView::setModel(QAbstractItemModel* model)
{
    QTreeView::setModel(model);
    
    header()->setResizeMode(ObserverTreeModel::OPERATOR, QHeaderView::Interactive);
    header()->setResizeMode(ObserverTreeModel::INPUT, QHeaderView::Stretch);
    
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

void ObserverTreeView::contextMenuEvent(QContextMenuEvent* event)
{
    QMenu menu(this);
    menu.addAction(m_addObserverAct);
    
    QModelIndex index = indexAt(event->pos());
    if(index.isValid())
    {
        if(index.parent().isValid())
            menu.addAction(m_removeInputAct);
        else
        {
            menu.addAction(m_removeObserverAct);
            menu.addAction(m_editObserverNameAct);
        }
    }
    menu.exec(event->globalPos());
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

void ObserverTreeView::updateObserverSelected(const QModelIndex& current,
                                              const QModelIndex& /*previous*/)
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

void ObserverTreeView::editObserverName()
{
    if(selectionModel())
    {
        QModelIndexList indices = selectionModel()->selectedRows(ObserverTreeModel::OBSERVER);
        if(indices.count() == 1)
            edit(indices[0]);
    }
}



