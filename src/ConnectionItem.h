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
    
    /** Sets the start point of the connection in scene coordinates. */
    void setStart(const QPointF & start);
    
    /** Sets the end point of the connection in scene coordinates. */
    void setEnd(const QPointF & end);
    
    virtual QRectF boundingRect() const;
    virtual void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget);
    
    /** Returns the connection model of this item. */
    ConnectionModel* model() const { return m_model; }
    
public slots:  
    /**
     * Marks the input connector at this connection as either occupied or
     * unoccupied. An an input connector is occupied if data is present
     * and can be consumed by the operator.
     * The visual representation of an connection with occupied inputs is different
     * from that of an unoccupied.
     */
    void setInputOccupied(bool occupied);
    
    /**
     * Marks the output connector at this connection as either occupied or
     * unoccupied. An an output connector is occupied if data is present
     * and can be transported to the next operator.
     * The visual representation of an connection with occupied outputs is different
     * from that of an unoccupied.
     */
    void setOutputOccupied(bool occupied);
    
protected:
    /** Updates the appearance of the connection when it is de-/selected. */
    virtual QVariant itemChange(GraphicsItemChange change, const QVariant & value);
    
    virtual QPainterPath shape() const;
    
    virtual void contextMenuEvent(QGraphicsSceneContextMenuEvent * event);
    
    void mousePressEvent ( QGraphicsSceneMouseEvent * event );
    
private slots:
    /** Sets the color of the connection. */
    void setColor(const QColor & color);
    
private:
    /** 
     * The extra height added to backward point connection to loop around the
     * operator.
     */
    static const qreal EXTRA_HEIGHT;
    
    /** 3.141592 */
    static const qreal PI;
    
    /** Angles for height less than \c EPSILON are always 0. */
    static const qreal EPSILON;

    
    /**
     * Computes the angle of a arc of a given \c height.
     * The arc is assumed to start at 90 degrees.
     */
    static qreal computeAngle(qreal radius, qreal height);
    
    /**
     * Computes the width of the arc of given \c height and
     * \c angle. The arc is assumed to start at 90 degrees.
     */
    static qreal computeWidth(qreal height, qreal angle);
    
    /** Draws a path from \c start to \c end into \c path. */
    static QPainterPath drawPath(const QPointF & start, const QPointF & end);
    
    /** Returns a double arrow shape item as a child of \c parent. */
    static QGraphicsPathItem* createDoubleArrow(QGraphicsItem* parent);
    
    /** Updates the current geometry and pen settings of the connection. */
    void update();
    
    /** 
     * Moves and rotates the arrows of the connection such that the fit
     * the connection. This functions is called by update().
     */
    void updateArrowPositions(const QPointF & start, const QPointF & end);
    
    QGraphicsPathItem* m_path;
    QGraphicsPathItem* m_startArrow;
    QGraphicsPathItem* m_endArrow;
    QGraphicsPathItem* m_centerArrow;
    ConnectionModel* m_model;
    QPen m_pen;
    QPointF m_start;
    QPointF m_end;
    bool m_inputOccupied;
    bool m_outputOccupied;
};

#endif // CONNECTIONITEM_H