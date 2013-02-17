#include "TriggerButton.h"

#include <QFileDialog>
#include <QSettings>

TriggerButton::TriggerButton(QWidget* parent)
  : QPushButton(parent),
    m_triggerPending(false)
{
    connect(this, SIGNAL(clicked(bool)), this, SLOT(trigger()));
}

void TriggerButton::trigger()
{
    m_triggerPending = true;
    emit pushedTriggerButton();
}

void TriggerButton::reset()
{
    m_triggerPending = false;
}


