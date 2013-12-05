#include "visualization/Histogram.h"

#include <stromx/runtime/Matrix.h>

#include <QBrush>
#include <QGraphicsLineItem>
#include <QPen>

#include "visualization/ColorChooser.h"

namespace
{
    template <class data_t>
    QList< QGraphicsItem* > createHistogramItemsTemplate(const stromx::runtime::Data& data,
        const VisualizationState::Properties & properties)
    {
        const int WIDTH = 400;
        const int HEIGHT = 400;
        
        using namespace stromx::runtime;
        QVariant colorVariant = properties.value("color", Qt::black);
        QColor color = colorVariant.value<QColor>();
        
        QList<QGraphicsItem*> items;
        try
        {
            // cast the data to a matrix
            const Matrix & matrix = data_cast<Matrix>(data);
            
            if(! (matrix.valueSize() == sizeof(data_t) && matrix.cols() == 1))
                return items;
            
            data_t maximum;
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
                    }
                    else
                    {
                        //Update maximum
                        if(currentValue > maximum)
                        {
                            maximum = currentValue;
                        }
                    }
                }
                rowPtr += matrix.stride();
            }
            
            for(unsigned int i = 0; i < matrix.rows(); ++i)
            {
                float binWidth = static_cast<float>(WIDTH) / matrix.rows();
                float binHeight = static_cast<float>(HEIGHT) / maximum * matrix.at<data_t>(i, 0);
                float x = binWidth * i;
                float y = HEIGHT - binHeight;
                
                QGraphicsRectItem* rect = new QGraphicsRectItem(x, y, binWidth, binHeight);
                rect->setPen(QPen(color));
                rect->setBrush(QBrush(Qt::NoBrush));
                items.append(rect);
            }
        }
        catch(BadCast&)
        {
        }
        
        return items;
    }
}

VisualizationWidget* Histogram::createEditor() const
{
    return new ColorChooser;
}

QList< QGraphicsItem* > Histogram::createItems(const stromx::runtime::Data & data,
        const VisualizationState::Properties & properties) const
{
    using namespace stromx::runtime;
    
    if(data.isVariant(DataVariant::INT_8_MATRIX))
        return createHistogramItemsTemplate<int8_t>(data, properties);
    else if(data.isVariant(DataVariant::UINT_8_MATRIX))
        return createHistogramItemsTemplate<uint8_t>(data, properties);
    else if(data.isVariant(DataVariant::INT_16_MATRIX))
        return createHistogramItemsTemplate<int16_t>(data, properties);
    else if(data.isVariant(DataVariant::UINT_16_MATRIX))
        return createHistogramItemsTemplate<uint16_t>(data, properties);
    else if(data.isVariant(DataVariant::INT_32_MATRIX))
        return createHistogramItemsTemplate<int32_t>(data, properties);
    else if(data.isVariant(DataVariant::UINT_32_MATRIX))
        return createHistogramItemsTemplate<uint32_t>(data, properties);
    else if(data.isVariant(DataVariant::FLOAT_32_MATRIX))
        return createHistogramItemsTemplate<float>(data, properties);
    else if(data.isVariant(DataVariant::FLOAT_64_MATRIX))
        return createHistogramItemsTemplate<double>(data, properties);
    else
        return QList<QGraphicsItem*>();
}
