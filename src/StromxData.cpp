#include "StromxData.h"

#include <stromx/core/Operator.h>

StromxData::StromxData(const QSet< stromx::core::Operator* >& operators)
  : m_operators(operators)
{

}

StromxData::StromxData(stromx::core::Operator*const op)
{
    m_operators.insert(op);
}

void StromxData::deleteData()
{
    for(QSet< stromx::core::Operator* >::iterator iter = m_operators.begin();
        iter != m_operators.end();
        ++iter)
    {
        delete (*iter);
    }
}
