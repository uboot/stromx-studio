#include "delegate/InputEditWidget.h"

#include <QCheckBox>
#include <QComboBox>
#include <QFormLayout>
#include <QLabel>

#include "Common.h"
#include "visualization/Visualization.h"
#include "visualization/VisualizationRegistry.h"
#include "visualization/VisualizationState.h"
#include "visualization/VisualizationWidget.h"

InputEditWidget::InputEditWidget(const QModelIndex & index, QWidget* parent)
  : QGroupBox(parent),
    m_index(index),
    m_widget(0),
    m_activeCheckBox(0),
    m_layout(0)
{
    m_layout = new QFormLayout();

    m_activeCheckBox = new QCheckBox(tr("Active"));
    connect(m_activeCheckBox, SIGNAL(stateChanged(int)), this, SLOT(updateState()));
    m_layout->addRow(m_activeCheckBox);
    
    m_visualizationMenu = new QComboBox();
    m_visualizationMenu->addItems(VisualizationRegistry::visualizationNameList());
    m_visualizationMenu->setCurrentIndex(-1);
    connect(m_visualizationMenu, SIGNAL(currentIndexChanged(int)), this, SLOT(updateState()));
    connect(m_visualizationMenu, SIGNAL(currentIndexChanged(int)), this, SLOT(updateWidget()));
    m_layout->addRow(tr("Visualization"), m_visualizationMenu);
    
    connect(index.model(), SIGNAL(dataChanged(QModelIndex,QModelIndex)),
            this, SLOT(handleModelChanged(QModelIndex,QModelIndex)));
    
    setLayout(m_layout);
}

void InputEditWidget::setInputTitle(const QString& title)
{
    setTitle(title);
}

void InputEditWidget::updateState()
{
    m_state.setIsActive(m_activeCheckBox->isChecked());
    if (m_widget)
        m_state.currentProperties() = m_widget->getProperties();
    
    const Visualization* visualization = VisualizationRegistry::visualization(m_visualizationMenu->currentIndex());
    QString identifier = visualization ? visualization->visualization() : "";
    m_state.setCurrentVisualization(identifier);
    
    emit dataChanged();
}

void InputEditWidget::setState(const VisualizationState& state)
{
    m_state = state;
        
    m_activeCheckBox->setChecked(state.isActive());
    QString identifier = state.currentVisualization();
    int index = VisualizationRegistry::identifierToIndex(identifier);
    m_visualizationMenu->setCurrentIndex(index);
    if (m_widget)
        m_widget->setProperties(state.currentProperties());
}

void InputEditWidget::updateWidget()
{
    const Visualization* visualization = VisualizationRegistry::visualization(m_state.currentVisualization());
    if (visualization)
    {
        delete m_widget;
        m_widget = visualization->createEditor();
        if (m_widget)
        {
            m_layout->addRow(m_widget);
            m_widget->setProperties(m_state.currentProperties());
            connect(m_widget, SIGNAL(valueChanged()), this, SLOT(updateState()));
        }
    }
}

void InputEditWidget::handleModelChanged(const QModelIndex& topLeft, const QModelIndex& bottomRight)
{
    if (topLeft.row() >= m_index.row() && bottomRight.row() <= m_index.row())
    {
        QVariant stateVariant = m_index.data(VisualizationStateRole);
        if (stateVariant.canConvert<VisualizationState>())
        {
            VisualizationState state = stateVariant.value<VisualizationState>();
            if (state != m_state)
                setState(state);
        }
    }
}


