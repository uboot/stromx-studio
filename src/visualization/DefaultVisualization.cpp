#include "visualization/DefaultVisualization.h"

#include <stromx/runtime/Image.h>
#include <stromx/runtime/Primitive.h>
#include <stromx/runtime/String.h>

#include <QBrush>
#include <QGraphicsLineItem>
#include <QPen>

#include "visualization/ColorChooser.h"

namespace
{
    template <class data_t>
    static QList<QGraphicsItem*> createPrimitiveItemsTemplate(const stromx::runtime::Data & data,
        const VisualizationState::Properties & properties)
    {
        QList<QGraphicsItem*> items;
        QVariant colorVariant = properties.value("color", Qt::black);
        QColor color = colorVariant.value<QColor>();
        
        try
        {
            // cast to the concrete primitive type
            const data_t & primitive = stromx::runtime::data_cast<data_t>(data);
            
            // format it as a astring and create a graphics item from the string
            QGraphicsSimpleTextItem* item = new QGraphicsSimpleTextItem(QString("%1").arg(primitive));
            
            QBrush brush = item->brush();
            brush.setColor(color);
            item->setBrush(brush);
            
            // pack the graphics item into a list
            items.append(item);
        }
        catch(stromx::runtime::BadCast&)
        {
        }
        
        return items;
    }
}

VisualizationWidget* DefaultVisualization::createEditor() const
{
    return new ColorChooser;
}

QList< QGraphicsItem* > DefaultVisualization::createItems(const stromx::runtime::Data & data,
        const VisualizationState::Properties & properties) const
{
    using namespace stromx::runtime;
    
    QList<QGraphicsItem*> items;
    
    if(data.isVariant(DataVariant::IMAGE))
    {
        items = createImageItems(data, properties);
    }
    else if(data.isVariant(DataVariant::PRIMITIVE))
    {
        items = createPrimitiveItems(data, properties);
    } 
    else if(data.isVariant(DataVariant::STRING))
    {
        items = createStringItems(data, properties);
    }
    
    return items;
}

QList<QGraphicsItem*> DefaultVisualization::createImageItems(const stromx::runtime::Data& data,
    const VisualizationState::Properties & /*properties*/)
{
    using namespace stromx::runtime;
    
    QList<QGraphicsItem*> items;
    try
    {
        const Image & image = data_cast<Image>(data);
        QImage::Format format;
        
        bool validPixelType = true;
        
        switch(image.pixelType())
        {
        case Image::MONO_8:
        case Image::BAYERBG_8:
        case Image::BAYERGB_8:
        case Image::MONO_16:
            format = QImage::Format_Indexed8;
            break;
        case Image::RGB_24:
        case Image::BGR_24:
        case Image::RGB_48:
        case Image::BGR_48:
            format = QImage::Format_RGB888;
            break;
        default:
            validPixelType = false;
        }
        
        QImage qtImage;
        QVector<QRgb> colorTable(256);
        for(unsigned int i = 0; i < 256; ++i)
            colorTable[i] = qRgb(i, i, i);
        qtImage.setColorTable(colorTable);
        
        if(validPixelType)
        {
            switch(image.pixelType())
            {
                case Image::MONO_16:                   
                    {
                        // loop over all pixels and divide it by 256
                        qtImage = QImage(image.width(), image.height(), format);
                        const uint8_t* rowPtrSrc = image.data();
                        for(unsigned int i = 0; i < image.rows(); ++i)
                        {
                            const uint16_t* pixelPtrSrc = reinterpret_cast<const uint16_t*>(rowPtrSrc);
                            uchar* pixelPtrDst = qtImage.scanLine(i);
                            for(unsigned int j = 0; j < image.cols(); ++j)
                            {
                                pixelPtrDst[j] = (*pixelPtrSrc)/256;
                                ++pixelPtrSrc;
                            }
                            rowPtrSrc += image.stride();
                        }
                        break;
                    }
                case Image::RGB_48:
                case Image::BGR_48:
                {
                    //loop over all pixels separated by color and divide each channel by 256
                    qtImage = QImage(image.width(), image.height(), format);
                    const uint8_t* rowPtrSrc = image.data();
                    const uint16_t* pixelPtrSrc = reinterpret_cast<const uint16_t*>(rowPtrSrc);
                    for(unsigned int i = 0; i < image.height(); ++i)
                    {                     
                        uchar* rowPtrDst = qtImage.scanLine(i);
                        uint8_t* pixelPtrDst = reinterpret_cast<uint8_t*>(rowPtrDst);
                        for(unsigned int j = 0; j < image.width(); ++j)
                        {
                            for(unsigned int iChannel = 0; iChannel < 3; ++iChannel)
                            {
                                *pixelPtrDst = (*pixelPtrSrc)/256;
                                ++pixelPtrSrc;
                                ++pixelPtrDst;
                            }
                        }
                        rowPtrSrc += image.stride();
                    }
                    break;
                }
                    
                default:
                    {
                        qtImage = QImage(image.data(), image.width(), image.height(), image.stride(), format);
                    }
            }

            QPixmap pixmap = QPixmap::fromImage(qtImage);
            items.append(new QGraphicsPixmapItem(pixmap));
        }
    }
    catch(BadCast&)
    {
    }
    
    return items;
}
   
QList<QGraphicsItem*> DefaultVisualization::createStringItems(const stromx::runtime::Data & data,
    const VisualizationState::Properties & properties)
{
    using namespace stromx::runtime;
    QVariant colorVariant = properties.value("color", Qt::black);
    QColor color = colorVariant.value<QColor>();
    
    QList<QGraphicsItem*> items;
    try
    {
        const String & string = stromx::runtime::data_cast<String>(data);
        QGraphicsSimpleTextItem* item = new QGraphicsSimpleTextItem(QString::fromStdString(string));
        QBrush brush = item->brush();
        brush.setColor(color);
        item->setBrush(brush);
        items.append(item);
    }
    catch(stromx::runtime::BadCast&)
    {
    }
    
    return items;
}

QList<QGraphicsItem*> DefaultVisualization::createPrimitiveItems(const stromx::runtime::Data & data,
    const VisualizationState::Properties & properties)
{    
    using namespace stromx::runtime;
    
    if(data.isVariant(DataVariant::BOOL))
        return createPrimitiveItemsTemplate<Bool>(data, properties);
    else if(data.isVariant(DataVariant::INT_8))
        return createPrimitiveItemsTemplate<Int8>(data, properties);
    else if(data.isVariant(DataVariant::UINT_8))
        return createPrimitiveItemsTemplate<UInt8>(data, properties);
    else if(data.isVariant(DataVariant::INT_16))
        return createPrimitiveItemsTemplate<Int16>(data, properties);
    else if(data.isVariant(DataVariant::UINT_16))
        return createPrimitiveItemsTemplate<UInt16>(data, properties);
    else if(data.isVariant(DataVariant::INT_32))
        return createPrimitiveItemsTemplate<Int32>(data, properties);
    else if(data.isVariant(DataVariant::UINT_32))
        return createPrimitiveItemsTemplate<UInt32>(data, properties);
    else if(data.isVariant(DataVariant::FLOAT_32))
        return createPrimitiveItemsTemplate<Float32>(data, properties);
    else if(data.isVariant(DataVariant::FLOAT_64))
        return createPrimitiveItemsTemplate<Float64>(data, properties);
    else
        return QList<QGraphicsItem*>();
}

