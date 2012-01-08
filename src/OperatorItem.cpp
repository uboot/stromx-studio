#include "OperatorItem.h"

#include <QPen>
#include <stromx/core/Operator.h>
#include "ConnectorItem.h"
#include "OperatorModel.h"

OperatorItem::OperatorItem(OperatorModel* op, QGraphicsItem * parent)
  : QGraphicsObject(parent),
    m_op(op)
{
    m_opRect = new QGraphicsRectItem(this);
    m_opRect->setRect(0, 0, 50, 50);
    setPos(m_op->pos());
    
    QGraphicsTextItem* label = new QGraphicsTextItem(this);
    label->setPlainText(QString::fromStdString(m_op->op()->info().type()));
    
    setFlag(ItemIsMovable, true);
    setFlag(ItemIsSelectable, true);
    setFlag(ItemIsFocusable, true);
    
    connect(op, SIGNAL(initializedChanged(bool)), this, SLOT(setInitialized(bool)));
}

QRectF OperatorItem::boundingRect() const
{
    return m_opRect->boundingRect();
}

void OperatorItem::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
{
    QPen currentPen = m_opRect->pen();
    
    if(isSelected())
        currentPen.setStyle(Qt::DashLine);
    else
        currentPen.setStyle(Qt::SolidLine);
        
    m_opRect->setPen(currentPen);
    
    m_opRect->paint(painter, option, widget);
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
    DescriptionVector inputs = m_op->op()->info().inputs();
    
    unsigned int i = 0;
    for(DescriptionVector::iterator iter = inputs.begin();
        iter != inputs.end();
        ++iter, ++i)
    {
        ConnectorItem* inputItem = new ConnectorItem(this);
        inputItem->setPos(0, i * 10);
        
        m_inputs.append(inputItem);
    }
    
    DescriptionVector outputs = m_op->op()->info().outputs();
    i = 0;
    for(DescriptionVector::iterator iter = outputs.begin();
        iter != outputs.end();
        ++iter, ++i)
    {
        ConnectorItem* outputItem = new ConnectorItem(this);
        outputItem->setPos(40, i * 10);
        
        m_outputs.append(outputItem);
    }
}

void OperatorItem::deinitialize()
{
}
