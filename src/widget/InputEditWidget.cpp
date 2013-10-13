#include "widget/InputEditWidget.h"

#include <QCheckBox>
#include <QComboBox>
#include <QFormLayout>
#include <QLabel>

#include "Common.h"

InputEditWidget::InputEditWidget(QWidget* parent)
  : InputWidget(parent),
    m_activeCheckBox(0),
    m_colorComboBox(0),
    m_visualizationTypeComboBox(0)
{
    QFormLayout* layout = new QFormLayout();

    m_activeCheckBox = new QCheckBox(tr("Active"));
    connect(m_activeCheckBox, SIGNAL(stateChanged(int)),
            this, SIGNAL(dataChanged()));
    layout->addRow(m_activeCheckBox);
    
    m_visualizationTypeComboBox = new QComboBox();
    m_visualizationTypeComboBox->addItems(visualizationLabels());
    connect(m_visualizationTypeComboBox, SIGNAL(currentIndexChanged(int)),
            this, SIGNAL(dataChanged()));
    layout->addRow(tr("Visualization"), m_visualizationTypeComboBox);
    
    m_colorComboBox = new QComboBox();
    int i = 0;
    QMapIterator<QString, QColor> iter(colorTable());
    while (iter.hasNext())
    {
        iter.next();
        m_colorComboBox->insertItem(i, iter.key());
        m_colorComboBox->setItemData(i, iter.value(), Qt::DecorationRole);
        ++i;
    }
    connect(m_colorComboBox, SIGNAL(currentIndexChanged(int)),
            this, SIGNAL(dataChanged()));
    layout->addRow(tr("Color"), m_colorComboBox);
    
    setLayout(layout);
}

bool InputEditWidget::inputActive() const
{
    return m_activeCheckBox->isChecked();
}

const QColor InputEditWidget::inputColor() const
{
    return colorTable().value(m_colorComboBox->currentText(), Qt::black);
}

int InputEditWidget::visualizationType() const
{
    return m_visualizationTypeComboBox->currentIndex();
}

void InputEditWidget::setInputTitle(const QString& title)
{
    setTitle(title);
}

void InputEditWidget::setInputActive(const bool isActive)
{
    m_activeCheckBox->setChecked(isActive);
}

void InputEditWidget::setInputColor(const QColor& color)
{
    int index = m_colorComboBox->findData(color, int(Qt::DecorationRole));
    m_colorComboBox->setCurrentIndex(index);
}

void InputEditWidget::setVisualizationType(const int type)
{
    m_visualizationTypeComboBox->setCurrentIndex(type);
}


