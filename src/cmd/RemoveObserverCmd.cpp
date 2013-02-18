#include "cmd/RemoveObserverCmd.h"

#include "model/ObserverTreeModel.h"
#include "model/StreamModel.h"


RemoveObserverCmd::RemoveObserverCmd(ObserverTreeModel* observerTree, int pos, ObserverModel* observer, QUndoCommand* parent)
  : QUndoCommand(QObject::tr("remove observer"), parent),
    m_observerTree(observerTree),
    m_observer(observer),
    m_pos(pos)
{
}

void RemoveObserverCmd::redo()
{
    m_observerTree->doRemoveObserver(m_pos);
}

void RemoveObserverCmd::undo()
{
    m_observerTree->doInsertObserver(m_pos, m_observer);
}
