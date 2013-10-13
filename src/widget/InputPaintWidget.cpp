#include "widget/InputPaintWidget.h"

#include <QCheckBox>
#include <QFormLayout>
#include <QLabel>
#include <QPainter>

#include <Common.h>

InputPaintWidget::InputPaintWidget(QWidget* parent)
  : InputWidget(parent),
    m_activeCheckBox(new QCheckBox(tr("Active"))),
    m_colorLabel(new QLabel()),
    m_visualizationTypeLabel(new QLabel())
{
    QFormLayout* layout = new QFormLayout();
    layout->addRow(m_activeCheckBox);
    layout->addRow(tr("Visualization"), m_visualizationTypeLabel);
    layout->addRow(tr("Color"), m_colorLabel);
    
    setLayout(layout);
}

void InputPaintWidget::setInputTitle(const QString& title)
{
    setTitle(title);
}

void InputPaintWidget::setInputActive(const bool isActive)
{
    m_activeCheckBox->setChecked(isActive);
}

void InputPaintWidget::setInputColor(const QColor& color)
{
    QPixmap square(20, 20);
    QPainter painter(&square);
    painter.fillRect(square.rect(), color);
    m_colorLabel->setPixmap(square);
}

void InputPaintWidget::setVisualizationType(const int type)
{
    QString text = visualizationLabels()[type];
    m_visualizationTypeLabel->setText(text);
}

