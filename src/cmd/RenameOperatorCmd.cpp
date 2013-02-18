#include "cmd/RenameOperatorCmd.h"

#include "model/OperatorModel.h"

RenameOperatorCmd::RenameOperatorCmd(OperatorModel* model, const QString& newName, QUndoCommand* parent)
  : QUndoCommand(QObject::tr("rename operator"), parent),
    m_model(model),
    m_oldName(model->name()),
    m_newName(newName)
{
}

void RenameOperatorCmd::redo()
{
    m_model->doSetName(m_newName);
}

void RenameOperatorCmd::undo()
{
    m_model->doSetName(m_oldName);
}
