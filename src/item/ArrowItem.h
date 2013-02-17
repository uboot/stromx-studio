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

#ifndef ARROWITEM_H
#define ARROWITEM_H

#include <QGraphicsObject>
#include <QPen>

class ConnectionModel;

/** \brief An arrow used to draw connections with the mouse. */
class ArrowItem : public QGraphicsObject
{
    Q_OBJECT
    
public:
    /** Constructs an arrow item. */
    explicit ArrowItem(QGraphicsItem * parent = 0);
    
    enum { Type = UserType + 2 };
    virtual int type() const { return Type; }
    
    /** Sets the start point of the arrow in scene coordinates. */
    void setStart(const QPointF & start);
    
    /** Sets the end point of the arrow in scene coordinates. */
    void setEnd(const QPointF & end);
    
    /** 
     * Activates or deactivates the arrow. Arrows should be activated
     * if the connect valid start point to an end point (i.e. the user moves
     * the mouse pointer to a valid connector while dragging an arrow).
     * Activated arrows are drawn bold.
     */
    void setActive(bool value);
    
    virtual QRectF boundingRect() const;
    virtual void paint(QPainter* /*painter*/, const QStyleOptionGraphicsItem* /*option*/,
                       QWidget* /*widget*/) {}
    
private:
    enum Width
    {
        /** The width of an active arrow. */
        ACTIVE_WIDTH = 4,
        
        /** The width of an inactive arrow. */
        INACTIVE_WIDTH = 2
    };
    
    /** 
     * Rotates the head of the arrow according to 
     * the angle of the connection shaft.
     */
    void rotateHead();
    
    /** Applies the current pen to all graphic items of the arrow. */
    void applyPen();
    
    QGraphicsLineItem* m_shaft;
    QGraphicsLineItem* m_head1;
    QGraphicsLineItem* m_head2;
    
    QPen m_pen;
};

#endif // ARROWITEM_H