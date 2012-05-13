#include "ConnectionItem.h"

#include <QGraphicsView>
#include <QPen>
#include <QVector2D>
#include <math.h>
#include "ConnectionModel.h"
#include "ConnectorItem.h"

ConnectionItem::ConnectionItem(ConnectionModel* model, QGraphicsItem* parent)
  : QGraphicsObject(parent),
    m_path(new QGraphicsPathItem(this)),
    m_model(model)
{
    setFlag(ItemIsSelectable, true);
    
    if(m_model)
    {
        m_pen.setColor(m_model->color());
        connect(m_model, SIGNAL(colorChanged(QColor)), this, SLOT(setColor(QColor)));
    }
    
    m_pen.setWidthF(ConnectorItem::SIZE);
    update();
}

QRectF ConnectionItem::boundingRect() const
{
    return m_path->boundingRect();
}

QPainterPath ConnectionItem::shape() const
{
    return m_path->shape();
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

        update();
        
        return value;
    }

    return QGraphicsItem::itemChange(change, value);
}

void ConnectionItem::setStart(const QPointF& start)
{
    m_start = start;
    update();
}

void ConnectionItem::setEnd(const QPointF& end)
{
    m_end = end;
    update();
}

void ConnectionItem::setColor(const QColor& color)
{
    m_pen.setColor(color);
    update();
}

void ConnectionItem::update()
{
    QPainterPath path;
    QPointF start(m_start.x() + ConnectorItem::SIZE, m_start.y());
    QPointF end(m_end.x() - ConnectorItem::SIZE, m_end.y());
    drawPath(start, end, path);
    m_path->setPath(path);
    
    m_path->setPen(m_pen);
}

void ConnectionItem::drawPath(const QPointF& start, const QPointF& end, QPainterPath& path)
{
    const qreal RADIUS = 1.5 * ConnectorItem::SIZE;
    
    QRectF arcRect(0, 0, RADIUS, RADIUS);
    qreal xCenter = (start.x() + end.x()) / 2;
    
    path.moveTo(start);
    
    if(start.y() < end.y())
    {
        // connections goes downwards
        arcRect.moveTo(xCenter - RADIUS, start.y());
        path.arcTo(arcRect, 90, -90);
        path.lineTo(xCenter, start.y() + RADIUS);
        arcRect.moveTo(xCenter, end.y() - RADIUS);
        path.arcTo(arcRect, 180, 90);
    }
    else
    {
        // connections goes upwards
        arcRect.moveTo(xCenter - RADIUS, start.y() - RADIUS);
        path.arcTo(arcRect, -90, 90);
        path.lineTo(xCenter, start.y() - RADIUS);
        arcRect.moveTo(xCenter, end.y());
        path.arcTo(arcRect, 180, -90);
    }
    
    path.lineTo(xCenter + RADIUS, end.y());
    path.lineTo(end);
}






