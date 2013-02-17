#include "SetInputColorCmd.h"

#include "model/InputModel.h"

SetInputColorCmd::SetInputColorCmd(InputModel* model, const QColor& newColor, QUndoCommand* parent)
  : QUndoCommand(QObject::tr("set input color"), parent),
    m_model(model),
    m_oldColor(model->color()),
    m_newColor(newColor)
{
}

void SetInputColorCmd::redo()
{
    m_model->doSetColor(m_newColor);
}

void SetInputColorCmd::undo()
{
    m_model->doSetColor(m_oldColor);
}
