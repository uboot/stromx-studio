#include "model/InputModel.h"

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
    m_undoStack(undoStack) 
{
    connect(op, SIGNAL(nameChanged(QString)), this, SLOT(updateOperatorName(QString)));
}

void InputModel::updateOperatorName(const QString&)
{
    emit changed(this);
}

QString InputModel::docTitle() const
{
    return QString::fromStdString(m_op->op()->info().input(m_id).title());
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
            state.setCurrentVisualization("points");
            break;
        case AbstractDataVisualizer::LINES:
            state.setCurrentVisualization("line_segments");
            break;
        case AbstractDataVisualizer::HISTOGRAM:
            state.setCurrentVisualization("histogram");
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





