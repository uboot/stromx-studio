#include "ThreadEditor.h"

#include <QAction>
#include <QHBoxLayout>
#include <QHeaderView>
#include <QTableView>
#include "ItemDelegate.h"
#include "StreamModel.h"
#include "ThreadListModel.h"

ThreadEditor::ThreadEditor(QWidget* parent)
  : QWidget(parent),
    m_model(0)
{    
    m_view = new QTableView;
    m_view->setItemDelegate(new ItemDelegate(this));
    m_view->verticalHeader()->setDefaultSectionSize(ItemDelegate::ROW_HEIGHT);
    m_view->setEditTriggers(QAbstractItemView::AllEditTriggers);
    m_view->setShowGrid(false);
    m_view->setAlternatingRowColors(true);
    
    QHBoxLayout* layout = new QHBoxLayout;
    layout->addWidget(m_view);
    
    setLayout(layout);
}

void ThreadEditor::setModel(StreamModel* model)
{
    m_model = model;
    
    m_view->setModel(m_model->threadListModel());
    m_view->horizontalHeader()->setResizeMode(QHeaderView::Stretch);
    m_view->verticalHeader()->hide();
    m_view->setSelectionBehavior(QAbstractItemView::SelectRows);
    m_view->setSelectionMode(QAbstractItemView::SingleSelection);
    connect(m_view->selectionModel(), SIGNAL(currentRowChanged(QModelIndex,QModelIndex)), 
            this, SLOT(updateThreadSelected(QModelIndex,QModelIndex)));
    connect(m_model, SIGNAL(streamStarted()), this, SLOT(updateStreamActive()));
    connect(m_model, SIGNAL(streamJoined()), this, SLOT(updateStreamActive()));
}

QAction* ThreadEditor::createAddThreadAction(QObject* parent)
{    
    QAction* action = new QAction(tr("Add thread"), parent);
    action->setStatusTip(tr("Add a thread to the stream"));
    action->setShortcut(tr("Ctrl+T"));
    action->setEnabled(true);
    connect(action, SIGNAL(triggered()), this, SLOT(addThread()));
    connect(this, SIGNAL(addThreadActiveChanged(bool)), action, SLOT(setEnabled(bool)));
    
    return action;
}

QAction* ThreadEditor::createRemoveThreadAction(QObject* parent)
{
    QAction* action = new QAction(tr("Remove thread"), parent);
    action->setStatusTip(tr("Remove the selected thread from the stream"));
    action->setShortcut(tr("Ctrl+Shift+T"));
    action->setEnabled(false);
    connect(action, SIGNAL(triggered()), this, SLOT(removeThread()));
    connect(this, SIGNAL(removeThreadActiveChanged(bool)), action, SLOT(setEnabled(bool)));
    
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
    if(m_view->selectionModel())
    {
        QModelIndex index = m_view->selectionModel()->currentIndex();
        if(index.isValid())
            return reinterpret_cast<ThreadModel*>(index.internalPointer());
    }
    
    return 0;
}

void ThreadEditor::updateThreadSelected(const QModelIndex& current, const QModelIndex& previous)
{
    if(current.isValid() && ! m_model->isActive())
        emit removeThreadActiveChanged(true);
    else
        emit removeThreadActiveChanged(false);
}

void ThreadEditor::updateStreamActive()
{
    if(! m_model->isActive())
    {
        emit addThreadActiveChanged(true);
        
        if(selectedThread())
            emit removeThreadActiveChanged(true);
        else
            emit removeThreadActiveChanged(false);
    }
    else
    {
        emit addThreadActiveChanged(false);
        emit removeThreadActiveChanged(false);
    }
            
}




