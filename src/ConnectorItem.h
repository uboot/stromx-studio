/* 
*  Copyright 2012 Matthias Fuchs
*
*  This file is part of stromx-studio.
*
*  Stromx-studio is free software: you can redistribute it and/or modify
*  it under the terms of the GNU General Public License as published by
*  the Free Software Foundation, either version 3 of the License, or
*  (at your option) any later version.
*
*  Stromx-studio is distributed in the hope that it will be useful,
*  but WITHOUT ANY WARRANTY; without even the implied warranty of
*  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
*  GNU General Public License for more details.
*
*  You should have received a copy of the GNU General Public License
*  along with stromx-studio.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef CONNECTORITEM_H
#define CONNECTORITEM_H

#include <QGraphicsRectItem>
#include <QSet>

class ArrowItem;
class ConnectionItem;
class OperatorModel;

/** \brief Graphical representation of a connector of an operator. */
class ConnectorItem : public QGraphicsRectItem
{
public:
    enum ConnectorType
    {
        INPUT,
        OUTPUT
    };
    
    enum { Type = UserType + 3 };
    virtual int type() const { return Type; }
    
    explicit ConnectorItem(OperatorModel* op, unsigned int id, ConnectorType type, QGraphicsItem * parent = 0);
    
    OperatorModel* op() const { return m_op; }
    unsigned int id() const { return m_id; }
    ConnectorType connectorType() const { return m_connectorType; }
    
    /** Returns the number of connections connected to this connector. */
    int numConnections() const;
    
    void addConnection(ConnectionItem* connection);
    void removeConnection(ConnectionItem* connection);
    void updateConnectionPositions() const;
    void setOccupied(bool occupied);

protected:
    virtual void mousePressEvent(QGraphicsSceneMouseEvent * event);
    virtual void mouseMoveEvent(QGraphicsSceneMouseEvent * event);
    virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent * event);
    virtual void hoverEnterEvent(QGraphicsSceneHoverEvent * event);
    virtual void hoverLeaveEvent(QGraphicsSceneHoverEvent * event);
    
private:
    ConnectorItem* connectorItemAt(const QPointF & pos) const;
    bool canConnect() const;
    void updateConnectionPosition(ConnectionItem* connection) const;
    
    OperatorModel* m_op;
    unsigned int m_id;
    ConnectorType m_connectorType;
    ArrowItem* m_currentArrow;
    QSet<ConnectionItem*> m_connections;
    QGraphicsTextItem* m_label;
};

#endif // CONNECTORITEM_H
