#include "RemoveInputCmd.h"

#include "ObserverTreeModel.h"
#include "StreamModel.h"


RemoveInputCmd::RemoveInputCmd(ObserverTreeModel* observerTree, int observerPos, int inputPos, InputModel* input, QUndoCommand* parent)
  : QUndoCommand(QObject::tr("remove input"), parent),
    m_observerTree(observerTree),
    m_observerPos(observerPos),
    m_inputPos(inputPos),
    m_input(input)
{

}

void RemoveInputCmd::redo()
{
    m_observerTree->doRemoveInput(m_observerPos, m_inputPos);
}

void RemoveInputCmd::undo()
{
    m_observerTree->doInsertInput(m_observerPos, m_inputPos, m_input);
}
