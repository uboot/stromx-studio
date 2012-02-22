#include "ObserverEditor.h"

#include <QAction>
#include <QHeaderView>
#include <QTreeView>
#include "ObserverModel.h"
#include "ObserverTreeModel.h"

ObserverEditor::ObserverEditor(QWidget* parent)
  : QDockWidget(parent),
    m_observerView(0)
{
    setWindowTitle("Observers");
    setObjectName("ObserverEditor");
    
    m_observerView = new QTreeView;
    setWidget(m_observerView);
}

void ObserverEditor::setModel(QAbstractItemModel* model)
{
    m_observerView->setModel(model);
    m_observerView->setDragDropMode(QAbstractItemView::DragDrop);
    m_observerView->header()->setResizeMode(QHeaderView::Stretch);
    m_observerView->setSelectionBehavior(QAbstractItemView::SelectRows);
    m_observerView->setSelectionMode(QAbstractItemView::SingleSelection);
    connect(m_observerView->selectionModel(), SIGNAL(currentRowChanged(QModelIndex,QModelIndex)), 
            this, SLOT(updateObserverSelected(QModelIndex,QModelIndex)));
}

QAction* ObserverEditor::createAddObserverAction(QObject* parent)
{
    QAction* action = new QAction(tr("Add observer"), parent);
    action->setStatusTip(tr("Add a new observer window"));
    action->setEnabled(true);
    connect(action, SIGNAL(triggered()), this, SLOT(addObserverList()));
    
    return action;
}

QAction* ObserverEditor::createRemoveObserverAction(QObject* parent)
{
    QAction* action = new QAction(tr("Remove observer"), parent);
    action->setStatusTip(tr("Remove the selected observer"));
    action->setEnabled(false);
    connect(action, SIGNAL(triggered()), this, SLOT(removeSelectedEntry()));
    connect(this, SIGNAL(observerSelectedChanged(bool)), action, SLOT(setEnabled(bool)));
    
    return action;
}

QAction* ObserverEditor::createRemoveInputAction(QObject* parent)
{
    QAction* action = new QAction(tr("Remove input"), parent);
    action->setStatusTip(tr("Remove the selected input"));
    action->setEnabled(false);
    connect(action, SIGNAL(triggered()), this, SLOT(removeSelectedEntry()));
    connect(this, SIGNAL(inputSelectedChanged(bool)), action, SLOT(setEnabled(bool)));
    
    return action;
}

void ObserverEditor::addObserverList()
{
    m_observerView->model()->insertRow(m_observerView->model()->rowCount());
}

void ObserverEditor::removeSelectedEntry()
{        
    if(m_observerView->selectionModel())
    {
        QModelIndex index = m_observerView->selectionModel()->currentIndex();
        if(index.isValid())
            m_observerView->model()->removeRows(index.row(), 1, index.parent());
    }
}

void ObserverEditor::updateObserverSelected(const QModelIndex& current, const QModelIndex& previous)
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


