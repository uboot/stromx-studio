#include "SetParameterCmd.h"

#include "OperatorModel.h"
#include <stromx/core/Operator.h>

SetParameterCmd::SetParameterCmd(OperatorModel* model, unsigned int parameter, const stromx::core::Data& newValue, QUndoCommand* parent)
  : QUndoCommand(QObject::tr("set parameter"), parent),
    m_model(model),
    m_parameter(parameter),
    m_oldValue(0),
    m_newValue(0)
{
    m_oldValue = m_model->op()->getParameter(parameter).clone();
    m_newValue = newValue.clone();
}

SetParameterCmd::~SetParameterCmd()
{
    delete m_oldValue;
    delete m_newValue;
}

void SetParameterCmd::redo()
{
    m_model->doSetParameter(m_parameter, *m_newValue);
}

void SetParameterCmd::undo()
{
    m_model->doSetParameter(m_parameter, *m_oldValue);
}
