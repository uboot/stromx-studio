#include "data/InputData.h"

#include <QStringList>
#include <stromx/runtime/Operator.h>
#include "model/InputModel.h"
#include "model/OperatorModel.h"

InputData::InputData(OperatorModel* op, unsigned int id)
  : m_id(id),
    m_op(op),
    m_input(0),
    m_sourceObserver(0),
    m_sourcePosition(-1)
{
    setText(QString("%1/%2").arg(QString::fromStdString(m_op->op()->name()), m_id));
}

InputData::InputData(InputModel* input, ObserverModel* sourceObserver, int sourcePosition, DragSource dragSource)
  : m_id(input->id()),
    m_op(input->op()),
    m_input(input),
    m_sourceObserver(sourceObserver),
    m_sourcePosition(sourcePosition),
    m_dragSource(dragSource)
{
    setText(QString("%1/%2").arg(QString::fromStdString(m_op->op()->name()), m_id));
}

QStringList InputData::formats () const
{
    return QStringList("stromx/input");
}

void InputData::setDragSource(InputData::DragSource dragSource)
{
    m_dragSource = dragSource;
}
