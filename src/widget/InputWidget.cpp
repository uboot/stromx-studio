#include "widget/InputWidget.h"

#include <QCheckBox>
#include <QComboBox>

#include <QFormLayout>
#include <QLabel>

InputWidget::InputWidget(QWidget* parent) : QGroupBox(parent)
{
    QFormLayout* layout = new QFormLayout();
    setLayout(layout);
}

void InputWidget::setInputTitle(const QString& title)
{
    setTitle(title);
}
