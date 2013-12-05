#include "visualization/Image.h"

#include <stromx/runtime/Matrix.h>

#include <qmath.h>
#include <QBrush>
#include <QGraphicsLineItem>
#include <QPen>

namespace
{
template <class data_t>
    QList< QGraphicsItem* > createImageFromMatrixTemplate(const stromx::runtime::Data& data,
                                                          const VisualizationState::Properties & /*properties*/)
    {
        using namespace stromx::runtime;
        
        QList<QGraphicsItem*> items;
        try
        {
            // cast the data to a matrix
            const Matrix & matrix = data_cast<Matrix>(data);
            
            // check if the value size of the matrix matches the size of the template
            // parameter
            data_t maximum;
            data_t minimum;
            if(matrix.valueSize() == sizeof(data_t))
            {
                // loop over the rows of the matrix and search for maximum value
                const uint8_t* rowPtr = matrix.data();
                for(unsigned int i = 0; i < matrix.rows(); ++i)
                {
                    const data_t* rowData = reinterpret_cast<const data_t*>(rowPtr);
                    for(unsigned int j = 0; j < matrix.cols(); ++j)
                    {
                        data_t currentValue = rowData[j];
                        if(i==0 && j==0)
                        {
                            //Initialization of min and max with first matrix entry
                            maximum = currentValue;
                            minimum = currentValue;
                        }
                        else
                        {
                            //Update maximum
                            if(currentValue > maximum)
                            {
                                maximum = currentValue;
                            }
                            else
                            {
                                if(currentValue < minimum)
                                {
                                    //Update minimum
                                    minimum = currentValue;
                                }
                            }
                        }
                    }
                    rowPtr += matrix.stride();
                }
                
                data_t range = maximum - minimum;
           
                //loop over the rows of the matrix and re-scale each entry such that
                //the determined maximum value becomes 255 (uchar) and store it in
                //grey-scale QT image
                QImage qtImage = QImage(matrix.cols(), matrix.rows(), QImage::Format_Indexed8);
                
                QVector<QRgb> colorTable(256);
                for(unsigned int i = 0; i < 256; ++i)
                    colorTable[i] = qRgb(i, i, i);
                qtImage.setColorTable(colorTable);
                
                const uint8_t* rowPtrSrc = matrix.data();
                for(unsigned int i = 0; i < matrix.rows(); ++i)
                {
                    const data_t* pixelPtrSrc = reinterpret_cast<const data_t*>(rowPtrSrc);
                    uchar* pixelPtrDst = qtImage.scanLine(i);
                    for(unsigned int j = 0; j < matrix.cols(); ++j)
                    {
                        //Caution: cast to double necessary to prevent integer arithmetic in case of data_t being a int or uint variant
                        pixelPtrDst[j] = static_cast<uchar>(qFloor((static_cast<double>(*pixelPtrSrc) - static_cast<double>(minimum))/static_cast<double>(range) * 255.0));
                        ++pixelPtrSrc;
                    }
                    rowPtrSrc += matrix.stride();
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
}

VisualizationWidget* Image::createEditor() const
{
    return 0;
}

QList< QGraphicsItem* > Image::createItems(const stromx::runtime::Data & data,
        const VisualizationState::Properties & properties) const
{
    using namespace stromx::runtime;
    
    if(data.isVariant(DataVariant::INT_8_MATRIX))
        return createImageFromMatrixTemplate<int8_t>(data, properties);
    else if(data.isVariant(DataVariant::UINT_8_MATRIX))
        return createImageFromMatrixTemplate<uint8_t>(data, properties);
    else if(data.isVariant(DataVariant::INT_16_MATRIX))
        return createImageFromMatrixTemplate<int16_t>(data, properties);
    else if(data.isVariant(DataVariant::UINT_16_MATRIX))
        return createImageFromMatrixTemplate<uint16_t>(data, properties);
    else if(data.isVariant(DataVariant::INT_32_MATRIX))
        return createImageFromMatrixTemplate<int32_t>(data, properties);
    else if(data.isVariant(DataVariant::UINT_32_MATRIX))
        return createImageFromMatrixTemplate<uint32_t>(data, properties);
    else if(data.isVariant(DataVariant::FLOAT_32_MATRIX))
        return createImageFromMatrixTemplate<float>(data, properties);
    else if(data.isVariant(DataVariant::FLOAT_64_MATRIX))
        return createImageFromMatrixTemplate<double>(data, properties);
    else
        return QList<QGraphicsItem*>();
}
