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
    
    /** 
     * Creates an action which sets the zoom of the view to 100%. If \c parent is 
     * not null the action is created as a child of \c parent.
     */
    QAction* createResetZoomAction(QObject* parent = 0);

protected:
    virtual void mousePressEvent(QMouseEvent *event);
    virtual void mouseMoveEvent(QMouseEvent *event);
    virtual void mouseReleaseEvent(QMouseEvent *event);
    virtual void wheelEvent(QWheelEvent *event);
    virtual void contextMenuEvent(QContextMenuEvent *event);
    /** Sets the current center point of the view in scene coordinates. */
    void setCenter(const QPointF center);
    /** Returns the current center point of the view in scene coordinates. */
    QPointF getCenter() const {return m_currentCenter;}
    
private slots:
    /** Resets the current view to the original view when document was opened */
    void resetZoomSize();
    
private:
    QPoint m_lastPanPos;
    QPointF m_currentCenter;
};

#endif // GRAPHICSVIEW_H
