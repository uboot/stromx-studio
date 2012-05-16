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
    m_model(model),
    m_occupied(false)
{
    Q_ASSERT(model);
    
    for(unsigned int i = 0; i < 3; ++i)
        m_arrows[i] = 0;
    
    setFlag(ItemIsSelectable, true);
    
    m_pen.setWidthF(ConnectorItem::SIZE);
    m_pen.setColor(m_model->color());
    connect(m_model, SIGNAL(colorChanged(QColor)), this, SLOT(setColor(QColor)));
    
    for(unsigned int i = 0; i < 3; ++i)
        m_arrows[i] = createDoubleArrow(this);
    
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
            m_pen.setStyle(Qt::DotLine);
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
    qreal width = m_end.x() - m_start.x();
    qreal height = m_end.y() - m_start.y();
    
    drawPath(start, end, path);
    m_path->setPath(path);
    m_path->setPen(m_pen);
    
    m_arrows[0]->setPos(m_start.x() + 0.25 * width, m_start.y());
    m_arrows[1]->setPos(m_start.x() + 0.5 * width, m_start.y() + 0.5 * height);
    if(height > 0.0)
        m_arrows[1]->setRotation(90);
    else
        m_arrows[1]->setRotation(-90);
    m_arrows[2]->setPos(start.x() + 0.75 * width, end.y());
    
    for(unsigned int i = 0; i < 3; ++i)
    {
        QBrush brush;
        if(m_occupied)
            brush = QBrush(m_pen.color().darker(130));
        else
            brush = QBrush(m_pen.color().lighter(130));
        
        m_arrows[i]->setBrush(brush);
    }
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

QGraphicsPathItem* ConnectionItem::createDoubleArrow(QGraphicsItem* parent)
{
    const qreal WIDTH = 0.5 * ConnectorItem::SIZE;
    
    QPainterPath path;
    
    // draw the first single arrow
    path.moveTo(-1.5 * WIDTH, 0.0);
    path.lineTo(-2.0 * WIDTH, -WIDTH);
    path.lineTo(-WIDTH, -WIDTH);
    path.lineTo(-0.5 * WIDTH, 0.0);
    path.lineTo(-WIDTH, WIDTH);
    path.lineTo(-2.0 * WIDTH, WIDTH);
    path.closeSubpath();
    
    // the second one
    path.moveTo(0.5 * WIDTH, 0.0);
    path.lineTo(0, -WIDTH);
    path.lineTo(WIDTH, -WIDTH);
    path.lineTo(1.5 * WIDTH, 0.0);
    path.lineTo(WIDTH, WIDTH);
    path.lineTo(0 * WIDTH, WIDTH);
    path.closeSubpath();
    
    QGraphicsPathItem* item = new QGraphicsPathItem(path, parent);
    item->setPen(QPen(Qt::NoPen));
    
    return item;
}

void ConnectionItem::setOccupied(bool occupied)
{
    m_occupied = occupied;
    update();
}





