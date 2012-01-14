/* 
*  Copyright 2011 Matthias Fuchs
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

#ifndef OPERATORITEM_H
#define OPERATORITEM_H

#include <QGraphicsObject>

class QGraphicsRectItem;
class ConnectionItem;
class ConnectorItem;
class OperatorModel;

class OperatorItem : public QGraphicsObject
{
    Q_OBJECT
    
public:
    explicit OperatorItem(OperatorModel* op, QGraphicsItem * parent = 0);
    
    enum { Type = UserType + 1 };
    virtual int type() const { return Type; }
    
    OperatorModel* op() const { return m_op; }
    
    virtual QRectF boundingRect() const;
    virtual void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget);
    
    void addInputConnection(unsigned int id, ConnectionItem* connection);
    void addOutputConnection(unsigned int id, ConnectionItem* connection);
    void removeConnection(ConnectionItem* connection);
    
public slots:
    void setInitialized(bool value);
    
protected:
    void mouseMoveEvent(QGraphicsSceneMouseEvent* event);
    
private:
    void initialize();
    void deinitialize();
    
    OperatorModel* m_op;
    QGraphicsRectItem* m_opRect;
    
    QMap<unsigned int, ConnectorItem*> m_inputs;
    QMap<unsigned int, ConnectorItem*> m_outputs;
};

#endif // OPERATORITEM_H
