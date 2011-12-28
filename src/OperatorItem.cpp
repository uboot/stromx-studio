#include "OperatorItem.h"

#include <QBrush>
#include <stromx/core/Operator.h>

OperatorItem::OperatorItem(QGraphicsItem* parent)
  : QGraphicsRectItem(parent),
    m_op(0)
{
    setRect(0, 0, 50, 50);
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


