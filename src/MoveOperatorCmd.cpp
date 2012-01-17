#include "MoveOperatorCmd.h"

#include "OperatorModel.h"

MoveOperatorCmd::MoveOperatorCmd(OperatorModel* model, const QPointF& newPos, QUndoCommand* parent)
  : QUndoCommand(QObject::tr("move operator"), parent),
    m_model(model),
    m_oldPos(model->pos()),
    m_newPos(newPos)
{
}

void MoveOperatorCmd::redo()
{
    m_model->doSetPos(m_newPos);
}

void MoveOperatorCmd::undo()
{
    m_model->doSetPos(m_oldPos);
}
