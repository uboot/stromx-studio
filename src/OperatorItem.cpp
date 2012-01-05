#include "OperatorItem.h"

#include <QBrush>
#include <stromx/core/Operator.h>

OperatorItem::OperatorItem(QGraphicsItem* parent)
  : QGraphicsObject(parent),
    m_op(0)
{
    m_rectItem = new QGraphicsRectItem(this);
    m_rectItem->setRect(0, 0, 50, 50);
    
    setFlag(ItemIsMovable, true);
    setFlag(ItemIsSelectable, true);
    setFlag(ItemIsFocusable, true);
}

OperatorItem::~OperatorItem()
{
    delete m_op;
}

void OperatorItem::setOperator(stromx::core::Operator* op)
{
    m_op = op;
}

QRectF OperatorItem::boundingRect() const
{
    return m_rectItem->boundingRect();
}

void OperatorItem::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
{
    m_rectItem->paint(painter, option, widget);
}

