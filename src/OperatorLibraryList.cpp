#include "OperatorLibraryList.h"

#include <QApplication>
#include <QMouseEvent>
#include "OperatorLibraryModel.h"
#include "StromxData.h"

OperatorLibraryList::OperatorLibraryList(QWidget* parent)
  : QTreeView(parent),
    m_model(0)
{
    m_model = new OperatorLibraryModel(this);
    setModel(m_model);
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
        // obtain a new operator from the factory
        stromx::core::Operator* op = m_model->newOperator(index);
        StromxData* data = new StromxData(op);
        
        QDrag* drag = new QDrag(this);
        drag->setMimeData(data);
        
        // if the drag failed delete the operator in the drag object
        if(! drag->exec(Qt::CopyAction, Qt::CopyAction) == Qt::CopyAction)
           data->deleteData();
    }
}