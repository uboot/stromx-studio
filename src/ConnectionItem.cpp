#include "ConnectionItem.h"

#include <QGraphicsSceneContextMenuEvent>
#include <QGraphicsView>
#include <QMenu>
#include <QPen>
#include <QVector2D>
#include <math.h>
#include "ConnectionModel.h"
#include "ConnectorItem.h"
#include "SelectionModel.h"
#include "StreamEditorScene.h"

const qreal ConnectionItem::EXTRA_HEIGHT = 20;
const qreal ConnectionItem::PI = 3.141592;
const qreal ConnectionItem::EPSILON = 0.1;
    
ConnectionItem::ConnectionItem(ConnectionModel* model, QGraphicsItem* parent)
  : QGraphicsObject(parent),
    m_path(new QGraphicsPathItem(this)),
    m_startArrow(0),
    m_endArrow(0),
    m_centerArrow(0),
    m_model(model),
    m_occupied(false)
{
    Q_ASSERT(model);
    
    setFlag(ItemIsSelectable, true);
    
    m_pen.setWidthF(ConnectorItem::SIZE);
    m_pen.setColor(m_model->color());
    connect(m_model, SIGNAL(colorChanged(QColor)), this, SLOT(setColor(QColor)));
    
    m_startArrow = createDoubleArrow(this);
    m_endArrow = createDoubleArrow(this);
    m_centerArrow = createDoubleArrow(this);
    
    update();
}

QRectF ConnectionItem::boundingRect() const
{
    return m_path->boundingRect();
}

QPainterPath ConnectionItem::shape() const
{
    return m_path->shape();
}

void ConnectionItem::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
{
}

QVariant ConnectionItem::itemChange(GraphicsItemChange change, const QVariant &value)
{
    if(change == ItemSelectedHasChanged)
    {
        if(isSelected())
            m_pen.setStyle(Qt::DotLine);
        else
            m_pen.setStyle(Qt::SolidLine);

        update();
        
        return value;
    }

    return QGraphicsItem::itemChange(change, value);
}

void ConnectionItem::setStart(const QPointF& start)
{
    m_start = start;
    update();
}

void ConnectionItem::setEnd(const QPointF& end)
{
    m_end = end;
    update();
}

void ConnectionItem::setColor(const QColor& color)
{
    m_pen.setColor(color);
    update();
}

void ConnectionItem::update()
{
    QPointF start(m_start.x() + ConnectorItem::SIZE, m_start.y());
    QPointF end(m_end.x() - ConnectorItem::SIZE, m_end.y());
    
    QPainterPath path = drawPath(start, end);
    m_path->setPath(path);
    m_path->setPen(m_pen);
    
    updateArrowPositions(start, end);
    
    QBrush brush;
    if(m_occupied)
        brush = QBrush(m_pen.color().darker(130));
    else
        brush = QBrush(m_pen.color().lighter(130));
       
    m_startArrow->setBrush(brush);
    m_endArrow->setBrush(brush);
    m_centerArrow->setBrush(brush);
}

