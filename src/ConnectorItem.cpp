#include "ConnectorItem.h"

#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include "ConnectionItem.h"
#include <iostream>

ConnectorItem::ConnectorItem(ConnectorType type, QGraphicsItem* parent)
  : QGraphicsRectItem(parent),
    m_connectorType(type),
    m_currentConnection(0)
{
    setRect(0, 0, 10, 10);
}

void ConnectorItem::mousePressEvent(QGraphicsSceneMouseEvent* event)
{
    if(m_connectorType == INPUT)
    {
        QPointF end(5, 5);
        QPointF start(mapFromScene(event->scenePos()));
        m_currentConnection = new ConnectionItem(start, end, this);
    }
    else
    {
        QPointF start(5, 5);
        QPointF end(mapFromScene(event->scenePos()));
        m_currentConnection = new ConnectionItem(start, end, this);
    }
}

void ConnectorItem::mouseMoveEvent(QGraphicsSceneMouseEvent* event)
{
    if(m_currentConnection)
    {
        if(m_connectorType == INPUT)
        {
            QPointF start(mapFromScene(event->scenePos()));
            m_currentConnection->setStart(start);
        }
        else
        {
            QPointF end(mapFromScene(event->scenePos()));
            m_currentConnection->setEnd(end);
        }
        
        QList<QGraphicsItem*> items = scene()->items(event->scenePos());
        
        QGraphicsItem* item = 0;
        ConnectorItem* connectorItem = 0;
        
        foreach(item, items)
        {
            if(connectorItem = qgraphicsitem_cast<ConnectorItem*>(item))
                break;
        }
            
        if(connectorItem)
        {
            if(connectorType() != connectorItem->connectorType())
                m_currentConnection->setActive(true);
            else
                m_currentConnection->setActive(false);
        }
        else
        {
            m_currentConnection->setActive(false);
        }
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
