#include "DeinitializeOperatorCmd.h"

#include "StreamModel.h"

DeinitializeOperatorCmd::DeinitializeOperatorCmd(StreamModel* stream, OperatorModel* model, QUndoCommand* parent)
  : QUndoCommand(parent),
    m_stream(stream),
    m_model(model)
{
}

void DeinitializeOperatorCmd::undo()
{
    m_stream->doInitializeOperator(m_model);
}

void DeinitializeOperatorCmd::redo()
{
    m_stream->doDeinitializeOperator(m_model);
}
