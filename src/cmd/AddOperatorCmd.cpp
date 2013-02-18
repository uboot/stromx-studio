#include "cmd/AddOperatorCmd.h"

#include "model/StreamModel.h"

AddOperatorCmd::AddOperatorCmd(StreamModel* stream, OperatorModel* model, QUndoCommand* parent)
  : QUndoCommand(QObject::tr("add operator"), parent),
    m_stream(stream),
    m_model(model)
{
}

void AddOperatorCmd::undo()
{
    m_stream->doRemoveOperator(m_model);
}

void AddOperatorCmd::redo()
{
    m_stream->doAddOperator(m_model);
}
