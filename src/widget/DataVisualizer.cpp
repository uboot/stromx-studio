#include "widget/DataVisualizer.h"

#include "model/InputModel.h"
#include "widget/DataVisualizerUtilities.h"
#include <QGraphicsItem>
#include <stromx/runtime/Primitive.h>

DataVisualizer::DataVisualizer(QWidget* parent)
  : GraphicsView(parent)
{
    setScene(new QGraphicsScene(this));
}

void DataVisualizer::addLayer(int pos)
{
    if(! m_items.contains(pos))
        m_items[pos] = QList<QGraphicsItem*>();
}

void DataVisualizer::moveLayer(int src, int dest)
{
    // return if the source layer does not exist
    if(! m_items.contains(src))
        return;
    
    // nothing to do if source and destination are the same
    if(src == dest)
        return;
    
    // if the destination layer exists delete all items there
    if(m_items.contains(dest))
    {
        foreach(QGraphicsItem* item, m_items[dest])
            delete item;
    }
    
    // copy the source items to the destination layer
    m_items[dest] = m_items[src];
    
    // adapt the z-value
    foreach(QGraphicsItem* item, m_items[dest])
    {
        if(item)
            item->setZValue(-dest);
    }
    
    // remove the source layer
    m_items.remove(src);
}

void DataVisualizer::removeLayer(int pos)
{
    if(m_items.contains(pos))
    {
        // if the layer exists delete all its items
        foreach(QGraphicsItem* item, m_items[pos])
            delete item;
        
        // remove the layer
        m_items.remove(pos);
    }
}

void DataVisualizer::setActive(int pos, bool active)
{
    // return  if the layer does not exist
    if(! m_items.contains(pos))
        return;  

    
    foreach(QGraphicsItem* item, m_items[pos])
    {
        // set the visibility status of each item
        if(item)
            item->setVisible(active);
    }
}

void DataVisualizer::setColor(int pos, const QColor& color)
{
    // return  if the layer does not exist
    if(! m_items.contains(pos))
        return;  

    // set the color of each valid item
    foreach(QGraphicsItem* item, m_items[pos])
    {
        if(item)
        {  
            switch(item->type())
            {
            case QGraphicsSimpleTextItem::Type:
                if(QGraphicsSimpleTextItem* textItem = qgraphicsitem_cast<QGraphicsSimpleTextItem*>(item))
                    textItem->setBrush(color);
                break;
            case QGraphicsLineItem::Type:
                if(QGraphicsLineItem* lineItem = qgraphicsitem_cast<QGraphicsLineItem*>(item))
                    lineItem->setPen(color);
                break;
            default:
                ;
            }
        }
    }
}

void DataVisualizer::setData(int pos, const stromx::runtime::Data& data, Visualization visualization)
{
    using namespace stromx::runtime;
    
    // return  if the layer does not exist
    if(! m_items.contains(pos))
        return;
    
    // if the layer exists delete all items there and empty the layer
    if(m_items.contains(pos))
    {
        foreach(QGraphicsItem* item, m_items[pos])
            delete item;
    }
    m_items[pos].clear();
    
    // create the graphic items representing the stromx data
    if(data.isVariant(DataVariant::IMAGE))
    {
        m_items[pos] = DataVisualizerUtilities::createImageItems(data, visualization);
    }
    else if(data.isVariant(DataVariant::PRIMITIVE))
    {
        m_items[pos] = DataVisualizerUtilities::createPrimitiveItems(data, visualization);
    } 
    if(data.isVariant(DataVariant::STRING))
    {
        m_items[pos] = DataVisualizerUtilities::createStringItems(data, visualization);
    }
    else if(data.isVariant(DataVariant::MATRIX))
    {
        m_items[pos] = DataVisualizerUtilities::createMatrixItems(data, visualization);
    }
    
    // add the items and set their z-value
    foreach(QGraphicsItem* item, m_items[pos])
    {
        scene()->addItem(item);
        item->setZValue(-pos);
    }
}





