#include "ConnectorItem.h"
#include <QGraphicsSceneMouseEvent>

ConnectorItem::ConnectorItem(QGraphicsItem* parent)
  : QGraphicsRectItem(parent),
    m_currentConnection(0)
{
    setRect(0, 0, 10, 10);
}

void ConnectorItem::mousePressEvent(QGraphicsSceneMouseEvent* event)
{
    QPointF start(5, 5);
    QPointF end(mapFromScene(event->scenePos()));
    m_currentConnection = new QGraphicsLineItem(start.x(), start.y(), end.x(), end.y(), this);
}

void ConnectorItem::mouseMoveEvent(QGraphicsSceneMouseEvent* event)
{
    if(m_currentConnection)
    {
        QPointF start(m_currentConnection->line().p1());
        QPointF end(mapFromScene(event->scenePos()));
        m_currentConnection->setLine(start.x(), start.y(), end.x(), end.y());
    }
    else
    {
        QGraphicsItem::mouseMoveEvent(event);
    }
}

void ConnectorItem::mouseReleaseEvent(QGraphicsSceneMouseEvent* event)
{
    if(m_currentConnection)
    {
        delete m_currentConnection;
        m_currentConnection = 0;
    }
    else
    {
        QGraphicsItem::mouseReleaseEvent(event);
    }
}
