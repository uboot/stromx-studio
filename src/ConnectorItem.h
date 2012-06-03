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

/** 
 * \brief Graphical representation of a connector of an operator.
 */
class ConnectorItem : public QGraphicsRectItem
{
public:
    /** The length of the side of the connector square. */
    static const qreal SIZE;
    
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
    
    /** Adds a connection to the connector. */
    void addConnection(ConnectionItem* connection);
    
    /** Removes a connection from the connector. */
    void removeConnection(ConnectionItem* connection);
    
    /** 
     * Updates the geometry of the connections of this connector
     * to the current position of the connector. Depending on the
     * type of the connector either the start or end point of the 
     * connection item is changed such that it matches the current
     * position of the connector.
     */
    void updateConnectionPositions() const;
    
    /**
     * Marks this connector as either occupied or unoccupied.
     * An connector is occupied if data is present at the operator
     * input or output which is represented by this connector.
     * The visual representation of an occupied connector is different
     * from that of an unoccupied.
     */
    void setOccupied(bool occupied);

protected:
    virtual void mousePressEvent(QGraphicsSceneMouseEvent * event);
    virtual void mouseMoveEvent(QGraphicsSceneMouseEvent * event);
    virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent * event);
    virtual void hoverEnterEvent(QGraphicsSceneHoverEvent * event);
    virtual void hoverLeaveEvent(QGraphicsSceneHoverEvent * event);
    
private:
    /** Z-value of the connector labels which should float above all other items. */
    static const qreal FLOAT_Z_VALUE;
    
    /** 
     * Returns the connector at the position \c pos of the scene of t
     * this connector. Returns 0 if there is no connector at that position.
     */
    ConnectorItem* connectorItemAt(const QPointF & pos) const;
    
    /**
     * Returns true if this connector can be connected to. Connections can only
     * be made if the current stream is inactive. Also not more than on connection
     * can be connected to an input (outputs can connect to several connections.
     */
    bool canConnect() const;
    
    /** 
     * Updates the geometry of \c connection to the current position of
     * the connector. Depending on the type of the connector either the
     * start or end point of \c connection is changed such that it matches
     * the current position of the connector.
     */
    void updateConnectionPosition(ConnectionItem* connection) const;
    
    /** Returns the text to display in the tool tip of the connector. */
    QString connectorToolTip() const;
    
    OperatorModel* m_op;
    unsigned int m_id;
    ConnectorType m_connectorType;
    ArrowItem* m_currentArrow;
    QSet<ConnectionItem*> m_connections;
};

#endif // CONNECTORITEM_H
