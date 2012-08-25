#include "DataVisualizer.h"

#include <stromx/core/Image.h>
#include <stromx/core/Primitive.h>
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
        m_items[pos] = 0;
}

void DataVisualizer::moveLayer(int src, int dest)
{
    if(! m_items.contains(src))
        return;
    
    QGraphicsItem* item = m_items[src];
    
    if(m_items.contains(dest))
        delete m_items[dest];
    m_items[dest] = item;
    
    if(item)
        item->setZValue(-dest);
    
    m_items.remove(src);
}

void DataVisualizer::removeLayer(int pos)
{
    if(m_items.contains(pos))
    {
        delete m_items[pos];
        m_items.remove(pos);
    }
}

void DataVisualizer::setActive(int pos, bool active)
{
    if(! m_items.contains(pos))
        return;  

    // get the item of this layer
    QGraphicsItem* item = m_items[pos];
    
    if(! item)
        return;
    
    item->setVisible(active);
}

void DataVisualizer::setColor(int pos, const QColor& color)
{
    if(! m_items.contains(pos))
        return;  

    // get the item of this layer
    QGraphicsItem* item = m_items[pos];
    
    if(! item)
        return;
    
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

void DataVisualizer::setData(int pos, const stromx::core::Data& data, Visualization visualization)
{
    using namespace stromx::core;
    
    if(! m_items.contains(pos))
        return;
    
    delete m_items[pos];
    m_items[pos] = 0;
    
    QList<QGraphicsItem*> items;
    if(data.isVariant(DataVariant::IMAGE))
    {
        items = createImageItems(data);
    }
    else if(data.isVariant(DataVariant::UINT_32))
    {
        items = createPrimitiveItems<UInt32>(data);
    }
    else if(data.isVariant(DataVariant::MATRIX))
    {
        if(visualization == LINE_SEGMENT)
            items = createLineSegmentItems(data);
    }
    
    foreach(QGraphicsItem* item, items)
    {
        scene()->addItem(item);
        item->setZValue(-pos);
        m_items[pos] = item;
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




