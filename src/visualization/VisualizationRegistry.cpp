#include "visualization/VisualizationRegistry.h"

#include "visualization/DefaultVisualization.h"
#include "visualization/LineSegments.h"
#include "visualization/Points.h"
#include "visualization/Histogram.h"
#include "visualization/Image.h"
#include "visualization/Visualization.h"

VisualizationRegistry VisualizationRegistry::m_registry;

void VisualizationRegistry::registerVisualization(Visualization*const visualization)
{
    Q_ASSERT(visualization);
    Q_ASSERT(! m_registry.m_identifierList.contains(visualization->visualization()));
        
    m_registry.m_visualizations.append(visualization);
    m_registry.m_nameList.append(visualization->name());
    m_registry.m_identifierList.append(visualization->visualization());
    m_registry.m_visualizationMap[visualization->visualization()] = visualization;
}

VisualizationRegistry::VisualizationRegistry()
{
    m_registry.registerVisualization(new DefaultVisualization);
    m_registry.registerVisualization(new Histogram);
    m_registry.registerVisualization(new Image);
    m_registry.registerVisualization(new LineSegments);
    m_registry.registerVisualization(new Points);
}

VisualizationRegistry::~VisualizationRegistry()
{
    foreach(Visualization* visualization, m_visualizations)
        delete visualization;
}

const QList<QString> & VisualizationRegistry::visualizationNameList()
{
    return m_registry.m_nameList;
}

int VisualizationRegistry::identifierToIndex(const QString& visualization)
{
    return m_registry.m_identifierList.indexOf(visualization);
}

const Visualization* VisualizationRegistry::visualization(const int index)
{
    if (index >= 0 && index < m_registry.m_visualizations.count())
        return m_registry.m_visualizations[index];
    else
        return 0;
}

const Visualization* VisualizationRegistry::visualization(const QString& identifier)
{
    if (m_registry.m_visualizationMap.keys().contains(identifier))
        return m_registry.m_visualizationMap[identifier];
    else
        return 0;
}
