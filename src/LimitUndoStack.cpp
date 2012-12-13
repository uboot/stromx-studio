#include "LimitUndoStack.h"

#include "UndoStackAction.h"


LimitUndoStack::LimitUndoStack(QObject* parent)
  : QUndoStack(parent),
    m_currentUndoLimit(0)
{
    connect(this, SIGNAL(canUndoChanged(bool)), this, SLOT(handleCanUndoChanged(bool)));
    connect(this, SIGNAL(indexChanged(int)), this, SLOT(handleUndoIndexChanged(int)));
}

QAction* LimitUndoStack::createLimitUndoAction(QObject* parent, const QString& prefix)
{
    UndoStackAction* action = new UndoStackAction(prefix.isNull() ? tr("Undo") : prefix, parent);
    action->setUndoText(undoText());
    action->setEnabled(canUndo());
    connect(this, SIGNAL(undoTextChanged(QString)), action, SLOT(setUndoText(QString)));
    connect(this, SIGNAL(undoActionEnabledChanged(bool)), action, SLOT(setEnabled(bool)));
    connect(action, SIGNAL(triggered(bool)), this, SLOT(undo()));
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

void LimitUndoStack::handleUndoIndexChanged(int /*index*/)
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
