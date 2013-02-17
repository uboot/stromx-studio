#include "SetThreadCmd.h"

#include "model/ConnectionModel.h"
#include "model/InputModel.h"

SetThreadCmd::SetThreadCmd(ConnectionModel* connection, ThreadModel* newThread, QUndoCommand* parent)
  : QUndoCommand(QObject::tr("set thread"), parent),
    m_model(connection),
    m_oldThread(connection->thread()),
    m_newThread(newThread)
{
}

void SetThreadCmd::redo()
{
    m_model->doSetThread(m_newThread);
}

void SetThreadCmd::undo()
{
    m_model->doSetThread(m_oldThread);
}
