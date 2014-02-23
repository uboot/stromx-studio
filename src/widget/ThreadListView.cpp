#include "widget/ThreadListView.h"

#include <QAction>
#include <QHeaderView>
#include <QContextMenuEvent>
#include <QMenu>
#include <QTableView>
#include "delegate/ItemDelegate.h"
#include "model/StreamModel.h"
#include "model/ThreadListModel.h"

ThreadListView::ThreadListView(QWidget* parent)
  : QTableView(parent),
    m_model(0),
    m_addThreadAct(0),
    m_removeThreadAct(0)
{    
    setItemDelegate(new ItemDelegate(this));
    setShowGrid(false);
    setAlternatingRowColors(true);
    setEditTriggers(QAbstractItemView::AllEditTriggers);
    verticalHeader()->hide();
    
    m_addThreadAct = createAddThreadAction(this);
    m_removeThreadAct = createRemoveThreadAction(this);
}

void ThreadListView::setStreamModel(StreamModel* model)
{
    m_model = model;
    
    setModel(m_model->threadListModel());
    
    
#ifdef STROMX_STUDIO_QT4
    horizontalHeader()->setResizeMode(QHeaderView::Stretch);
#else
    horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
#endif // STROMX_STUDIO_QT4
    
    connect(selectionModel(), SIGNAL(currentRowChanged(QModelIndex,QModelIndex)), 
            this, SLOT(updateThreadSelected(QModelIndex,QModelIndex)));
    connect(m_model, SIGNAL(streamStarted()), this, SLOT(updateStreamActive()));
    connect(m_model, SIGNAL(streamJoined()), this, SLOT(updateStreamActive()));
}

QAction* ThreadListView::createAddThreadAction(QObject* parent)
{    
    QAction* action = new QAction(tr("Add thread"), parent);
    action->setStatusTip(tr("Add a thread to the stream"));
    action->setShortcut(tr("Ctrl+T"));
    action->setEnabled(true);
    connect(action, SIGNAL(triggered()), this, SLOT(addThread()));
    connect(this, SIGNAL(addThreadActiveChanged(bool)), action, SLOT(setEnabled(bool)));
    
    return action;
}

QAction* ThreadListView::createRemoveThreadAction(QObject* parent)
{
    QAction* action = new QAction(tr("Remove thread"), parent);
    action->setStatusTip(tr("Remove the selected thread from the stream"));
    action->setShortcut(tr("Ctrl+Shift+T"));
    action->setEnabled(false);
    connect(action, SIGNAL(triggered()), this, SLOT(removeThread()));
    connect(this, SIGNAL(removeThreadActiveChanged(bool)), action, SLOT(setEnabled(bool)));
    
    return action;
}

void ThreadListView::addThread() const
{
    m_model->addThread();
}

void ThreadListView::removeThread() const
{
    ThreadModel* thread = selectedThread();
    if(thread)
        m_model->removeThread(thread);
}

ThreadModel* ThreadListView::selectedThread() const
{
    if(selectionModel())
    {
        QModelIndex index = selectionModel()->currentIndex();
        if(index.isValid())
            return reinterpret_cast<ThreadModel*>(index.internalPointer());
    }
    
    return 0;
}

void ThreadListView::contextMenuEvent(QContextMenuEvent* event)
{
    QMenu menu(this);
    menu.addAction(m_addThreadAct);
    
    if(indexAt(event->pos()).isValid())
    {
        menu.addAction(m_removeThreadAct);
    }
    menu.exec(event->globalPos());
}

void ThreadListView::updateThreadSelected(const QModelIndex& current, const QModelIndex& /*previous*/)
{
    if(current.isValid() && ! m_model->isActive())
        emit removeThreadActiveChanged(true);
    else
        emit removeThreadActiveChanged(false);
}

void ThreadListView::updateStreamActive()
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




