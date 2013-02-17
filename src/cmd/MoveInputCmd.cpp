#include "MoveInputCmd.h"

#include "model/ObserverTreeModel.h"


MoveInputCmd::MoveInputCmd(ObserverTreeModel* observerTree, int srcObserverPos, int srcInputPos, int destObserverPos, int destInputPos, InputModel* input, QUndoCommand* parent)
  : QUndoCommand(QObject::tr("move input"), parent),
    m_observerTree(observerTree),
    m_srcObserverPos(srcObserverPos),
    m_srcInputPos(srcInputPos),
    m_destObserverPos(destObserverPos),
    m_destInputPos(destInputPos),
    m_input(input)
{
}

void MoveInputCmd::redo()
{
    m_observerTree->doMoveInput(m_srcObserverPos, m_srcInputPos, m_destObserverPos, m_destInputPos, m_input);
}

void MoveInputCmd::undo()
{
    m_observerTree->doMoveInput(m_destObserverPos, m_destInputPos, m_srcObserverPos, m_srcInputPos, m_input);
}
