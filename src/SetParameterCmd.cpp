#include "SetParameterCmd.h"

#include "ParameterServer.h"
#include <stromx/core/Operator.h>

SetParameterCmd::SetParameterCmd(ParameterServer* server, unsigned int parameter,
                                 const stromx::core::Data& newValue, QUndoCommand* parent)
  : QUndoCommand(QObject::tr("set parameter"), parent),
    m_server(server),
    m_parameter(parameter),
    m_oldValue(0),
    m_newValue(0)
{
    m_oldValue = m_server->op()->getParameter(parameter).clone();
    m_newValue = newValue.clone();
}

SetParameterCmd::~SetParameterCmd()
{
    delete m_oldValue;
    delete m_newValue;
}

void SetParameterCmd::redo()
{
    m_server->doSetParameter(m_parameter, *m_newValue);
}

void SetParameterCmd::undo()
{
    m_server->doSetParameter(m_parameter, *m_oldValue);
}