QPainterPath ConnectionItem::drawPath(const QPointF& start, const QPointF& end)
{
    const qreal RADIUS = 1.5 * ConnectorItem::SIZE;
    const qreal ARC_RECT_SIZE = 2*RADIUS;
    QRectF arcRect(0, 0, ARC_RECT_SIZE, ARC_RECT_SIZE);
    qreal xDiff = end.x() - start.x();
    qreal yDiff = end.y() - start.y();
    qreal yOffset = 0;
    
    if(yDiff <= 0)
        yOffset = yDiff;
    else
        yOffset = 0;
    
    QPainterPath path;
    
    path.moveTo(start);
    if(xDiff > 0)
    {
        // the connections points forward
        if(xDiff > 2*RADIUS)
        {
            // start and end are so far from each other that the can
            // be directly connected (without loop)
            if(yDiff > 0)
            {
                // the connection points downwards
                if(fabs(yDiff) < 2*RADIUS)
                {
                    // start and end point are so close too each other that
                    // the arcs must be less than 90 degrees
                    qreal angle = computeAngle(RADIUS, yDiff/2);
                    qreal width = computeWidth(yDiff/2, angle);
                    
                    arcRect.moveTo(start.x() + xDiff/2 - width - RADIUS, start.y());
                    path.arcTo(arcRect, 90, -angle);
                    arcRect.moveTo(start.x() + xDiff/2 + width - RADIUS, end.y() - ARC_RECT_SIZE);
                    path.arcTo(arcRect, -90 - angle, angle);
                }
                else
                {
                    // two 90 degree arcs are possible
                    arcRect.moveTo(start.x() + xDiff/2 - ARC_RECT_SIZE, start.y());
                    path.arcTo(arcRect, 90, -90);
                    arcRect.moveTo(start.x() + xDiff/2, end.y() - ARC_RECT_SIZE);
                    path.arcTo(arcRect, 180, 90);
                }
            }
            else
            {
                // the connection points upwards
                if(fabs(yDiff) < 2*RADIUS)
                {
                    // start and end point are so close too each other that
                    // the arcs must be less than 90 degrees
                    qreal angle = computeAngle(RADIUS, yDiff/2);
                    qreal width = computeWidth(yDiff/2, angle);
                    
                    arcRect.moveTo(start.x() + xDiff/2 - width - RADIUS, start.y() - ARC_RECT_SIZE);
                    path.arcTo(arcRect, 270, angle);
                    arcRect.moveTo(start.x() + xDiff/2 + width - RADIUS, end.y());
                    path.arcTo(arcRect, 90 + angle, -angle);
                }
                else
                {
                    // two 90 degree arcs are possible
                    arcRect.moveTo(start.x() + xDiff/2 - ARC_RECT_SIZE, start.y() - ARC_RECT_SIZE);
                    path.arcTo(arcRect, 270, 90);
                    arcRect.moveTo(start.x() + xDiff/2, end.y());
                    path.arcTo(arcRect, 180, -90);
                }
            }
        }
        else
        {
            // start and end are so close to each other to each other
            // that the connections must have loop
            arcRect.moveTo(start.x() + xDiff/2 - RADIUS, start.y() - ARC_RECT_SIZE);
            path.arcTo(arcRect, -90, 90);
            arcRect.moveTo(start.x() + xDiff/2 - RADIUS, start.y() + yOffset - 2*RADIUS);
            path.arcTo(arcRect, 0, 90);
            arcRect.moveTo(start.x() + xDiff/2 - RADIUS, start.y() + yOffset - 2*RADIUS);
            path.arcTo(arcRect, 90, 90);
            arcRect.moveTo(start.x() + xDiff/2 - RADIUS, end.y() - ARC_RECT_SIZE);
            path.arcTo(arcRect, 180, 90);
        }
    }
    else
    {
        // the connections points backward
        if(fabs(yDiff) > 4 * RADIUS)
        {            
            // start and end are so far from each other that the can
            // be directly with a line between the to operators
            if(yDiff > 0)
            {
                // the connection points downwards
                arcRect.moveTo(start.x() - RADIUS, start.y());
                path.arcTo(arcRect, 90, -90);
                arcRect.moveTo(start.x() - RADIUS, start.y() + yDiff/2 - ARC_RECT_SIZE);
                path.arcTo(arcRect, 0, -90);
                arcRect.moveTo(end.x() - RADIUS, start.y() + yDiff/2);
                path.arcTo(arcRect, 90, 90);
                arcRect.moveTo(end.x() - RADIUS, end.y() - ARC_RECT_SIZE);
                path.arcTo(arcRect, 180, 90);
            }
            else
            {
                // the connection points upwards
                arcRect.moveTo(start.x() - RADIUS, start.y() - ARC_RECT_SIZE);
                path.arcTo(arcRect, 270, 90);
                arcRect.moveTo(start.x() - RADIUS, start.y() + yDiff/2);
                path.arcTo(arcRect, 0, 90);
                arcRect.moveTo(end.x() - RADIUS, start.y() + yDiff/2 - ARC_RECT_SIZE);
                path.arcTo(arcRect, 270, -90);
                arcRect.moveTo(end.x() - RADIUS, end.y());
                path.arcTo(arcRect, 180, -90);
            }
        }
        else
        {
            // start and end are so close to each other to each other
            // that the connection must run around one of the operators
            arcRect.moveTo(start.x() - RADIUS, start.y() - ARC_RECT_SIZE);
            path.arcTo(arcRect, -90, 90);
            arcRect.moveTo(start.x() - RADIUS, start.y() - ARC_RECT_SIZE + yOffset - EXTRA_HEIGHT);
            path.arcTo(arcRect, 0, 90);
            arcRect.moveTo(end.x() - RADIUS, start.y() - ARC_RECT_SIZE + yOffset - EXTRA_HEIGHT);
            path.arcTo(arcRect, 90, 90);
            arcRect.moveTo(end.x() - RADIUS, end.y() - ARC_RECT_SIZE);
            path.arcTo(arcRect, 180, 90);
        }
    }
    path.lineTo(end);
    
    return path;
}

