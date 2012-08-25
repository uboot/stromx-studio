#include "InputModel.h"

#include "Common.h"
#include "OperatorModel.h"
#include "SetInputColorCmd.h"
#include "SetInputVisualizationCmd.h"
#include "StreamModel.h"

InputModel::InputModel(OperatorModel* op, unsigned int id, QUndoStack* undoStack, QObject* parent)
  : QObject(parent),
    m_op(op),
    m_id(id),
    m_color(*(colorTable().begin())),
    m_visualization(AUTOMATIC),
    m_undoStack(undoStack) 
{
    connect(op, SIGNAL(nameChanged(QString)), this, SLOT(updateOperatorName(QString)));
}

void InputModel::updateOperatorName(const QString& name)
{
    emit changed(this);
}

void InputModel::setColor(const QColor& color)
{
    if(color != m_color)
    {
        QUndoCommand* cmd = new SetInputColorCmd(this, color);
        m_undoStack->push(cmd);
    }
}

void InputModel::setVisualization(Visualization visualization)
{
    if(visualization != m_visualization)
    {
        QUndoCommand* cmd = new SetInputVisualizationCmd(this, visualization);
        m_undoStack->push(cmd);
    }
}

void InputModel::doSetColor(const QColor& color)
{
    m_color = color;
    emit colorChanged(m_color);
    emit changed(this);
}

void InputModel::doSetVisualization(const InputModel::Visualization& visualization)
{
    m_visualization = visualization;
    emit visualizationChanged(m_visualization);
    emit changed(this);
}


