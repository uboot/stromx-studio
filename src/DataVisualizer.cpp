#include "DataVisualizer.h"

#include <stromx/core/Image.h>
#include <stromx/core/Primitive.h>
#include <stromx/core/String.h>
#include <QGraphicsObject>
#include "InputModel.h"

DataVisualizer::DataVisualizer(QWidget* parent)
  : GraphicsView(parent)
{
    setScene(new QGraphicsScene(this));
}

void DataVisualizer::addLayer(int pos)
{
    if(! m_items.contains(pos))
        m_items[pos] = ItemList();
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
        // set the activation status of each valid item
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

void DataVisualizer::setData(int pos, const stromx::core::Data& data, Visualization visualization)
{
    using namespace stromx::core;
    
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
        m_items[pos] = createImageItems(data);
    }
    else if(data.isVariant(DataVariant::UINT_32))
    {
        m_items[pos] = createPrimitiveItems<UInt32>(data);
    } 
    if(data.isVariant(DataVariant::STRING))
    {
        m_items[pos] = createStringItems(data);
    }
    else if(data.isVariant(DataVariant::MATRIX))
    {
        if(visualization == LINE_SEGMENT)
            m_items[pos] = createLineSegmentItems(data);
    }
    
    // add the items and set their z-value
    foreach(QGraphicsItem* item, m_items[pos])
    {
        scene()->addItem(item);
        item->setZValue(-pos);
    }
}

QList<QGraphicsItem*> DataVisualizer::createImageItems(const stromx::core::Data& data)
{
    using namespace stromx::core;
    
    QList<QGraphicsItem*> items;
    try
    {
        const Image & image = data_cast<const Image &>(data);
        QImage::Format format;
        
        bool validPixelType = true;
        
        switch(image.pixelType())
        {
        case Image::MONO_8:
        case Image::BAYERBG_8:
        case Image::BAYERGB_8:
            format = QImage::Format_Indexed8;
            break;
        case Image::RGB_24:
        case Image::BGR_24:
            format = QImage::Format_RGB888;
            break;
        default:
            validPixelType = false;
        }
        
        if(validPixelType)
        {
            QImage qtImage(image.data(), image.width(), image.height(), image.stride(), format);
            QVector<QRgb> colorTable(256);
            for(unsigned int i = 0; i < 256; ++i)
                colorTable[i] = qRgb(i, i, i);
            qtImage.setColorTable(colorTable);
            QPixmap pixmap(QPixmap::fromImage(qtImage));
            items.append(new QGraphicsPixmapItem(pixmap));
        }
    }
    catch(BadCast&)
    {
    }
    
    return items;
}
   
QList<QGraphicsItem*> DataVisualizer::createStringItems(const stromx::core::Data & data)
{
    using namespace stromx::core;
    
    QList<QGraphicsItem*> items;
    try
    {
        const String & string = stromx::core::data_cast<const String &>(data);
        QGraphicsItem* item = new QGraphicsSimpleTextItem(QString::fromStdString(string));
        items.append(item);
    }
    catch(stromx::core::BadCast&)
    {
    }
    
    return items;
}


QList< QGraphicsItem* > DataVisualizer::createLineSegmentItems(const stromx::core::Data& data)
{
    using namespace stromx::core;
    
    QList<QGraphicsItem*> items;
    try
    {
        const Matrix & matrix = data_cast<const Matrix &>(data);
        
        if(matrix.valueType() == Matrix::DOUBLE && matrix.cols() == 4)
        {
            const uint8_t* rowPtr = matrix.data();
            for(unsigned int i = 0; i < matrix.rows(); ++i)
            {
                double* rowData = (double*)(rowPtr);
                items.append(new QGraphicsLineItem(rowData[0], rowData[1], rowData[2], rowData[3]));
                rowPtr += matrix.stride();
            }
        }
    }
    catch(BadCast&)
    {
    }
    
    return items;
}