void ConnectionItem::updateArrowPositions(const QPointF& start, const QPointF& end)
{
    static const qreal ARROW_LENGTH = 2 * ConnectorItem::SIZE;
    const qreal RADIUS = 1.5 * ConnectorItem::SIZE;
    const qreal ARC_RECT_SIZE = 2*RADIUS;
    QRectF arcRect(0, 0, ARC_RECT_SIZE, ARC_RECT_SIZE);
    qreal xDiff = end.x() - start.x();
    qreal yDiff = end.y() - start.y();
    qreal yOffset = 0;
    
    
    if(yDiff <= 0)
        yOffset = yDiff;
    else
        yOffset = 0;
    
    // hide all arrows
    m_startArrow->setVisible(false);
    m_endArrow->setVisible(false);
    m_centerArrow->setVisible(false);
    
    if(xDiff > 0) // the connections points forward
    {
        if(xDiff > 2*RADIUS)
        {
            // start and end are so far from each other that the can
            // be directly connected (without loop)
            if(xDiff > 2*RADIUS + 2*ARROW_LENGTH) 
            {
                m_startArrow->setVisible(true);
                m_startArrow->setPos(start.x() + (xDiff - 2*RADIUS)/4, start.y());
                m_startArrow->setRotation(0);
                
                m_endArrow->setVisible(true);
                m_endArrow->setPos(end.x() - (xDiff - 2*RADIUS)/4, end.y());
                m_endArrow->setRotation(0);
            }
            
            if(fabs(yDiff) > 2*RADIUS + ARROW_LENGTH)
            {
                // start and end point are so far from each other that a
                // center arrow can be drawn
                m_centerArrow->setVisible(true);
                m_centerArrow->setPos(start.x() + xDiff/2, start.y() + yDiff/2);
                
                if(yDiff > 0) // the connection points downwards
                    m_centerArrow->setRotation(90);
                else // the connection points upwards
                    m_centerArrow->setRotation(-90);
            }
        }
        else
        {
            // start and end are so close to each other to each other
            // that the connections must have loop
            if(fabs(yDiff) > ARROW_LENGTH)
            {
                if(yOffset < 0)
                {
                    m_startArrow->setVisible(true);
                    m_startArrow->setPos(start.x() + xDiff/2 + RADIUS, start.y() - RADIUS + yDiff/2);
                    m_startArrow->setRotation(-90);
                }
                else
                {
                    m_endArrow->setVisible(true);
                    m_endArrow->setPos(start.x() + xDiff/2 - RADIUS, start.y() - RADIUS + yDiff/2);
                    m_endArrow->setRotation(90);
                }
            }
        }
    }
    else // the connections points backward
    {
        // the backward section is long enough to display an arrow
        if(fabs(xDiff) > ARROW_LENGTH)
        {
            m_centerArrow->setVisible(true);
            m_centerArrow->setRotation(180);
        }
        
        if(fabs(yDiff) > 4*RADIUS)
        {            
            // start and end are so far from each other that the can
            // be directly with a line between the to operators
            if(fabs(yDiff) > 4*RADIUS + 2*ARROW_LENGTH)
            {
                m_startArrow->setVisible(true);
                m_endArrow->setVisible(true);
                m_startArrow->setPos(start.x() + RADIUS, start.y() + yDiff/4);
                m_endArrow->setPos(end.x() - RADIUS, end.y() - yDiff/4);
                if(yDiff > 0)
                {
                    m_startArrow->setRotation(90);
                    m_endArrow->setRotation(90);
                }
                else
                {
                    m_startArrow->setRotation(-90);
                    m_endArrow->setRotation(-90);
                }
            }
            
            m_centerArrow->setPos(start.x() + xDiff/2, start.y() + yDiff/2);
        }
        else
        {
            // start and end are so close to each other to each other
            // that the connection must run around one of the operators
            m_startArrow->setVisible(true);
            m_endArrow->setVisible(true);
            m_startArrow->setPos(start.x() + RADIUS, start.y() + (yOffset - EXTRA_HEIGHT)/2 - RADIUS);
            m_endArrow->setPos(end.x() - RADIUS, end.y() - RADIUS - ((yDiff - yOffset) + EXTRA_HEIGHT)/2);
            m_startArrow->setRotation(-90);
            m_endArrow->setRotation(90);
            
            m_centerArrow->setPos(start.x() + xDiff/2, start.y() + yOffset - EXTRA_HEIGHT - ARC_RECT_SIZE);
        }
    }
}

