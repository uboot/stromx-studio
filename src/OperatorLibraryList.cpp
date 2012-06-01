#include "OperatorLibraryList.h"

#include <QApplication>
#include <QHeaderView>
#include <QMouseEvent>
#include "ItemDelegate.h"
#include "OperatorLibraryModel.h"
#include "OperatorData.h"

OperatorLibraryList::OperatorLibraryList(QWidget* parent)
  : QTreeView(parent),
    m_model(0)
{
}

void OperatorLibraryList::setOperatorLibraryModel(OperatorLibraryModel* model)
{
    m_model = model;
    setModel(model);
    setItemDelegate(new ItemDelegate(this));
    header()->setResizeMode(QHeaderView::Stretch);
    setAlternatingRowColors(true);
}


void OperatorLibraryList::mouseMoveEvent(QMouseEvent* event)
{
    if(event->buttons() & Qt::LeftButton)
    {
        int distance = (event->pos() - m_startPos).manhattanLength();
        
        if(distance >= QApplication::startDragDistance())
            startDrag();
    }
        
    QTreeView::mouseMoveEvent(event);
}

void OperatorLibraryList::mousePressEvent(QMouseEvent* event)
{
    if(event->button() == Qt::LeftButton)
        m_startPos = event->pos();
    
    QTreeView::mousePressEvent(event);
}

void OperatorLibraryList::startDrag()
{
    QModelIndex index = currentIndex();
    
    if(index.isValid() && m_model->isOperator(index))
    {
        // allocate a operator data object
        OperatorData* data = m_model->newOperatorData(index);
        
        QDrag* drag = new QDrag(this);
        drag->setMimeData(data);
        drag->exec(Qt::CopyAction, Qt::CopyAction);
    }
}