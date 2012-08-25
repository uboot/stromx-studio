#include "DataVisualizer.h"

#include <stromx/core/Data.h>
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
    
    QGraphicsItem* item = 0;
    if(data.isVariant(DataVariant::IMAGE))
    {
        try
        {
            const Image & image = data_cast<const Image &>(data);
            QImage::Format format;
            
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
                return;
            }
            
            QImage qtImage(image.data(), image.width(), image.height(), image.stride(), format);
            QVector<QRgb> colorTable(256);
            for(unsigned int i = 0; i < 256; ++i)
                colorTable[i] = qRgb(i, i, i);
            qtImage.setColorTable(colorTable);
            QPixmap pixmap(QPixmap::fromImage(qtImage));
            item = scene()->addPixmap(pixmap);
        }
        catch(BadCast&)
        {
        }
    }
    else if(data.isVariant(DataVariant::UINT_32))
    {
        try
        {
            const UInt32 & uint = data_cast<const UInt32 &>(data);
            QGraphicsSimpleTextItem* textItem = scene()->addSimpleText(QString("%1").arg(uint));
            item = textItem;
        }
        catch(BadCast&)
        {
        }
    }
    
    if(item)
    {
        item->setZValue(-pos);
        m_items[pos] = item;
    }
}



