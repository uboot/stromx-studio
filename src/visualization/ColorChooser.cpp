#include "ColorChooser.h"

#include <QComboBox>
#include <QFormLayout>
#include "Common.h"

ColorChooser::ColorChooser(QWidget* parent) : VisualizationWidget(parent)
{
    QFormLayout* layout = new QFormLayout();
    
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
    connect(m_colorComboBox, SIGNAL(currentIndexChanged(int)), this, SIGNAL(valueChanged()));
    layout->addRow(tr("Color"), m_colorComboBox);
    
    setLayout(layout);
}

QMap< QString, QVariant > ColorChooser::getProperties() const
{
    QMap<QString, QVariant> properties;
    QString colorText = m_colorComboBox->currentText();
    properties["color"] = colorTable().value(colorText, Qt::black);
    return properties;
}

void ColorChooser::setProperties(const QMap< QString, QVariant >& properties)
{
    QVariant colorVariant = properties.value("color", Qt::black);
    if (colorVariant.canConvert<QColor>())
    {
        QColor color = colorVariant.value<QColor>();
        int index = m_colorComboBox->findData(color, int(Qt::DecorationRole));
        m_colorComboBox->setCurrentIndex(index);
    }
}
