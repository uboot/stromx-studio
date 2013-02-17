#include "SetThreadColorCmd.h"

#include "model/ThreadModel.h"

SetThreadColorCmd::SetThreadColorCmd(ThreadModel* model, const QColor& newColor, QUndoCommand* parent)
  : QUndoCommand(QObject::tr("set thread color"), parent),
    m_model(model),
    m_oldColor(model->color()),
    m_newColor(newColor)
{
}

void SetThreadColorCmd::redo()
{
    m_model->doSetColor(m_newColor);
}

void SetThreadColorCmd::undo()
{
    m_model->doSetColor(m_oldColor);
}
