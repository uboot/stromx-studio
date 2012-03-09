#include "LimitUndoStack.h"

#include "UndoStackAction.h"


LimitUndoStack::LimitUndoStack(QObject* parent)
  : QUndoStack(parent),
    m_currentUndoLimit(0)
{
}

QAction* LimitUndoStack::createLimitUndoAction(QObject* parent, const QString& prefix)
{
    UndoStackAction* action = new UndoStackAction(prefix, parent);
    connect(this, SIGNAL(undoTextChanged(QString)), action, SLOT(setUndoText(QString)));
    connect(this, SIGNAL(undoActionEnabledChanged(bool)), action, SLOT(setEnabled(bool)));
    return action;
}

void LimitUndoStack::handleCanUndoChanged(bool canUndo)
{
    // disable undo stack for any undo actions beyond the limit
    if(index() == m_currentUndoLimit)
        emit undoActionEnabledChanged(false);
    else
        emit undoActionEnabledChanged(canUndo);
}

void LimitUndoStack::handleUndoIndexChanged(int index)
{
    // disable undo stack for any undo actions beyond the limit
    if(QUndoStack::index() == m_currentUndoLimit)
        undoActionEnabledChanged(false);
}

void LimitUndoStack::activateLimit()
{
    m_currentUndoLimit = index();
}

void LimitUndoStack::deactivateLimit()
{
    m_currentUndoLimit = 0;
}
