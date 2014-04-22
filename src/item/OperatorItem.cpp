#include "item/OperatorItem.h"

#include <QGraphicsScene>
#include <QGraphicsSceneContextMenuEvent>
#include <QMenu>
#include <QMouseEvent>
#include <QPen>
#include <QUndoStack>
#include <stromx/runtime/Operator.h>
#include "StreamEditorScene.h"
#include "item/ConnectionItem.h"
#include "item/ConnectorItem.h"


const qreal OperatorItem::SIZE = 55;
const qreal OperatorItem::RADIUS = 3;
const qreal OperatorItem::WIDTH = 1.5;
const qreal OperatorItem::CONNECTOR_OFFSET = 5;
const qreal OperatorItem::LABEL_OFFSET = 5;
const qreal OperatorItem::CONNECTION_Z_OFFSET = 1.0/100.0;
    
OperatorItem::OperatorItem(OperatorModel* model, QGraphicsItem * parent)
  : QGraphicsObject(parent),
    m_model(model)
{
    QPainterPath path;
    path.addRoundedRect(QRectF(-SIZE/2, -SIZE/2, SIZE, SIZE), RADIUS, RADIUS);
    m_opRect = new QGraphicsPathItem(path, this);
    QPen pen = m_opRect->pen();
    pen.setWidthF(WIDTH);
    m_opRect->setPen(pen);
    m_opRect->setBrush(Qt::white);
    setPos(m_model->pos());
    
    m_label = new QGraphicsTextItem(this);
    m_label->setPlainText(QString::fromStdString(m_model->op()->name()));
    m_label->setPos(-m_label->boundingRect().width()/2, SIZE/2 + LABEL_OFFSET);
    
    setFlag(ItemIsMovable, true);
    setFlag(ItemIsSelectable, true);
    setFlag(ItemIsFocusable, true);
    setFlag(ItemSendsScenePositionChanges, true);
    
    setInitialized(model->isInitialized());
    
    connect(m_model, SIGNAL(nameChanged(QString)), this, SLOT(setName(QString)));
    connect(m_model, SIGNAL(initializedChanged(bool)), this, SLOT(setInitialized(bool)));
    connect(m_model, SIGNAL(posChanged(QPointF)), this, SLOT(setOperatorPos(QPointF)));
    connect(m_model, SIGNAL(connectorOccupiedChanged(OperatorModel::ConnectorType,uint,bool)),
            this, SLOT(setConnectorOccupied(OperatorModel::ConnectorType,uint,bool)));
    connect(m_model, SIGNAL(activeChanged(bool)), this, SLOT(resetAllConnectors()));
}

QRectF OperatorItem::boundingRect() const
{
    return m_opRect->boundingRect();
}

void OperatorItem::paint(QPainter* /*painter*/, const QStyleOptionGraphicsItem* /*option*/,
                         QWidget* /*widget*/)
{
}

QVariant OperatorItem::itemChange(GraphicsItemChange change, const QVariant &value)
{
    if(change == ItemSelectedHasChanged)
    {
        QPen currentPen = m_opRect->pen();

        if(isSelected())
            currentPen.setStyle(Qt::DashLine);
        else
            currentPen.setStyle(Qt::SolidLine);

        m_opRect->setPen(currentPen);
        
        return value;
    }
    if(change == ItemPositionHasChanged)
    {
        updateConnectionPositions();
    }

    return QGraphicsItem::itemChange(change, value);
}

void OperatorItem::contextMenuEvent(QGraphicsSceneContextMenuEvent* event)
{
    if(isSelected())
    {
        QGraphicsScene* graphicsScene = scene();
        
        if(StreamEditorScene* streamScene = qobject_cast<StreamEditorScene*>(graphicsScene))
        {
            QMenu menu;
            QAction* initializeAction = streamScene->createInitializeAction(&menu);
            QAction* deinitializeAction = streamScene->createDeinitializeAction(&menu);
            
            menu.addAction(initializeAction);
            menu.addAction(deinitializeAction);
            menu.exec(event->screenPos());
        }    
    }
}

void OperatorItem::setInitialized(bool value)
{
    if(value == true)
        initialize();
    else
        deinitialize();
}

