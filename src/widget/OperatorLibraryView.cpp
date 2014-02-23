#include "widget/OperatorLibraryView.h"

#include <QApplication>
#include <QDrag>
#include <QHeaderView>
#include <QMouseEvent>
#include "data/OperatorData.h"
#include "delegate/ItemDelegate.h"
#include "model/OperatorLibraryModel.h"

OperatorLibraryView::OperatorLibraryView(QWidget* parent)
  : QTreeView(parent),
    m_model(0)
{    
    m_model = new OperatorLibraryModel(this);
    
    setModel(m_model);
    setItemDelegate(new ItemDelegate(this));
    
#ifdef STROMX_STUDIO_QT4
    header()->setResizeMode(OperatorLibraryModel::OPERATOR, QHeaderView::Interactive);
    header()->setResizeMode(OperatorLibraryModel::VERSION, QHeaderView::Stretch);
#else
    header()->setSectionResizeMode(OperatorLibraryModel::OPERATOR, QHeaderView::Interactive);
    header()->setSectionResizeMode(OperatorLibraryModel::VERSION, QHeaderView::Stretch);
#endif // STROMX_STUDIO_QT4
    
    setAlternatingRowColors(true);
}

void OperatorLibraryView::mouseMoveEvent(QMouseEvent* event)
{
    if(event->buttons() & Qt::LeftButton)
    {
        int distance = (event->pos() - m_startPos).manhattanLength();
        
        if(distance >= QApplication::startDragDistance())
            startDrag();
    }
        
    QTreeView::mouseMoveEvent(event);
}

void OperatorLibraryView::mousePressEvent(QMouseEvent* event)
{
    if(event->button() == Qt::LeftButton)
        m_startPos = event->pos();
    
    QTreeView::mousePressEvent(event);
}

void OperatorLibraryView::startDrag()
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