#include "delegate/InputPaintWidget.h"

#include <QCheckBox>
#include <QFormLayout>
#include <QLabel>
#include <QPainter>

#include <Common.h>

InputPaintWidget::InputPaintWidget(QWidget* parent)
  : QGroupBox(parent),
    m_activeLabel(new QLabel()),
    m_visualizationTypeLabel(new QLabel())
{
    QFormLayout* layout = new QFormLayout();
    layout->addRow(tr("Active"), m_activeLabel);
    layout->addRow(tr("Visualization"), m_visualizationTypeLabel);
    
    setLayout(layout);
}

void InputPaintWidget::setInputTitle(const QString& title)
{
    setTitle(title);
}

void InputPaintWidget::setInputActive(const bool isActive)
{
    QString text = isActive ? tr("yes") : tr("no");
    m_activeLabel->setText(text);
}

void InputPaintWidget::setVisualizationType(const QString & type)
{
    m_visualizationTypeLabel->setText(type);
}

