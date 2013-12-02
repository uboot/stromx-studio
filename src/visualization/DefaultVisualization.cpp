#include "visualization/DefaultVisualization.h"

#include "visualization/ColorChooser.h"

VisualizationWidget* DefaultVisualization::createEditor() const
{
    return new ColorChooser;
}

QList< QGraphicsItem* > DefaultVisualization::createItems(const stromx::runtime::Data & data,
        const VisualizationState::Properties & properties) const
{
    return QList<QGraphicsItem*>();
}
