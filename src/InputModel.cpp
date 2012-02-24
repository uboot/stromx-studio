#include "InputModel.h"

InputModel::InputModel(OperatorModel* op, unsigned int id, QUndoStack* undoStack, QObject* parent)
  : QObject(parent),
    m_op(op),
    m_id(id),
    m_undoStack(undoStack) 
{

}

InputModel::InputModel(QUndoStack* undoStack, QObject* parent)
  : QObject(parent),
    m_op(0),
    m_id(0),
    m_undoStack(undoStack) 
{

}

QDataStream& operator<<(QDataStream& stream, const InputModel* model)
{
    return stream;
}

QDataStream& operator>>(QDataStream& stream, InputModel* model)
{
    return stream;
}
