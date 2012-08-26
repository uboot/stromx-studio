#include "DataVisualizerUtilities.h"

#include <stromx/core/Image.h>
#include <stromx/core/Primitive.h>
#include <stromx/core/String.h>

namespace
{
    template <class data_t>
    static QList<QGraphicsItem*> createPrimitiveItemsTemplate(const stromx::core::Data & data)
    {
        QList<QGraphicsItem*> items;
        
        try
        {
            // cast to the concrete primitive type
            const data_t & primitive = stromx::core::data_cast<const data_t &>(data);
            
            // format it as a astring and create a graphics item from the string
            QGraphicsItem* item = new QGraphicsSimpleTextItem(QString("%1").arg(primitive));
            
            // pack the graphics item into a list
            items.append(item);
        }
        catch(stromx::core::BadCast&)
        {
        }
        
        return items;
    }
    
    template <class data_t>
    QList< QGraphicsItem* > createLineSegmentItemsTemplate(const stromx::core::Data& data)
    {
        using namespace stromx::core;
        
        QList<QGraphicsItem*> items;
        try
        {
            // cast the data to a matrix
            const Matrix & matrix = data_cast<const Matrix &>(data);
            
            // check if the value size of the matrix matches the size of the template
            // parameter and make sure the matrix has 4 columns
            if(matrix.valueSize() == sizeof(data_t) && matrix.cols() == 4)
            {
                // loop over the rows of the matrix and construct a line item from each row
                const uint8_t* rowPtr = matrix.data();
                for(unsigned int i = 0; i < matrix.rows(); ++i)
                {
                    data_t* rowData = (data_t*)(rowPtr);
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
}

QList<QGraphicsItem*> DataVisualizerUtilities::createImageItems(const stromx::core::Data& data)
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
   
QList<QGraphicsItem*> DataVisualizerUtilities::createStringItems(const stromx::core::Data & data)
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

QList< QGraphicsItem* > DataVisualizerUtilities::createLineSegmentItems(const stromx::core::Data& data)
{
    using namespace stromx::core;
    
    if(data.isVariant(DataVariant::INT_8_MATRIX))
        return createLineSegmentItemsTemplate<int8_t>(data);
    else if(data.isVariant(DataVariant::UINT_8_MATRIX))
        return createLineSegmentItemsTemplate<uint8_t>(data);
    else if(data.isVariant(DataVariant::INT_16_MATRIX))
        return createLineSegmentItemsTemplate<int16_t>(data);
    else if(data.isVariant(DataVariant::UINT_16_MATRIX))
        return createLineSegmentItemsTemplate<uint16_t>(data);
    else if(data.isVariant(DataVariant::INT_32_MATRIX))
        return createLineSegmentItemsTemplate<int32_t>(data);
    else if(data.isVariant(DataVariant::UINT_32_MATRIX))
        return createLineSegmentItemsTemplate<uint32_t>(data);
    else if(data.isVariant(DataVariant::FLOAT_MATRIX))
        return createLineSegmentItemsTemplate<float>(data);
    else if(data.isVariant(DataVariant::DOUBLE_MATRIX))
        return createLineSegmentItemsTemplate<double>(data);
    else
        return QList<QGraphicsItem*>();
}

QList<QGraphicsItem*> DataVisualizerUtilities::createPrimitiveItems(const stromx::core::Data & data)
{    
    using namespace stromx::core;
    
    if(data.isVariant(DataVariant::BOOL))
        return createPrimitiveItemsTemplate<Bool>(data);
    else if(data.isVariant(DataVariant::INT_8))
        return createPrimitiveItemsTemplate<Int8>(data);
    else if(data.isVariant(DataVariant::UINT_8))
        return createPrimitiveItemsTemplate<UInt8>(data);
    else if(data.isVariant(DataVariant::INT_16))
        return createPrimitiveItemsTemplate<Int16>(data);
    else if(data.isVariant(DataVariant::UINT_16))
        return createPrimitiveItemsTemplate<UInt16>(data);
    else if(data.isVariant(DataVariant::INT_32))
        return createPrimitiveItemsTemplate<Int32>(data);
    else if(data.isVariant(DataVariant::UINT_32))
        return createPrimitiveItemsTemplate<UInt32>(data);
    else if(data.isVariant(DataVariant::FLOAT))
        return createPrimitiveItemsTemplate<Float>(data);
    else if(data.isVariant(DataVariant::DOUBLE))
        return createPrimitiveItemsTemplate<Double>(data);
    else
        return QList<QGraphicsItem*>();
}
