#include "widget/DataVisualizerUtilities.h"

#include <QPen>
#include <boost/concept_check.hpp>

#include <stromx/runtime/Image.h>
#include <stromx/runtime/Primitive.h>
#include <stromx/runtime/String.h>

namespace
{
    template <class data_t>
    static QList<QGraphicsItem*> createPrimitiveItemsTemplate(const stromx::runtime::Data & data,
        const AbstractDataVisualizer::VisualizationProperties & visualizationProperties)
    {
        QList<QGraphicsItem*> items;
        QVariant colorVariant = visualizationProperties.value("color", Qt::black);
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
    
    
    
    template <class data_t>
    QList< QGraphicsItem* > createLineSegmentItemsTemplate(const stromx::runtime::Data& data, 
        const AbstractDataVisualizer::VisualizationProperties & visualizationProperties)
    {
        using namespace stromx::runtime;
        QVariant colorVariant = visualizationProperties.value("color", Qt::black);
        QColor color = colorVariant.value<QColor>();
        
        QList<QGraphicsItem*> items;
        try
        {
            // cast the data to a matrix
            const Matrix & matrix = data_cast<Matrix>(data);
            
            // check if the value size of the matrix matches the size of the template
            // parameter and make sure the matrix has 4 columns
            if(matrix.valueSize() == sizeof(data_t) && matrix.cols() == 4)
            {
                // loop over the rows of the matrix and construct a line item from each row
                const uint8_t* rowPtr = matrix.data();
                for(unsigned int i = 0; i < matrix.rows(); ++i)
                {
                    const data_t* rowData = reinterpret_cast<const data_t*>(rowPtr);
                    QGraphicsLineItem* lineItem = 
                        new QGraphicsLineItem(rowData[0], rowData[1], rowData[2], rowData[3]);
                    QPen pen = lineItem->pen();
                    pen.setColor(color);
                    lineItem->setPen(pen);
                    items.append(lineItem);
                    rowPtr += matrix.stride();
                }
            }
        }
        catch(BadCast&)
        {
        }
        
        return items;
    }
    
    QList< QGraphicsItem* > createLineSegments(const stromx::runtime::Data& data, 
        const AbstractDataVisualizer::VisualizationProperties & visualizationProperties)
    {
        using namespace stromx::runtime;
        
        if(data.isVariant(DataVariant::INT_8_MATRIX))
            return createLineSegmentItemsTemplate<int8_t>(data, visualizationProperties);
        else if(data.isVariant(DataVariant::UINT_8_MATRIX))
            return createLineSegmentItemsTemplate<uint8_t>(data, visualizationProperties);
        else if(data.isVariant(DataVariant::INT_16_MATRIX))
            return createLineSegmentItemsTemplate<int16_t>(data, visualizationProperties);
        else if(data.isVariant(DataVariant::UINT_16_MATRIX))
            return createLineSegmentItemsTemplate<uint16_t>(data, visualizationProperties);
        else if(data.isVariant(DataVariant::INT_32_MATRIX))
            return createLineSegmentItemsTemplate<int32_t>(data, visualizationProperties);
        else if(data.isVariant(DataVariant::UINT_32_MATRIX))
            return createLineSegmentItemsTemplate<uint32_t>(data, visualizationProperties);
        else if(data.isVariant(DataVariant::FLOAT_32_MATRIX))
            return createLineSegmentItemsTemplate<float>(data, visualizationProperties);
        else if(data.isVariant(DataVariant::FLOAT_64_MATRIX))
            return createLineSegmentItemsTemplate<double>(data, visualizationProperties);
        else
            return QList<QGraphicsItem*>();
    }
}

QList<QGraphicsItem*> DataVisualizerUtilities::createImageItems(const stromx::runtime::Data& data,
    const AbstractDataVisualizer::VisualizationProperties & /*visualizationProperties*/)
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
   
QList<QGraphicsItem*> DataVisualizerUtilities::createStringItems(const stromx::runtime::Data & data,
    const AbstractDataVisualizer::VisualizationProperties & visualizationProperties)
{
    using namespace stromx::runtime;
    QVariant colorVariant = visualizationProperties.value("color", Qt::black);
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

QList< QGraphicsItem* > DataVisualizerUtilities::createMatrixItems(const stromx::runtime::Data& data,
    const AbstractDataVisualizer::VisualizationProperties & visualizationProperties)
{
    const QVariant visualizationVariant = 
        visualizationProperties.value(QString("visualization"), AbstractDataVisualizer::AUTOMATIC);
        
    switch(visualizationVariant.toInt())
    {
        case AbstractDataVisualizer::LINES:
            return createLineSegments(data, visualizationProperties);
        default:
            return QList<QGraphicsItem*>();
    }
    using namespace stromx::runtime;
}

QList<QGraphicsItem*> DataVisualizerUtilities::createPrimitiveItems(const stromx::runtime::Data & data,
    const AbstractDataVisualizer::VisualizationProperties & visualizationProperties)
{    
    using namespace stromx::runtime;
    
    if(data.isVariant(DataVariant::BOOL))
        return createPrimitiveItemsTemplate<Bool>(data, visualizationProperties);
    else if(data.isVariant(DataVariant::INT_8))
        return createPrimitiveItemsTemplate<Int8>(data, visualizationProperties);
    else if(data.isVariant(DataVariant::UINT_8))
        return createPrimitiveItemsTemplate<UInt8>(data, visualizationProperties);
    else if(data.isVariant(DataVariant::INT_16))
        return createPrimitiveItemsTemplate<Int16>(data, visualizationProperties);
    else if(data.isVariant(DataVariant::UINT_16))
        return createPrimitiveItemsTemplate<UInt16>(data, visualizationProperties);
    else if(data.isVariant(DataVariant::INT_32))
        return createPrimitiveItemsTemplate<Int32>(data, visualizationProperties);
    else if(data.isVariant(DataVariant::UINT_32))
        return createPrimitiveItemsTemplate<UInt32>(data, visualizationProperties);
    else if(data.isVariant(DataVariant::FLOAT_32))
        return createPrimitiveItemsTemplate<Float32>(data, visualizationProperties);
    else if(data.isVariant(DataVariant::FLOAT_64))
        return createPrimitiveItemsTemplate<Float64>(data, visualizationProperties);
    else
        return QList<QGraphicsItem*>();
}
