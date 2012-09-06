/* 
*  Copyright 2012 Thomas Fidler
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

#ifndef GRAPHICSVIEW_H
#define GRAPHICSVIEW_H

#include <QGraphicsView>

/** 
 * \brief Extension of the Qt graphics viewer.
 *
 * This class extends QGraphicsView and provides slots to control the
 * current zoom and display position.
 */
class GraphicsView : public QGraphicsView
{   
    Q_OBJECT
    
public:
    /** Constructs a graphics view. */
    explicit GraphicsView(QWidget* parent = 0);
    
    /** Returns the current zoom value. */
    qreal zoom() const { return 1.0; }
    
    /** 
     * Returns the scene coordinates of the scene point in the upper left
     * corner of the view.
     */
    QPointF viewPos() const { return QPointF(); }

protected:
    virtual void mousePressEvent(QMouseEvent *event);
    virtual void mouseMoveEvent(QMouseEvent *event);
    virtual void mouseReleaseEvent(QMouseEvent *event);
    virtual void wheelEvent(QWheelEvent *event);
    /** Sets the current center point of the view in scene coordinates. */
    void setCenter(const QPointF center);
    /** Returns the current center point of the view in scene coordinates. */
    QPointF getCenter() const {return m_currentCenter;}

public slots:
    /** Sets the current zoom. */
    void setZoom(qreal value);
    
    /**
     * Scrolls the view such that the upper left corner of the view corresponds
     * to the scene point \c p.
     */
    void setViewPos(const QPointF &p);
    
private:
    QPoint m_lastPanPos;
    QPointF m_currentCenter;
};

#endif // GRAPHICSVIEW_H
