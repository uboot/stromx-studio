#include "RemoveThreadCmd.h"
#include "StreamModel.h"

RemoveThreadCmd::RemoveThreadCmd(StreamModel* stream, ThreadModel* model, QUndoCommand* parent)
  : QUndoCommand(parent),
    m_stream(stream),
    m_model(model)
{

}

void RemoveThreadCmd::redo()
{
    m_stream->doRemoveThread(m_model);
}

void RemoveThreadCmd::undo()
{
    m_stream->doAddThread(m_model);
}
