#include "ArrowItem.h"

#include <QGraphicsView>
#include <QPen>
#include <QVector2D>
#include <math.h>
#include "model/ConnectionModel.h"

ArrowItem::ArrowItem(QGraphicsItem* parent)
  : QGraphicsObject(parent),
    m_shaft(new QGraphicsLineItem(this)),
    m_head1(new QGraphicsLineItem(0, 0, -5, -5, this)),
    m_head2(new QGraphicsLineItem(0, 0, -5, 5, this))
{
    setFlag(ItemIsSelectable, true);
    
    m_pen.setWidth(INACTIVE_WIDTH);
    m_shaft->setPen(m_pen);
    m_head1->setPen(m_pen);
    m_head2->setPen(m_pen);
}

QRectF ArrowItem::boundingRect() const
{
    return m_shaft->boundingRect();
}

void ArrowItem::rotateHead()
{
    double angle = - m_shaft->line().angle();
    m_head1->setRotation(angle);
    m_head2->setRotation(angle);
}

void ArrowItem::setStart(const QPointF& start)
{
    m_shaft->setLine(start.x(), start.y(),
                     m_shaft->line().p2().x(), m_shaft->line().p2().y());
    rotateHead();
}

void ArrowItem::setEnd(const QPointF& end)
{
    m_shaft->setLine(m_shaft->line().p1().x(),
                     m_shaft->line().p1().y(), end.x(), end.y());
    m_head1->setPos(end);
    m_head2->setPos(end);
    rotateHead();
}

void ArrowItem::setActive(bool value)
{
    if(value)
        m_pen.setWidth(ACTIVE_WIDTH);
    else
        m_pen.setWidth(INACTIVE_WIDTH);
    
    applyPen();
}

void ArrowItem::applyPen()
{
    m_shaft->setPen(m_pen);
    m_head1->setPen(m_pen);
    m_head2->setPen(m_pen);
}





