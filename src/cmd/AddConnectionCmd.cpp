#include "AddConnectionCmd.h"

#include "model/StreamModel.h"

AddConnectionCmd::AddConnectionCmd(StreamModel* stream, ConnectionModel* model, QUndoCommand* parent)
  : QUndoCommand(parent),
    m_stream(stream),
    m_model(model)
{
}

void AddConnectionCmd::redo()
{
    m_stream->doAddConnection(m_model);
}

void AddConnectionCmd::undo()
{
    m_stream->doRemoveConnection(m_model);
}
