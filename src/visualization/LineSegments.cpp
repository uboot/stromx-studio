#include "visualization/LineSegments.h"

#include <stromx/runtime/Matrix.h>

#include <QBrush>
#include <QGraphicsLineItem>
#include <QPen>

#include "Common.h"
#include "visualization/ColorChooser.h"

namespace
{
    template <class data_t>
    QList< QGraphicsItem* > createLineSegmentItemsTemplate(const stromx::runtime::Data& data, 
        const VisualizationState::Properties & properties)
    {
        using namespace stromx::runtime;
        QVariant colorVariant = properties.value("color", Colors::DEFAULT);
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
}

VisualizationWidget* LineSegments::createEditor() const
{
    return new ColorChooser;
}

QList< QGraphicsItem* > LineSegments::createItems(const stromx::runtime::Data & data,
        const VisualizationState::Properties & properties) const
{
    using namespace stromx::runtime;
    
    if(data.isVariant(Variant::INT_8_MATRIX))
        return createLineSegmentItemsTemplate<int8_t>(data, properties);
    else if(data.isVariant(Variant::UINT_8_MATRIX))
        return createLineSegmentItemsTemplate<uint8_t>(data, properties);
    else if(data.isVariant(Variant::INT_16_MATRIX))
        return createLineSegmentItemsTemplate<int16_t>(data, properties);
    else if(data.isVariant(Variant::UINT_16_MATRIX))
        return createLineSegmentItemsTemplate<uint16_t>(data, properties);
    else if(data.isVariant(Variant::INT_32_MATRIX))
        return createLineSegmentItemsTemplate<int32_t>(data, properties);
    else if(data.isVariant(Variant::UINT_32_MATRIX))
        return createLineSegmentItemsTemplate<uint32_t>(data, properties);
    else if(data.isVariant(Variant::FLOAT_32_MATRIX))
        return createLineSegmentItemsTemplate<float>(data, properties);
    else if(data.isVariant(Variant::FLOAT_64_MATRIX))
        return createLineSegmentItemsTemplate<double>(data, properties);
    else
        return QList<QGraphicsItem*>();
}
