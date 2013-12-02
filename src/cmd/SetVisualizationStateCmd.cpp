#include "cmd/SetVisualizationStateCmd.h"

#include "model/InputModel.h"

SetVisualizationStateCmd::SetVisualizationStateCmd(InputModel* model,
    const VisualizationState& newState, QUndoCommand* parent)
  : QUndoCommand(QObject::tr("set visualization properties"), parent),
    m_model(model),
    m_oldState(model->visualizationState()),
    m_newState(newState)
{
}

void SetVisualizationStateCmd::redo()
{
    m_model->doSetVisualizationState(m_newState);
}

void SetVisualizationStateCmd::undo()
{
    m_model->doSetVisualizationState(m_oldState);
}
