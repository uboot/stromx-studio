#include "InputData.h"

#include <QStringList>
#include <stromx/core/Operator.h>
#include "InputModel.h"
#include "OperatorModel.h"

InputData::InputData(OperatorModel* op, unsigned int id)
  : m_id(id),
    m_op(op),
    m_input(0),
    m_sourceObserver(0),
    m_sourcePosition(-1)
{
    setText(QString("%1/%2").arg(QString::fromStdString(m_op->op()->name()), m_id));
}

InputData::InputData(InputModel* input, ObserverModel* sourceObserver, int sourcePosition)
  : m_id(input->id()),
    m_op(input->op()),
    m_input(input),
    m_sourceObserver(sourceObserver),
    m_sourcePosition(sourcePosition)
{
    setText(QString("%1/%2").arg(QString::fromStdString(m_op->op()->name()), m_id));
}

QStringList InputData::formats () const
{
    return QStringList("stromx/input");
}