#include "ConnectionItem.h"

#include <QGraphicsView>
#include <QPen>
#include <QVector2D>
#include <math.h>
#include "ConnectionModel.h"
#include "InputData.h"

ConnectionItem::ConnectionItem(ConnectionModel* model, QGraphicsItem* parent)
  : QGraphicsObject(parent),
    m_shaft(new QGraphicsLineItem(this)),
    m_head1(new QGraphicsLineItem(0, 0, -5, -5, this)),
    m_head2(new QGraphicsLineItem(0, 0, -5, 5, this)),
    m_model(model)
{
    setFlag(ItemIsSelectable, true);
    m_pen.setWidth(3);
    m_pen.setColor(m_model->color());
    m_shaft->setPen(m_pen);
    m_head1->setPen(m_pen);
    m_head2->setPen(m_pen);
    
    connect(m_model, SIGNAL(colorChanged(QColor)), this, SLOT(setColor(QColor)));
}

QRectF ConnectionItem::boundingRect() const
{
    return m_shaft->boundingRect();
}

QPainterPath ConnectionItem::shape() const
{
    return m_shaft->shape();
}

void ConnectionItem::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
{
}

QVariant ConnectionItem::itemChange(GraphicsItemChange change, const QVariant &value)
{
    if(change == ItemSelectedHasChanged)
    {
        if(isSelected())
            m_pen.setStyle(Qt::DashLine);
        else
            m_pen.setStyle(Qt::SolidLine);

        applyPen();
        
        return value;
    }

    return QGraphicsItem::itemChange(change, value);
}

void ConnectionItem::rotateHead()
{
    double angle = - m_shaft->line().angle();
    m_head1->setRotation(angle);
    m_head2->setRotation(angle);
}

void ConnectionItem::setStart(const QPointF& start)
{
    m_shaft->setLine(start.x(), start.y(),
                     m_shaft->line().p2().x(), m_shaft->line().p2().y());
    rotateHead();
}

void ConnectionItem::setEnd(const QPointF& end)
{
    m_shaft->setLine(m_shaft->line().p1().x(),
                     m_shaft->line().p1().y(), end.x(), end.y());
    m_head1->setPos(end);
    m_head2->setPos(end);
    rotateHead();
}

void ConnectionItem::setActive(bool value)
{
    if(value)
        m_pen.setWidth(ACTIVE_WIDTH);
    else
        m_pen.setWidth(INACTIVE_WIDTH);
    
    applyPen();
}

void ConnectionItem::setColor(const QColor& color)
{
    m_pen.setColor(color);
    applyPen();
}

void ConnectionItem::applyPen()
{
    m_shaft->setPen(m_pen);
    m_head1->setPen(m_pen);
    m_head2->setPen(m_pen);
}





