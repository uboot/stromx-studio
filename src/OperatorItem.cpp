#include "OperatorItem.h"

#include <QBrush>
#include <stromx/core/Operator.h>
#include "OperatorModel.h"

OperatorItem::OperatorItem(OperatorModel* op, QGraphicsItem * parent)
  : QGraphicsObject(parent),
    m_op(op)
{
    m_rectItem = new QGraphicsRectItem(this);
    m_rectItem->setRect(0, 0, 50, 50);
    setPos(op->pos());
    
    setFlag(ItemIsMovable, true);
    setFlag(ItemIsSelectable, true);
    setFlag(ItemIsFocusable, true);
}

QRectF OperatorItem::boundingRect() const
{
    return m_rectItem->boundingRect();
}

void OperatorItem::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
{
    m_rectItem->paint(painter, option, widget);
}

