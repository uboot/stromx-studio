#include "widget/InputPaintWidget.h"

#include <QLabel>
#include <QPainter>
#include <QVBoxLayout>

#include <Common.h>

InputPaintWidget::InputPaintWidget(QWidget* parent)
  : InputWidget(parent),
    m_activeLabel(new QLabel()),
    m_colorLabel(new QLabel()),
    m_visualizationTypeLabel(new QLabel())
{
    QVBoxLayout* layout = new QVBoxLayout();
    layout->addWidget(m_activeLabel);
    layout->addWidget(m_visualizationTypeLabel);
    layout->addWidget(m_colorLabel);
    
    setLayout(layout);
}

void InputPaintWidget::setInputTitle(const QString& title)
{
    setTitle(title);
}

void InputPaintWidget::setInputActive(const bool isActive)
{
    QString text = isActive ? tr("Active") : tr("Inactive");
    m_activeLabel->setText(text);
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

