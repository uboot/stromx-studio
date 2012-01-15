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
}


