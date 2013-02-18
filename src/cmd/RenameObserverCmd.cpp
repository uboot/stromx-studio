#include "cmd/RenameObserverCmd.h"

#include "model/ObserverModel.h"

RenameObserverCmd::RenameObserverCmd(ObserverModel* model, const QString& newName, QUndoCommand* parent)
  : QUndoCommand(QObject::tr("rename observer"), parent),
    m_model(model),
    m_oldName(model->name()),
    m_newName(newName)
{
}

void RenameObserverCmd::redo()
{
    m_model->doSetName(m_newName);
}

void RenameObserverCmd::undo()
{
    m_model->doSetName(m_oldName);
}
