#include "model/InputModel.h"

#include "Common.h"
#include "cmd/SetInputActiveCmd.h"
#include "cmd/SetInputColorCmd.h"
#include "cmd/SetInputVisualizationCmd.h"
#include "model/OperatorModel.h"
#include "model/StreamModel.h"
#include <stromx/runtime/Operator.h>

InputModel::InputModel(OperatorModel* op, unsigned int id, QUndoStack* undoStack, QObject* parent)
  : QObject(parent),
    m_op(op),
    m_id(id),
    m_active(true),
    m_color(*(colorTable().begin())),
    m_visualization(AbstractDataVisualizer::AUTOMATIC),
    m_undoStack(undoStack) 
{
    connect(op, SIGNAL(nameChanged(QString)), this, SLOT(updateOperatorName(QString)));
}

void InputModel::updateOperatorName(const QString&)
{
    emit changed(this);
}

QString InputModel::docTitle() const
{
    return QString::fromStdString(m_op->op()->info().input(m_id).title());
}

void InputModel::setActive(bool active)
{
    if(active != m_active)
    {
        QUndoCommand* cmd = new SetInputActiveCmd(this, active);
        m_undoStack->push(cmd);
    }
}

void InputModel::setColor(const QColor& color)
{
    if(color != m_color)
    {
        QUndoCommand* cmd = new SetInputColorCmd(this, color);
        m_undoStack->push(cmd);
    }
}

void InputModel::setVisualization(AbstractDataVisualizer::Visualization visualization)
{
    if(visualization != m_visualization)
    {
        QUndoCommand* cmd = new SetInputVisualizationCmd(this, visualization);
        m_undoStack->push(cmd);
    }
}

void InputModel::doSetActive(bool active)
{
    m_active = active;
    emit activeChanged(m_active);
    emit changed(this);
}

void InputModel::doSetColor(const QColor& color)
{
    m_color = color;
    emit colorChanged(m_color);
    emit changed(this);
}

void InputModel::doSetVisualization(const AbstractDataVisualizer::Visualization& visualization)
{
    m_visualization = visualization;
    emit visualizationChanged(m_visualization);
    emit changed(this);
}

QDataStream& operator<<(QDataStream& stream, const InputModel* model)
{
    QMap<QString, QVariant> properties;
    
    properties["active"] = model->active();
    properties["color"] = model->color();
    properties["visualization"] = qint32(model->visualization());
    
    stream << properties;
    
    return stream;
}

QDataStream& operator>>(QDataStream& stream, InputModel* model)
{
    QMap<QString, QVariant> properties;
    
    stream >> properties;
    
    QVariant active = properties.value("active", true);
    if(active.type() == QVariant::Bool)
        model->doSetActive(active.toBool());
    
    QVariant color = properties.value("color", Qt::black);
    if(color.type() == QVariant::Color)
        model->doSetColor(color.value<QColor>());
    
    QVariant visualization = properties.value("visualization", AbstractDataVisualizer::AUTOMATIC);
    if(visualization.type() == QVariant::Int)
        model->doSetVisualization(AbstractDataVisualizer::Visualization(visualization.toInt()));
    
    return stream;
}



