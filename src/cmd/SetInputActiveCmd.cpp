#include "SetInputActiveCmd.h"

#include "InputModel.h"

SetInputActiveCmd::SetInputActiveCmd(InputModel* model, bool newActive, QUndoCommand* parent)
  : QUndoCommand(QObject::tr("set input active"), parent),
    m_model(model),
    m_oldActive(model->active()),
    m_newActive(newActive)
{
}

void SetInputActiveCmd::redo()
{
    m_model->doSetActive(m_newActive);
}

void SetInputActiveCmd::undo()
{
    m_model->doSetActive(m_oldActive);
}
