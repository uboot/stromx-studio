#include "InputData.h"

#include <QStringList>
#include <stromx/core/Operator.h>
#include "InputModel.h"
#include "OperatorModel.h"

InputData::InputData(OperatorModel* op, unsigned int id)
  : m_id(id),
    m_op(op),
    m_input(0)
{
    setText(QString("%1/%2").arg(QString::fromStdString(m_op->op()->name()), m_id));
}

InputData::InputData(InputModel* input)
  : m_id(input->id()),
    m_op(input->op()),
    m_input(input)
{
    setText(QString("%1/%2").arg(QString::fromStdString(m_op->op()->name()), m_id));
}


QStringList InputData::formats () const
{
    return QStringList("stromx/input");
}