QGraphicsPathItem* ConnectionItem::createDoubleArrow(QGraphicsItem* parent)
{
    const qreal WIDTH = 0.5 * ConnectorItem::SIZE;
    
    QPainterPath path;
    
    // draw the first single arrow
    path.moveTo(-1.5 * WIDTH, 0.0);
    path.lineTo(-2.0 * WIDTH, -WIDTH);
    path.lineTo(-WIDTH, -WIDTH);
    path.lineTo(-0.5 * WIDTH, 0.0);
    path.lineTo(-WIDTH, WIDTH);
    path.lineTo(-2.0 * WIDTH, WIDTH);
    path.closeSubpath();
    
    // the second one
    path.moveTo(0.5 * WIDTH, 0.0);
    path.lineTo(0, -WIDTH);
    path.lineTo(WIDTH, -WIDTH);
    path.lineTo(1.5 * WIDTH, 0.0);
    path.lineTo(WIDTH, WIDTH);
    path.lineTo(0 * WIDTH, WIDTH);
    path.closeSubpath();
    
    QGraphicsPathItem* item = new QGraphicsPathItem(path, parent);
    item->setPen(QPen(Qt::NoPen));
    
    return item;
}

void ConnectionItem::setOccupied(bool occupied)
{
    m_occupied = occupied;
    update();
}

qreal ConnectionItem::computeAngle(qreal radius, qreal height)
{
    qreal c = fabs(height)/fabs(radius);
    qreal sin = sqrt(c/2);
    return 2 * asin(sin) * 180 / PI;
}

qreal ConnectionItem::computeWidth(qreal height, qreal angle)
{
    if(fabs(height) < EPSILON)
        return 0.0;
    else
        return fabs(height) / tan(angle / 2 / 180 * PI);
}

void ConnectionItem::contextMenuEvent(QGraphicsSceneContextMenuEvent* event)
{
    if(isSelected())
    {
        QGraphicsScene* graphicsScene = scene();
        
        if(StreamEditorScene* streamScene = dynamic_cast<StreamEditorScene*>(graphicsScene))
        {
            QMenu menu;
            QList<QAction*> actions = streamScene->selectionModel()->createThreadActions(&menu);
            
            foreach(QAction* action, actions)
                menu.addAction(action);
            
            menu.exec(event->screenPos());
        }    
    }
}




