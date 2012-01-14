#include "ConnectionItem.h"

#include <QPen>
#include <QVector2D>
#include <math.h>

ConnectionItem::ConnectionItem(ConnectionModel* connection, QGraphicsItem* parent)
  : QGraphicsLineItem(parent),
    m_head1(new QGraphicsLineItem(0, 0, -5, -5, this)),
    m_head2(new QGraphicsLineItem(0, 0, -5, 5, this)),
    m_connection(0)
{
    setFlag(ItemIsSelectable, true);
}

void ConnectionItem::rotateHead()
{
    double angle = - line().angle();
    m_head1->setRotation(angle);
    m_head2->setRotation(angle);
}

void ConnectionItem::setStart(const QPointF& start)
{
    setLine(start.x(), start.y(), line().p2().x(), line().p2().y());
    rotateHead();
}

void ConnectionItem::setEnd(const QPointF& end)
{
    setLine(line().p1().x(), line().p1().y(), end.x(), end.y());
    m_head1->setPos(end);
    m_head2->setPos(end);
    rotateHead();
}

void ConnectionItem::setActive(bool value)
{
    if(value)
    {
        setPen(QPen(Qt::red));
        m_head1->setPen(QPen(Qt::red));
        m_head2->setPen(QPen(Qt::red));
    }
    else
    {
        setPen(QPen(Qt::black));
        m_head1->setPen(QPen(Qt::black));
        m_head2->setPen(QPen(Qt::black));
    }
}


