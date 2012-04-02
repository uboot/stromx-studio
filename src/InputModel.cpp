#include "InputModel.h"

#include "OperatorModel.h"
#include "StreamModel.h"

InputModel::InputModel(OperatorModel* op, unsigned int id, QUndoStack* undoStack, QObject* parent)
  : QObject(parent),
    m_op(op),
    m_id(id),
    m_undoStack(undoStack) 
{
    connect(op, SIGNAL(nameChanged(QString)), this, SLOT(updateOperatorName(QString)));
}

void InputModel::updateOperatorName(const QString& name)
{
    emit changed(this);
}

