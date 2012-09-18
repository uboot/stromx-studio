#include <QMouseEvent>
#include <QScrollBar>
#include <QMenu>
#include "GraphicsView.h"
#include <cmath>
#include <QObject>

GraphicsView::GraphicsView(QWidget* parent)
  : QGraphicsView(parent)
{

}

void GraphicsView::mouseMoveEvent(QMouseEvent* event)
{   
    if(event->buttons() & Qt::MidButton)
    {
        if(!m_lastPanPos.isNull())
        {
            QPointF deltaMove = mapToScene(m_lastPanPos) - mapToScene(event->pos());
            m_lastPanPos = event->pos();
            
            setCenter(getCenter() + deltaMove);
        }
    }
    else
        QGraphicsView::mouseMoveEvent(event);        
}

void GraphicsView::mousePressEvent(QMouseEvent* event)
{
   if (event->button() == Qt::MidButton)
    {
        setCursor(Qt::ClosedHandCursor);
        m_lastPanPos = event->pos();
        m_currentCenter = mapToScene(viewport()->contentsRect().center());
        
        QRectF visibleArea = mapToScene(rect()).boundingRect();
        QRectF sceneArea = sceneRect();
        if (visibleArea.contains(sceneArea))
            setCursor(Qt::ForbiddenCursor);
    }
    
    QGraphicsView::mousePressEvent(event);
}

void GraphicsView::mouseReleaseEvent(QMouseEvent *event)
{
    if(event->button() == Qt::MidButton)
    {
        setCursor(Qt::ArrowCursor);
        m_lastPanPos = QPoint();
    }
    
    QGraphicsView::mouseReleaseEvent(event);
}

void GraphicsView::wheelEvent(QWheelEvent* event)
{
    setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
    double numDegrees = -event->delta()/8.0;
    double numSteps = numDegrees/15.0;
    double factor = pow(1.125,numSteps);
    scale(factor,factor);
    
    m_currentCenter = mapToScene(viewport()->contentsRect().center());
    setCenter(m_currentCenter);
}

void GraphicsView::contextMenuEvent(QContextMenuEvent* event)
{
    QMenu contextMenu;
    QAction *resetViewAction = new QAction("Zoom to original size", &contextMenu);
    resetViewAction->setEnabled(!matrix().isIdentity());
    connect(resetViewAction,SIGNAL(triggered(bool)),this,SLOT(resetZoomSize()));
    
    contextMenu.addAction(resetViewAction);
    contextMenu.exec(event->globalPos());
    
    QGraphicsView::contextMenuEvent(event);
}


void GraphicsView::setCenter(const QPointF center)
{
    // Get the visible area of QGraphicsView 
    QRectF visibleArea = mapToScene(rect()).boundingRect();
    // Get the area covered by QGraphicsScene 
    QRectF sceneArea = sceneRect();
    
    // Create trusted region (rectangle) to contain a valid center point
    // based on the margins of visibleArea and sceneArea
    // rect() in Qt: top left point + width and height
    qreal trustedXPoint = visibleArea.width() / 2.0;
    qreal trustedYPoint = visibleArea.height() / 2.0;
    qreal trustedWidth = sceneArea.width() - visibleArea.width();
    qreal trustedHeight = sceneArea.height() - visibleArea.height();
    
    QRectF trustedRegion(trustedXPoint,trustedYPoint,trustedWidth,trustedHeight);
    
    if(trustedRegion.contains(center))
    {
        // Center is valid; entire visibleArea is contained in sceneArea
        m_currentCenter = center;
    }
    else
    {
        if(visibleArea.contains(sceneArea))
        {
            // Special case #1: visibleArea contains sceneArea, i.e. the
            //                  entire scene can be seen -> use center of 
            //                  the scene as new center
            m_currentCenter = sceneArea.center();    
        }
        else
        {
            // Special case #2: Invalid center: intersection of visibleArea
            // with sceneArea is not equal to visibleArea and not equal to 
            // sceneArea, i.e. visibleArea is shifted outside of sceneArea
            // Solution: move visibleArea as much as possible in the desired
            // direction indicated by center
            
            m_currentCenter = center;
            
            if(center.x() > trustedRegion.x() + trustedRegion.width())
            {
                m_currentCenter.setX(trustedRegion.x() + trustedRegion.width());
            }
            else
            {
                if(center.x() < trustedRegion.x())
                {
                    m_currentCenter.setX(trustedRegion.x());
                }
            }
                
            if(center.y() > trustedRegion.y() + trustedRegion.height())
            {
                m_currentCenter.setY(trustedRegion.y() + trustedRegion.height());
            }
            else
            {
                if(center.y() < trustedRegion.y())
                {
                    m_currentCenter.setY(trustedRegion.y());
                }
            }    
        }
    }

    centerOn(m_currentCenter);
}

void GraphicsView::resetZoomSize()
{
    resetMatrix();
}

