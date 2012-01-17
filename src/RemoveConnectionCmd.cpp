#include "RemoveConnectionCmd.h"

#include "StreamModel.h"

RemoveConnectionCmd::RemoveConnectionCmd(StreamModel* stream, ConnectionModel* model, QUndoCommand* parent)
  : QUndoCommand(QObject::tr("remove operator"), parent),
    m_stream(stream),
    m_model(model)
{
}

void RemoveConnectionCmd::redo()
{
    m_stream->doRemoveConnection(m_model);
}

void RemoveConnectionCmd::undo()
{
    m_stream->doAddConnection(m_model);
}
