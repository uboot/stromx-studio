#include "UndoStackAction.h"

UndoStackAction::UndoStackAction(const QString & prefix, QObject* parent)
  : QAction(parent),
    m_prefix(prefix)
{
}

void UndoStackAction::setUndoText(const QString& text)
{
    setText(QString("%1 %2").arg(m_prefix, text));
}
