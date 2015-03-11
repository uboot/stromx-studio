#include "visualization/Points.h"

#include <stromx/runtime/Matrix.h>

#include <QBrush>
#include <QGraphicsEllipseItem>
#include <QPen>

#include "Common.h"
#include "visualization/ColorChooser.h"

namespace
{
    template <class data_t>
    QList< QGraphicsItem* > createPointItemsTemplate(const stromx::runtime::Data& data,
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
            // parameter and make sure the matrix has 2 columns
            if(matrix.valueSize() == sizeof(data_t) && matrix.cols() == 2)
            {
                // loop over the rows of the matrix and construct a point item from each row
                const uint8_t* rowPtr = matrix.data();
                for(unsigned int i = 0; i < matrix.rows(); ++i)
                {
                    const data_t* rowData = reinterpret_cast<const data_t*>(rowPtr);
                    QGraphicsEllipseItem* newItem = new QGraphicsEllipseItem(rowData[0]-2, rowData[1]-2, 4, 4);
                    newItem->setPen(QPen(Qt::NoPen));
                    newItem->setBrush(QBrush(color));
                    items.append(newItem);
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

VisualizationWidget* Points::createEditor() const
{
    return new ColorChooser;
}

QList< QGraphicsItem* > Points::createItems(const stromx::runtime::Data & data,
        const VisualizationState::Properties & properties) const
{
    using namespace stromx::runtime;
        
    if(data.isVariant(Variant::INT_8_MATRIX))
        return createPointItemsTemplate<int8_t>(data, properties);
    else if(data.isVariant(Variant::UINT_8_MATRIX))
        return createPointItemsTemplate<uint8_t>(data, properties);
    else if(data.isVariant(Variant::INT_16_MATRIX))
        return createPointItemsTemplate<int16_t>(data, properties);
    else if(data.isVariant(Variant::UINT_16_MATRIX))
        return createPointItemsTemplate<uint16_t>(data, properties);
    else if(data.isVariant(Variant::INT_32_MATRIX))
        return createPointItemsTemplate<int32_t>(data, properties);
    else if(data.isVariant(Variant::UINT_32_MATRIX))
        return createPointItemsTemplate<uint32_t>(data, properties);
    else if(data.isVariant(Variant::FLOAT_32_MATRIX))
        return createPointItemsTemplate<float>(data, properties);
    else if(data.isVariant(Variant::FLOAT_64_MATRIX))
        return createPointItemsTemplate<double>(data, properties);
    else
        return QList<QGraphicsItem*>();
}
