#include "model/InputModel.h"

#ifndef STROMX_STUDIO_QT4
#include <QJsonObject>
#endif

#include <stromx/runtime/Operator.h>

#include "AbstractDataVisualizer.h"
#include "Common.h"
#include "cmd/SetVisualizationStateCmd.h"
#include "model/OperatorModel.h"
#include "model/StreamModel.h"

InputModel::InputModel(OperatorModel* op, unsigned int id, QUndoStack* undoStack, QObject* parent)
  : QObject(parent),
    m_op(op),
    m_id(id),
    m_undoStack(undoStack),
    m_parentModel(0)
{
    m_visualizationState.setCurrentVisualization("default");
    connect(op, SIGNAL(nameChanged(QString)), this, SLOT(updateOperatorName(QString)));
}

void InputModel::updateOperatorName(const QString&)
{
    emit changed(this);
}

QString InputModel::docTitle() const
{
    return fromStromxTitle(m_op->op()->info().input(m_id).title());
}

void InputModel::setVisualizationState(const VisualizationState& state)
{
    if (state != m_visualizationState)
    {
        QUndoCommand* cmd = new SetVisualizationStateCmd(this, state);
        m_undoStack->push(cmd);
    }

}

void InputModel::doSetVisualizationState(const VisualizationState& state)
{
    m_visualizationState = state;
    emit changed(this);
    emit visualizationStateChanged(m_visualizationState);
}

QDataStream& operator<<(QDataStream& stream, const InputModel* model)
{
    stream << model->visualizationState();
    
    return stream;
}

QDataStream& operator>>(QDataStream& stream, InputModel* model)
{
    VisualizationState state;
    
    stream >> state;
    model->doSetVisualizationState(state);
    
    return stream;
}


QDataStream& readVersion01(QDataStream& stream, InputModel* model)
{
    QMap<QString, QVariant> properties;
    stream >> properties;
    
    VisualizationState state;
    state.setIsActive(properties.value("active", true).toBool());
    
    if (properties.keys().contains("visualization"))
    {
        int visualization = properties["visualization"].toInt();
        switch (visualization)
        {
        case AbstractDataVisualizer::POINTS:
            state.setCurrentVisualization("points_2d");
            break;
        case AbstractDataVisualizer::LINES:
            state.setCurrentVisualization("line_segments");
            break;
        case AbstractDataVisualizer::HISTOGRAM:
            state.setCurrentVisualization("histogram");
            break;
        case AbstractDataVisualizer::IMAGE:
            state.setCurrentVisualization("image_2d");
            break;
        default:
            state.setCurrentVisualization("default");
            break;
        }
    }
    
    if (state.currentVisualization() != "image" && properties.keys().contains("color"))
    {
        QVariant color = properties["color"];
        state.currentProperties()["color"] = color;
    }
    
    model->doSetVisualizationState(state);
    
    return stream;
}

#ifndef STROMX_STUDIO_QT4
void InputModel::write(QJsonObject & json) const
{
    m_visualizationState.write(json);
}

void InputModel::read(const QJsonObject & json)
{
    m_visualizationState.read(json);
}
#endif




