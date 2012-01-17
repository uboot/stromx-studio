#include "InitializeOperatorCmd.h"

#include "StreamModel.h"

InitializeOperatorCmd::InitializeOperatorCmd(StreamModel* stream, OperatorModel* model, QUndoCommand* parent)
  : QUndoCommand(QObject::tr("initialize operator"), parent),
    m_stream(stream),
    m_model(model)
{
}

void InitializeOperatorCmd::undo()
{
    m_stream->doDeinitializeOperator(m_model);
}

void InitializeOperatorCmd::redo()
{
    m_stream->doInitializeOperator(m_model);
}