void OperatorItem::initialize()
{
    typedef std::vector<const stromx::runtime::Description*> DescriptionVector;
    
    DescriptionVector inputs = m_model->op()->info().inputs();
    qreal firstInputYPos = computeFirstYPos(inputs.size());
    qreal yOffset = ConnectorItem::SIZE + CONNECTOR_OFFSET;
    qreal inputXPos = -SIZE/2 - ConnectorItem::SIZE/2;
    
    unsigned int i = 0;
    for(DescriptionVector::iterator iter = inputs.begin();
        iter != inputs.end();
        ++iter, ++i)
    {
        ConnectorItem* inputItem = new ConnectorItem(this->m_model, (*iter)->id(), ConnectorItem::INPUT, this);
        inputItem->setPos(inputXPos, firstInputYPos + i*yOffset);
        
        m_inputs[(*iter)->id()] = inputItem;
    }
    
    DescriptionVector outputs = m_model->op()->info().outputs();
    qreal firstOutputYPos = computeFirstYPos(outputs.size());
    qreal outputXPos = SIZE/2 + ConnectorItem::SIZE/2;
    
    i = 0;
    for(DescriptionVector::iterator iter = outputs.begin();
        iter != outputs.end();
        ++iter, ++i)
    {
        ConnectorItem* outputItem = new ConnectorItem(this->m_model, (*iter)->id(), ConnectorItem::OUTPUT, this);
        outputItem->setPos(outputXPos, firstOutputYPos + i*yOffset);
        
        m_outputs[(*iter)->id()] = outputItem;
    }
}

void OperatorItem::deinitialize()
{
    QMapIterator<unsigned int, ConnectorItem*> inputIter(m_inputs);
    while (inputIter.hasNext())
    {
        inputIter.next();
        delete inputIter.value();
    }
    m_inputs.clear();
    
    QMapIterator<unsigned int, ConnectorItem*> outputIter(m_outputs);
    while (outputIter.hasNext())
    {
        outputIter.next();
        delete outputIter.value();
    }
    m_outputs.clear();
}

void OperatorItem::addInputConnection(unsigned int id, ConnectionItem* connection)
{
    m_inputs[id]->addConnection(connection);
}

void OperatorItem::addOutputConnection(unsigned int id, ConnectionItem* connection)
{
    m_outputs[id]->addConnection(connection);
    
    // move the output connection slightly in front of the operator
    connection->setZValue(zValue() + m_outputs.count() * CONNECTION_Z_OFFSET);
}

void OperatorItem::removeConnection(ConnectionItem* connection)
{
    QMapIterator<unsigned int, ConnectorItem*> inputIter(m_inputs);
    while (inputIter.hasNext())
    {
        inputIter.next();
        inputIter.value()->removeConnection(connection);
    }
    
    QMapIterator<unsigned int, ConnectorItem*> outputIter(m_outputs);
    while (outputIter.hasNext())
    {
        outputIter.next();
        outputIter.value()->removeConnection(connection);
    }
}

void OperatorItem::updateConnectionPositions()
{
    QMapIterator<unsigned int, ConnectorItem*> inputIter(m_inputs);
    while (inputIter.hasNext())
    {
        inputIter.next();
        inputIter.value()->updateConnectionPositions();
    }
    
    QMapIterator<unsigned int, ConnectorItem*> outputIter(m_outputs);
    while (outputIter.hasNext())
    {
        outputIter.next();
        outputIter.value()->updateConnectionPositions();
    }
}

void OperatorItem::mouseReleaseEvent(QGraphicsSceneMouseEvent* event)
{
    // check if the item was moved
    if(model()->pos() != pos())
    {
        model()->undoStack()->beginMacro("move objects");
        
        foreach(QGraphicsItem* item, scene()->selectedItems())
        {
            if(OperatorItem* opItem = qgraphicsitem_cast<OperatorItem*>(item))
                opItem->model()->setPos(opItem->pos());
        }
        
        model()->undoStack()->endMacro();
    }
    
    QGraphicsObject::mouseReleaseEvent(event);
}

void OperatorItem::setOperatorPos(const QPointF& value)
{
    setPos(value);
    updateConnectionPositions();
}

void OperatorItem::setConnectorOccupied(OperatorModel::ConnectorType type, unsigned int id, bool occupied)
{
    if(type == OperatorModel::INPUT)
    {
        if(m_inputs.contains(id))
            m_inputs[id]->setOccupied(occupied);
    }
    else
    {
        if(m_outputs.contains(id))
            m_outputs[id]->setOccupied(occupied);
    }
}

void OperatorItem::setName(const QString& value)
{
    m_label->setPlainText(value);
    m_label->setPos(-m_label->boundingRect().width()/2, SIZE/2 + LABEL_OFFSET);
}

void OperatorItem::resetAllConnectors()
{ 
    QMapIterator<unsigned int, ConnectorItem*> input(m_inputs);
    while (input.hasNext())
    {
        input.next();
        input.value()->setOccupied(false);
    }
    
    QMapIterator<unsigned int, ConnectorItem*> output(m_outputs);
    while (output.hasNext())
    {
        output.next();
        output.value()->setOccupied(false);
    }
}

qreal OperatorItem::computeFirstYPos(int numConnectors)
{
    if(numConnectors <= 0)
        return 0;
    
    qreal totalHeight = (numConnectors - 1) * (ConnectorItem::SIZE + CONNECTOR_OFFSET);
    return -totalHeight / 2;
}

void OperatorItem::mousePressEvent(QGraphicsSceneMouseEvent* event)
{
    event->setButton(Qt::LeftButton);
    QGraphicsObject::mousePressEvent(event);
}





