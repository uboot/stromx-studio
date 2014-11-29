#include "VisualizationState.h"

#ifndef STROMX_STUDIO_QT4
#include <QJsonObject>
#endif

#include <QDataStream>

VisualizationState::VisualizationState()
  : m_active(true)
{

}

VisualizationState::VisualizationState(const VisualizationState& state)
  : m_active(state.m_active),
    m_visualization(state.m_visualization),
    m_properties(state.m_properties)
{

}

const VisualizationState& VisualizationState::operator=(const VisualizationState& state)
{
    m_active = state.m_active;
    m_visualization = state.m_visualization;
    m_properties = state.m_properties;
    
    return *this;
}

void VisualizationState::setCurrentVisualization(const QString& visualization)
{
    m_visualization = visualization;
}

void VisualizationState::setIsActive(const bool active)
{
    m_active = active;
}

VisualizationState::Properties & VisualizationState::properties(const QString& visualization)
{
    return m_properties[visualization];
}

const VisualizationState::Properties VisualizationState::properties(const QString& visualization) const
{
    return m_properties[visualization];
}

VisualizationState::Properties & VisualizationState::currentProperties()
{
    return m_properties[m_visualization];
}

const VisualizationState::Properties VisualizationState::currentProperties() const
{
    return m_properties[m_visualization];
}

QDataStream& operator<<(QDataStream& stream, const VisualizationState & state)
{
    stream << state.m_active;
    stream << state.m_visualization;
    stream << state.m_properties;
    
    return stream;
}

QDataStream& operator>>(QDataStream& stream, VisualizationState & state)
{
    stream >> state.m_active;
    stream >> state.m_visualization;
    stream >> state.m_properties;
    
    return stream;
}

#ifndef STROMX_STUDIO_QT4
void VisualizationState::write(QJsonObject & json) const
{    
    json["active"] = m_active;
    json["visualization"] = m_visualization;
    
    // loop over all visualizations
    QJsonObject properties;
    QMapIterator<QString, QVariant> i(m_properties[m_visualization]);
    while (i.hasNext())
    {
        i.next();
        properties[i.key()] = QJsonValue::fromVariant(i.value());
    }
    
    json["properties"] = properties;
}

void VisualizationState::read(const QJsonObject & json)
{
    m_active = json["active"].toBool();
    m_visualization = json["visualization"].toString();
    m_properties.clear();
    m_properties[m_visualization] = Properties();
    
    QVariantMap properties = json["properties"].toObject().toVariantMap();
    QMapIterator<QString, QVariant> i(properties);
    QVariantMap& inputProperties = m_properties[m_visualization];
    while (i.hasNext())
    {
        inputProperties[i.key()] = i.value();
    }
}
#endif

bool operator==(const VisualizationState& lhs, const VisualizationState& rhs)
{
    return lhs.m_active == rhs.m_active && 
           lhs.m_visualization == rhs.m_visualization &&
           lhs.m_properties == rhs.m_properties;
}

bool operator!=(const VisualizationState& lhs, const VisualizationState& rhs)
{
    return ! (lhs == rhs);
}


