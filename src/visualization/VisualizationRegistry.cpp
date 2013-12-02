#include "visualization/VisualizationRegistry.h"

#include "visualization/DefaultVisualization.h"
#include "visualization/LineSegments.h"
#include "visualization/Visualization.h"

VisualizationRegistry VisualizationRegistry::m_registry;

void VisualizationRegistry::registerVisualization(Visualization*const visualization)
{
    Q_ASSERT(visualization);
    Q_ASSERT(! m_identifierList.contains(visualization->visualization()));
        
    m_visualizations.append(visualization);
    m_nameList.append(visualization->name());
    m_identifierList.append(visualization->visualization());
    m_visualizationMap[visualization->visualization()] = visualization;
}

VisualizationRegistry::VisualizationRegistry()
{
    m_registry.registerVisualization(new DefaultVisualization);
    m_registry.registerVisualization(new LineSegments);
}

VisualizationRegistry::~VisualizationRegistry()
{
    foreach(Visualization* visualization, m_visualizations)
        delete visualization;
}

VisualizationRegistry& VisualizationRegistry::registry()
{
    return m_registry;
}

const QList<QString> & VisualizationRegistry::visualizationNameList()
{
    return m_nameList;
}

int VisualizationRegistry::identifierToIndex(const QString& visualization)
{
    return m_identifierList.indexOf(visualization);
}

const Visualization* VisualizationRegistry::visualization(const int index)
{
    if (index >= 0 && index < m_visualizations.count())
        return m_visualizations[index];
    else
        return 0;
}

const Visualization* VisualizationRegistry::visualization(const QString& identifier)
{
    if (m_visualizationMap.keys().contains(identifier))
        return m_visualizationMap[identifier];
    else
        return 0;
}
