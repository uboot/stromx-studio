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

class ConnectionItem;

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
    
    explicit ConnectorItem(ConnectorType type, QGraphicsItem * parent = 0);
    
    ConnectorType connectorType() const { return m_connectorType; }

protected:
    virtual void mousePressEvent(QGraphicsSceneMouseEvent * event);
    virtual void mouseMoveEvent(QGraphicsSceneMouseEvent * event);
    virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent * event);
    
private:
    
    ConnectorType m_connectorType;
    ConnectionItem* m_currentConnection;
};

#endif // CONNECTORITEM_H
