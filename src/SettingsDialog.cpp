#include "SettingsDialog.h"

SettingsDialog::SettingsDialog(QWidget* parent)
  : QDialog(parent)
{
}


void SettingsDialog::setModel(StreamModel* stream)
{
    m_stream = stream;
}
