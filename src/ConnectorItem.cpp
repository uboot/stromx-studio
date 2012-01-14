#include "ConnectorItem.h"

#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include "ConnectionItem.h"
#include "StreamEditorScene.h"
#include "StreamModel.h"

ConnectorItem::ConnectorItem(OperatorModel* op, unsigned int id, ConnectorType type, QGraphicsItem* parent)
  : QGraphicsRectItem(parent),
    m_op(op),
    m_id(id),
    m_connectorType(type),
    m_currentConnection(0)
{
    setRect(0, 0, 10, 10);
}

void ConnectorItem::mousePressEvent(QGraphicsSceneMouseEvent* event)
{
    m_currentConnection = new ConnectionItem(0, this);
    if(m_connectorType == INPUT)
    {
        m_currentConnection->setStart(mapFromScene(event->scenePos()));
        m_currentConnection->setEnd(QPointF(5, 5));
    }
    else
    {
        m_currentConnection->setStart(QPointF(5, 5));
        m_currentConnection->setEnd(mapFromScene(event->scenePos()));
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
        
        ConnectorItem* connectorItem = connectorItemAt(event->scenePos());
            
        if(connectorItem)
            m_currentConnection->setActive(true);
        else
            m_currentConnection->setActive(false);
    }
}

ConnectorItem* ConnectorItem::connectorItemAt(const QPointF& pos) const
{
    QList<QGraphicsItem*> items = scene()->items(pos);
    
    QGraphicsItem* item = 0;
    ConnectorItem* connectorItem = 0;
    
    foreach(item, items)
    {
        if(connectorItem = qgraphicsitem_cast<ConnectorItem*>(item))
            if(connectorType() != connectorItem->connectorType())
                return connectorItem;
    }
    
    return 0;
}

void ConnectorItem::mouseReleaseEvent(QGraphicsSceneMouseEvent* event)
{
    if(m_currentConnection)
    {
        delete m_currentConnection;
        m_currentConnection = 0;
    }
    
    ConnectorItem* connectorItem = connectorItemAt(event->scenePos());
        
    if(connectorItem)
    {
        StreamEditorScene* streamEditorScene = qobject_cast<StreamEditorScene*>(scene());
        
        if(streamEditorScene)
        {
            StreamModel* model = streamEditorScene->model();
            if(connectorType() == INPUT)
                model->addConnection(connectorItem->op(), connectorItem->id(), op(), id());
            else
                model->addConnection(op(), id(), connectorItem->op(), connectorItem->id());
        }
    }
}

void ConnectorItem::addConnection(ConnectionItem* connection)
{
    m_connections.insert(connection);
    updateConnectionPosition(connection);
}

void ConnectorItem::removeConnection(ConnectionItem* connection)
{
    m_connections.remove(connection);
}

void ConnectorItem::updateConnectionPositions() const
{
    ConnectionItem* connection = 0;
    foreach(connection, m_connections)
        updateConnectionPosition(connection);
}

void ConnectorItem::updateConnectionPosition(ConnectionItem* connection) const
{
    if(connectorType() == INPUT)
        connection->setEnd(mapToScene(QPointF(5, 5)));
    else
        connection->setStart(mapToScene(QPointF(5, 5)));
}

