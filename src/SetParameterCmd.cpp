#include "SetParameterCmd.h"

#include "ParameterServer.h"
#include <stromx/runtime/Operator.h>

SetParameterCmd::SetParameterCmd(ParameterServer* server, unsigned int parameter,
                                 const stromx::runtime::DataRef& oldValue, const stromx::runtime::DataRef& newValue,
                                 QUndoCommand* parent)
  : QUndoCommand(QObject::tr("set parameter"), parent),
    m_server(server),
    m_parameter(parameter),
    m_oldValue(oldValue),
    m_newValue(newValue)
{
}

SetParameterCmd::~SetParameterCmd()
{
}

void SetParameterCmd::redo()
{
    m_server->doSetParameter(m_parameter, m_newValue);
}

void SetParameterCmd::undo()
{
    m_server->doSetParameter(m_parameter, m_oldValue);
}
