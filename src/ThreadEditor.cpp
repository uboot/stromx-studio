#include "ThreadEditor.h"

#include <QAction>
#include <QHBoxLayout>
#include <QHeaderView>
#include <QTableView>
#include "StreamModel.h"
#include "ThreadListModel.h"

ThreadEditor::ThreadEditor(QWidget* parent)
  : QWidget(parent),
    m_model(0)
{    
    m_table = new QTableView;
    m_table->verticalHeader()->hide();
    
    QHBoxLayout* layout = new QHBoxLayout;
    layout->addWidget(m_table);
    
    setLayout(layout);
}

void ThreadEditor::setModel(StreamModel* model)
{
    m_model = model;
    
    m_table->setModel(m_model->threadListModel());
    m_table->horizontalHeader()->setResizeMode(QHeaderView::Stretch);
    m_table->setSelectionBehavior(QAbstractItemView::SelectRows);
    m_table->setSelectionMode(QAbstractItemView::SingleSelection);
    connect(m_table->selectionModel(), SIGNAL(currentRowChanged(QModelIndex,QModelIndex)), 
            this, SLOT(updateThreadSelected(QModelIndex,QModelIndex)));
}

QAction* ThreadEditor::createAddThreadAction(QObject* parent)
{    
    QAction* action = new QAction(tr("Add thread"), parent);
    action->setStatusTip(tr("Add a thread to the stream"));
    action->setShortcut(tr("Ctrl+T"));
    action->setEnabled(true);
    connect(action, SIGNAL(triggered()), this, SLOT(addThread()));
    
    return action;
}

QAction* ThreadEditor::createRemoveThreadAction(QObject* parent)
{
    QAction* action = new QAction(tr("Remove thread"), parent);
    action->setStatusTip(tr("Remove the selected thread from the stream"));
    action->setShortcut(tr("Ctrl+Shift+T"));
    action->setEnabled(false);
    connect(action, SIGNAL(triggered()), this, SLOT(removeThread()));
    connect(this, SIGNAL(threadSelectedChanged(bool)), action, SLOT(setEnabled(bool)));
    
    return action;
}

void ThreadEditor::addThread() const
{
    m_model->addThread();
}

void ThreadEditor::removeThread() const
{
    ThreadModel* thread = selectedThread();
    if(thread)
        m_model->removeThread(thread);
}

ThreadModel* ThreadEditor::selectedThread() const
{
    if(m_table->selectionModel())
    {
        QModelIndex index = m_table->selectionModel()->currentIndex();
        if(index.isValid())
            return m_model->threadListModel()->thread(index);
    }
    
    return 0;
}

void ThreadEditor::updateThreadSelected(const QModelIndex& current, const QModelIndex& previous)
{
    if(current.isValid())
        emit threadSelectedChanged(true);
    else
        emit threadSelectedChanged(false);
}



