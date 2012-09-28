#include "ParameterServer.h"

ParameterServer::ParameterServer(stromx::core::Operator* op, QUndoStack* undoStack, QObject* parent)
  : QObject(parent),
    m_op(op),
    m_undoStack(undoStack)
{

}
