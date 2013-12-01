#include "cmd/SetStreamSettingsCmd.h"

#include <QVariant>

#include "model/StreamModel.h"

SetStreamSettingsCmd::SetStreamSettingsCmd(StreamModel* model, 
           const QMap<QString, QVariant> & newSettings, QUndoCommand* parent)
  : QUndoCommand(QObject::tr("set stream settings"), parent),
    m_model(model),
    m_newSettings(newSettings),
    m_oldSettings(m_model->m_settings)
{
}

void SetStreamSettingsCmd::undo()
{
    m_model->doSetSettings(m_oldSettings);
}

void SetStreamSettingsCmd::redo()
{
    m_model->doOverwriteSettings(m_newSettings);
}