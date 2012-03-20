#include "DataVisualizer.h"

#include <stromx/core/Data.h>
#include <stromx/core/Image.h>
#include <stromx/core/Primitive.h>
#include <QGraphicsObject>

DataVisualizer::DataVisualizer(QWidget* parent)
  : QGraphicsView(parent)
{
    setScene(new QGraphicsScene);
}

void DataVisualizer::addLayer(int layer)
{
    if(layer >= 0 && layer <= m_items.count())
        m_items.insert(layer, 0);
    else
        Q_ASSERT(false);
    
    reorderItems();
}

void DataVisualizer::moveLayer(int src, int dest)
{
    QGraphicsItem* item = 0;
    if(src >= 0 && src < m_items.count())
        item = m_items[src];
    else
        Q_ASSERT(false);
    
    if(dest >= 0 && dest <= m_items.count())
        m_items.insert(dest, item);
    else
        Q_ASSERT(false);
    
    m_items.removeAt(src);
    
    reorderItems();
}

void DataVisualizer::removeLayer(int layer)
{
    if(layer >= 0 && layer < m_items.count())
    {
        delete m_items[layer];
        m_items.removeAt(layer);
    }
    else
    {
        Q_ASSERT(false);
    }
    
    reorderItems();
}

void DataVisualizer::setActive(int layer, bool active)
{

}

void DataVisualizer::setAlpha(int layer, int alpha)
{

}

void DataVisualizer::setColor(int layer, const QColor& color)
{

}

void DataVisualizer::setData(int layer, const stromx::core::Data& data)
{
    using namespace stromx::core;
    
    if(layer >= 0 && layer < m_items.count())
    {
        if(m_items[layer])
        {
            delete m_items[layer];
            m_items[layer] = 0;
        }
    }
    else
    {
        Q_ASSERT(false);
    }
    
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
                format = QImage::Format_Indexed8;
                break;
            case Image::RGB_24:
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
            textItem->setBrush(Qt::red);
            item = textItem;
        }
        catch(BadCast&)
        {
        }
    }
    
    if(item)
    {
        item->setZValue(-layer);
        m_items[layer] = item;
    }
}

void DataVisualizer::reorderItems()
{
    int layer = 0;
    foreach(QGraphicsItem* item, m_items)
    {
        if(item)
            item->setZValue(-layer);
        layer++;
    }
}


