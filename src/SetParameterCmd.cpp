#include "SetParameterCmd.h"

#include "OperatorModel.h"
#include <stromx/core/Operator.h>

SetParameterCmd::SetParameterCmd(OperatorModel* model, unsigned int parameter, const stromx::core::Data& newValue, QUndoCommand* parent)
  : QUndoCommand(QObject::tr("set parameter"), parent),
    m_parameter(parameter),
    m_oldValue(0),
    m_newValue(0)
{
//     m_oldValue = model->op()->
    
}

void SetParameterCmd::redo()
{
    QUndoCommand::redo();
}

void SetParameterCmd::undo()
{
    QUndoCommand::undo();
}
