#include "cmd/SetVisualizationPropertiesCmd.h"

#include "model/InputModel.h"

SetVisualizationPropertiesCmd::SetVisualizationPropertiesCmd(InputModel* model,
    const InputModel::VisualizationProperties& newProperties, QUndoCommand* parent)
  : QUndoCommand(QObject::tr("set input visualization"), parent),
    m_model(model),
    m_oldProperties(model->visualizationProperties()),
    m_newProperties(newProperties)
{
}

void SetVisualizationPropertiesCmd::redo()
{
    m_model->doSetVisualizationProperties(m_newProperties);
}

void SetVisualizationPropertiesCmd::undo()
{
    m_model->doSetVisualizationProperties(m_oldProperties);
}
