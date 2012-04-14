#include "RemoveOperatorCmd.h"

#include "StreamModel.h"

RemoveOperatorCmd::RemoveOperatorCmd(StreamModel* stream, OperatorModel* model, QUndoCommand* parent)
  : QUndoCommand(QObject::tr("remove operator"), parent),
    m_stream(stream),
    m_model(model)
{
}

void RemoveOperatorCmd::undo()
{
    m_stream->doAddOperator(m_model);
}

void RemoveOperatorCmd::redo()
{
    m_stream->doRemoveOperator(m_model);
}
