#include "model/InputModel.h"

#include "Common.h"
#include "cmd/SetVisualizationPropertiesCmd.h"
#include "model/OperatorModel.h"
#include "model/StreamModel.h"
#include <stromx/runtime/Operator.h>

InputModel::InputModel(OperatorModel* op, unsigned int id, QUndoStack* undoStack, QObject* parent)
  : QObject(parent),
    m_op(op),
    m_id(id),
    m_undoStack(undoStack) 
{
    m_visualizationProperties["color"] = colorTable()["Red"];
    m_visualizationProperties["visualization"] = AbstractDataVisualizer::AUTOMATIC;
    m_visualizationProperties["active"] = true;
    
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

void InputModel::setVisualizationProperties(const VisualizationProperties & properties)
{
    if(properties != m_visualizationProperties)
    {
        QUndoCommand* cmd = new SetVisualizationPropertiesCmd(this, properties);
        m_undoStack->push(cmd);
    }
}

void InputModel::doSetVisualizationProperties(const VisualizationProperties & properties)
{
    m_visualizationProperties = properties;
    emit visualizationPropertiesChanged(m_visualizationProperties);
    emit changed(this);
}

QDataStream& operator<<(QDataStream& stream, const InputModel* model)
{
    stream << model->visualizationProperties();
    
    return stream;
}

QDataStream& operator>>(QDataStream& stream, InputModel* model)
{
    QMap<QString, QVariant> properties;
    
    stream >> properties;
    model->doSetVisualizationProperties(properties);
    
    return stream;
}



