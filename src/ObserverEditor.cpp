#include "ObserverEditor.h"

#include <QAction>
#include <QTreeView>
#include "ObserverListModel.h"
#include "ObserverTreeModel.h"

ObserverEditor::ObserverEditor(QWidget* parent)
  : QDockWidget(parent),
    m_model(0),
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
}

QAction* ObserverEditor::createAddObserverListAction(QObject* parent)
{
    QAction* action = new QAction(tr("Add observer"), parent);
    action->setStatusTip(tr("Add a new observer window"));
    action->setEnabled(true);
    connect(action, SIGNAL(triggered()), this, SLOT(addObserverList()));
    
    return action;
}

QAction* ObserverEditor::createRemoveObserverListAction(QObject* parent)
{
    QAction* action = new QAction(tr("Delete observer"), parent);
    action->setStatusTip(tr("Delete the selected observer"));
    action->setEnabled(true);
    connect(action, SIGNAL(triggered()), this, SLOT(removeObserverList()));
    
    return action;
}

void ObserverEditor::addObserverList()
{
    
}

void ObserverEditor::removeObserverList()
{

}

