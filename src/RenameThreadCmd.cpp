#include "RenameThreadCmd.h"

#include "ThreadModel.h"

RenameThreadCmd::RenameThreadCmd(ThreadModel* model, const QString& newName, QUndoCommand* parent)
  : QUndoCommand(QObject::tr("rename operator"), parent),
    m_model(model),
    m_oldName(model->name()),
    m_newName(newName)
{
}

void RenameThreadCmd::redo()
{
    m_model->doSetName(m_newName);
}

void RenameThreadCmd::undo()
{
    m_model->doSetName(m_oldName);
}
