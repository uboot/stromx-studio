#include "visualization/LineSegments.h"

VisualizationWidget* LineSegments::createEditor() const
{
    return 0;
}

QList< QGraphicsItem* > LineSegments::createItems(const stromx::runtime::Data & data,
        const VisualizationState::Properties & properties) const
{
    return QList<QGraphicsItem*>();
}
