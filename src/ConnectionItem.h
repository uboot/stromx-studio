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

#include <QGraphicsObject>
#include <QPen>

class ConnectionModel;

/** \brief Graphical representation of a connection model. */
class ConnectionItem : public QGraphicsObject
{
    Q_OBJECT
    
public:
    /** Constructs a connection item for a connection model. */
    explicit ConnectionItem(ConnectionModel* model, QGraphicsItem * parent = 0);
    
    enum { Type = UserType + 2 };
    virtual int type() const { return Type; }
    
    void setStart(const QPointF & start);
    void setEnd(const QPointF & end);
    void setActive(bool value);
    
    virtual QRectF boundingRect() const;
    virtual void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget);
    
    ConnectionModel* model() const { return m_model; }
    
protected:
    /** Updates the appearance of the connection when it is de-/selected. */
    virtual QVariant itemChange(GraphicsItemChange change, const QVariant & value);
    
    virtual QPainterPath shape() const;
    
private slots:
    /** Sets the color of the connection. */
    void setColor(const QColor & color);
    
private:
    enum Width
    {
        ACTIVE_WIDTH = 4,
        INACTIVE_WIDTH = 2
    };
    
    /** 
     * Rotates the head of the connection arrow according to 
     * the angle of the connection shaft.
     */
    void rotateHead();
    
    /** Applies the current pen to all graphic items of the connection. */
    void applyPen();
    
    QGraphicsLineItem* m_shaft;
    QGraphicsLineItem* m_head1;
    QGraphicsLineItem* m_head2;
    
    QPointF m_startPos;
    ConnectionModel* m_model;
    QPen m_pen;
};

#endif // CONNECTIONITEM_H