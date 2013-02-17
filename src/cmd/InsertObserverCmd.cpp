#include "InsertObserverCmd.h"

#include "model/ObserverTreeModel.h"
#include "model/StreamModel.h"


InsertObserverCmd::InsertObserverCmd(ObserverTreeModel* observerTree, int pos, ObserverModel* observer, QUndoCommand* parent)
  : QUndoCommand(QObject::tr("insert observer"), parent),
    m_observerTree(observerTree),
    m_observer(observer),
    m_pos(pos)
{
}

void InsertObserverCmd::redo()
{
    m_observerTree->doInsertObserver(m_pos, m_observer);
}

void InsertObserverCmd::undo()
{
    m_observerTree->doRemoveObserver(m_pos);
}
