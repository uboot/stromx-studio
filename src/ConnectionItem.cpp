#include "ConnectionItem.h"

#include <QGraphicsView>
#include <QPen>
#include <QVector2D>
#include <math.h>
#include "ConnectionModel.h"
#include "InputData.h"

ConnectionItem::ConnectionItem(ConnectionModel* model, QGraphicsItem* parent)
  : QGraphicsLineItem(parent),
    m_head1(new QGraphicsLineItem(0, 0, -5, -5, this)),
    m_head2(new QGraphicsLineItem(0, 0, -5, 5, this)),
    m_model(model)
{
    setFlag(ItemIsSelectable, true);
    m_pen.setWidth(3);
    setPen(m_pen);
    m_head1->setPen(m_pen);
    m_head2->setPen(m_pen);
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
        m_pen.setColor(Qt::red);
    else
        m_pen.setColor(Qt::black);
    
    setPen(m_pen);
    m_head1->setPen(m_pen);
    m_head2->setPen(m_pen);
}




