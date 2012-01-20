#include "AddThreadCmd.h"
#include "StreamModel.h"

AddThreadCmd::AddThreadCmd(StreamModel* stream, ThreadModel* model, QUndoCommand* parent)
  : QUndoCommand(parent),
    m_stream(stream),
    m_model(model)
{

}

void AddThreadCmd::redo()
{
    m_stream->doAddThread(m_model);
}

void AddThreadCmd::undo()
{
    m_stream->doRemoveThread(m_model);
}
