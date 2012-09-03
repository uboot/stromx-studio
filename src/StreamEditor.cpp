#include "StreamEditor.h"

#include <cmath>
#include <QApplication>
#include <QGraphicsView>
#include <QMouseEvent>
#include "ConnectionItem.h"
#include "ConnectionModel.h"
#include "ConnectorItem.h"
#include "InputData.h"
#include "StreamEditorScene.h"
#include "StreamModel.h"

StreamEditor::StreamEditor(QWidget* parent)
  : GraphicsView(parent),
    m_targetOp(0),
    m_inputId(0)
{
    m_scene = new StreamEditorScene(this);
    
    m_scene->setSceneRect(0, 0, 512, 512);
    
    setCenter(QPointF(256.0, 256.0));
    setScene(m_scene);
    
    setAcceptDrops(true);
    setDragMode(QGraphicsView::RubberBandDrag);
    setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);
}

void StreamEditor::mouseMoveEvent(QMouseEvent* event)
{
    if(event->buttons() & Qt::LeftButton && m_targetOp)
    {
        int distance = (event->pos() - m_startPos).manhattanLength();
        
        if(distance >= QApplication::startDragDistance())
            startDrag();
        
        return;
    }
    
    if(event->buttons() & Qt::MidButton)
    {
        if(!m_lastPanPos.isNull())
        {
            QPointF deltaMove = mapToScene(m_lastPanPos) - mapToScene(event->pos());
            m_lastPanPos = event->pos();
            
            setCenter(getCenter() + deltaMove);
        }
    }
    
    QGraphicsView::mouseMoveEvent(event);        
}

void StreamEditor::mousePressEvent(QMouseEvent* event)
{
    if(event->button() == Qt::LeftButton)
    {
        QPointF scenePos = mapToScene(event->pos());
        QGraphicsItem* item = scene()->itemAt(scenePos);
        
        if(item)
        {    
            if(ConnectionItem* connection = qgraphicsitem_cast<ConnectionItem*>(item->parentItem()))
            {
                m_startPos = event->pos();
                m_targetOp = connection->model()->targetOp();
                m_inputId = connection->model()->inputId();
            }
            else if(ConnectorItem* connector = qgraphicsitem_cast<ConnectorItem*>(item))
            {
                if(connector->connectorType() == ConnectorItem::INPUT
                    && connector->numConnections())
                {
                    m_startPos = event->pos();
                    m_targetOp = connector->op();
                    m_inputId = connector->id();
                }
                else
                {
                    m_targetOp = 0;
                    m_inputId = 0;
                }    
            }
            else
            {
                m_targetOp = 0;
                m_inputId = 0;
            }
        }
        else
        {
            m_targetOp = 0;
            m_inputId = 0;
        }
    }
    else
    {
        m_targetOp = 0;
        m_inputId = 0;
    }
    
    if (event->button() == Qt::MidButton)
    {
        setCursor(Qt::ClosedHandCursor);
        m_lastPanPos = event->pos(); 
    }
    else
    QGraphicsView::mousePressEvent(event);
}
void StreamEditor::mouseReleaseEvent(QMouseEvent *event)
{
    if(event->button() == Qt::MidButton)
    {
        setCursor(Qt::ArrowCursor);
        m_lastPanPos = QPoint();
    }   
}

void StreamEditor::wheelEvent(QWheelEvent* event)
{
    double numDegrees = -event->delta()/8.0;
    double numSteps = numDegrees/15.0;
    double factor = pow(1.125,numSteps);
    scale(factor,factor);
}


void StreamEditor::startDrag()
{    
    // allocate a input data object
    InputData* data = new InputData(m_targetOp, m_inputId);
    
    QList<QGraphicsView*> views = scene()->views();
    if(views.count() != 1)
        return;
    QWidget* currentWidget = views[0];
    
    QDrag* drag = new QDrag(currentWidget);
    drag->setMimeData(data);
    drag->exec(Qt::CopyAction, Qt::CopyAction);
    
    m_targetOp = 0;
    m_inputId = 0;
}

void StreamEditor::setCenter(const QPointF center)
{
    // Get the visible area of QGraphicsView 
    QRectF visibleArea = mapToScene(rect()).boundingRect();
    // Get the area covered by QGraphicsScene 
    QRectF sceneArea = m_scene->sceneRect();
    
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

