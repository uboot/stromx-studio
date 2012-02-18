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

#ifndef CONNECTIONITEM_H
#define CONNECTIONITEM_H

#include <QGraphicsLineItem>
#include <QPen>

class ConnectionModel;

class ConnectionItem : public QGraphicsLineItem
{
public:
    explicit ConnectionItem(ConnectionModel* model, QGraphicsItem * parent = 0);
    
    enum { Type = UserType + 2 };
    virtual int type() const { return Type; }
    
    void setStart(const QPointF & start);
    void setEnd(const QPointF & end);
    void setActive(bool value);
    
    ConnectionModel* model() const { return m_model; }
    
private:
    void rotateHead();
    
    QGraphicsLineItem* m_head1;
    QGraphicsLineItem* m_head2;
    
    QPointF m_startPos;
    ConnectionModel* m_model;
    QPen m_pen;
};

#endif // CONNECTIONITEM_H