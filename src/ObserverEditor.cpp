#include "ObserverEditor.h"

#include <QAction>
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
    m_observerView->setDragDropMode(QAbstractItemView::DragDrop);
    setWidget(m_observerView);
}

void ObserverEditor::setModel(QAbstractItemModel* model)
{
    m_observerView->setModel(model);
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
    QAction* action = new QAction(tr("Delete observer"), parent);
    action->setStatusTip(tr("Delete the selected observer"));
    action->setEnabled(true);
    connect(action, SIGNAL(triggered()), this, SLOT(removeObserverList()));
    connect(this, SIGNAL(observerSelectedChanged(bool)), action, SLOT(setEnabled(bool)));
    
    return action;
}

void ObserverEditor::addObserverList()
{
    m_observerView->model()->insertRow(m_observerView->model()->rowCount());
}

void ObserverEditor::removeObserverList()
{        
    if(m_observerView->selectionModel())
    {
        QModelIndex index = m_observerView->selectionModel()->currentIndex();
        if(index.isValid())
            m_observerView->model()->removeRow(index.row());
    }
}

