#include "ConnectionItem.h"

#include <QGraphicsView>
#include <QPen>
#include <QVector2D>
#include <math.h>
#include "ConnectionModel.h"

ConnectionItem::ConnectionItem(ConnectionModel* model, QGraphicsItem* parent)
  : QGraphicsObject(parent),
    m_line(new QGraphicsLineItem(this)),
    m_model(model)
{
    setFlag(ItemIsSelectable, true);
    
    if(m_model)
    {
        m_pen.setColor(m_model->color());
        connect(m_model, SIGNAL(colorChanged(QColor)), this, SLOT(setColor(QColor)));
    }
    
    m_pen.setWidth(INACTIVE_WIDTH);
    m_line->setPen(m_pen);
}

QRectF ConnectionItem::boundingRect() const
{
    return m_line->boundingRect();
}

QPainterPath ConnectionItem::shape() const
{
    return m_line->shape();
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

void ConnectionItem::setStart(const QPointF& start)
{
    m_line->setLine(start.x(), start.y(),
                     m_line->line().p2().x(), m_line->line().p2().y());
}

void ConnectionItem::setEnd(const QPointF& end)
{
    m_line->setLine(m_line->line().p1().x(),
                    m_line->line().p1().y(), end.x(), end.y());
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
    m_line->setPen(m_pen);
}





