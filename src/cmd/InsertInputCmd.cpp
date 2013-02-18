#include "cmd/InsertInputCmd.h"

#include "model/ObserverTreeModel.h"
#include "model/StreamModel.h"


InsertInputCmd::InsertInputCmd(ObserverTreeModel* observerTree, int observerPos, int inputPos, InputModel* input, QUndoCommand* parent)
  : QUndoCommand(QObject::tr("insert input"), parent),
    m_observerTree(observerTree),
    m_observerPos(observerPos),
    m_inputPos(inputPos),
    m_input(input)
{

}

void InsertInputCmd::redo()
{
    m_observerTree->doInsertInput(m_observerPos, m_inputPos, m_input);
}

void InsertInputCmd::undo()
{
    m_observerTree->doRemoveInput(m_observerPos, m_inputPos);
}
