#include "ConnectionItem.h"

#include <QGraphicsView>
#include <QPen>
#include <QVector2D>
#include <math.h>
#include "ConnectionModel.h"

const qreal ConnectionItem::WIDTH = 2.0;

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
    
    m_pen.setWidth(ConnectionItem::WIDTH);
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
    m_path->setPen(m_pen);
    
    QPainterPath path;
    path.moveTo(m_start);
    path.lineTo((m_end.x() + m_start.x())/2, m_start.y());
    path.lineTo((m_end.x() + m_start.x())/2, m_end.y());
    path.lineTo(m_end);
    m_path->setPath(path);
}





