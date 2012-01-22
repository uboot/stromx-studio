#include "OperatorItem.h"

#include <QGraphicsScene>
#include <QPen>
#include <stromx/core/Operator.h>
#include "ConnectorItem.h"
#include "OperatorModel.h"
#include "StreamEditorScene.h"

OperatorItem::OperatorItem(OperatorModel* model, QGraphicsItem * parent)
  : QGraphicsObject(parent),
    m_model(model)
{
    m_opRect = new QGraphicsRectItem(this);
    m_opRect->setRect(0, 0, 50, 50);
    setPos(m_model->pos());
    
    QGraphicsTextItem* label = new QGraphicsTextItem(this);
    label->setPos(0, 55);
    label->setPlainText(QString::fromStdString(m_model->op()->info().type()));
    
    setFlag(ItemIsMovable, true);
    setFlag(ItemIsSelectable, true);
    setFlag(ItemIsFocusable, true);
    setFlag(ItemSendsScenePositionChanges, true);
    
    connect(m_model, SIGNAL(initializedChanged(bool)), this, SLOT(setInitialized(bool)));
    connect(m_model, SIGNAL(posChanged(QPointF)), this, SLOT(setOperatorPos(QPointF)));
}

QRectF OperatorItem::boundingRect() const
{
    return m_opRect->boundingRect();
}

void OperatorItem::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
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

void OperatorItem::setInitialized(bool value)
{
    if(value == true)
        initialize();
    else
        deinitialize();
}

void OperatorItem::initialize()
{
    typedef std::vector<const stromx::core::Description*> DescriptionVector;
    DescriptionVector inputs = m_model->op()->info().inputs();
    
    unsigned int i = 0;
    for(DescriptionVector::iterator iter = inputs.begin();
        iter != inputs.end();
        ++iter, ++i)
    {
        ConnectorItem* inputItem = new ConnectorItem(this->m_model, (*iter)->id(), ConnectorItem::INPUT, this);
        inputItem->setPos(0, i * 10);
        
        m_inputs[(*iter)->id()] = inputItem;
    }
    
    DescriptionVector outputs = m_model->op()->info().outputs();
    i = 0;
    for(DescriptionVector::iterator iter = outputs.begin();
        iter != outputs.end();
        ++iter, ++i)
    {
        ConnectorItem* outputItem = new ConnectorItem(this->m_model, (*iter)->id(), ConnectorItem::OUTPUT, this);
        outputItem->setPos(40, i * 10);
        
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
    StreamEditorScene* streamScene = qobject_cast<StreamEditorScene*>(scene());
    
    // check if the item was moved
    if(model()->pos() != pos())
    {
        if(streamScene)
            streamScene->beginMacro("move objects");
        
        foreach(QGraphicsItem* item, scene()->selectedItems())
        {
            if(OperatorItem* opItem = qgraphicsitem_cast<OperatorItem*>(item))
                opItem->model()->setPos(opItem->pos());
        }
        
        if(streamScene)
            streamScene->endMacro();
    }
    
    QGraphicsObject::mouseReleaseEvent(event);
}

void OperatorItem::setOperatorPos(const QPointF& value)
{
    setPos(value);
    updateConnectionPositions();
}



