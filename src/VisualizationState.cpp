#include "VisualizationState.h"

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

QMap<QString, QVariant>& VisualizationState::properties(const QString& visualization)
{
    return m_properties[visualization];
}

QDataStream& operator<<(QDataStream& stream, const VisualizationState* state)
{
    stream << state->m_active;
    stream << state->m_visualization;
    stream << state->m_properties;
    
    return stream;
}

QDataStream& operator>>(QDataStream& stream, VisualizationState* state)
{
    stream >> state->m_active;
    stream >> state->m_visualization;
    stream >> state->m_properties;
    
    return stream;
}


