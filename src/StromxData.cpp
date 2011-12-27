#include "StromxData.h"

#include <stromx/core/Operator.h>

StromxData::StromxData(const QList< stromx::core::Operator* >& operators)
  : m_operators(operators)
{

}

StromxData::StromxData(stromx::core::Operator*const op)
{
    m_operators.append(op);
}


const QList< stromx::core::Operator* > StromxData::useOperators()
{
    QList< stromx::core::Operator* > operators = m_operators;
    m_operators.clear();
    
    return operators;
}

StromxData::~StromxData()
{
    for(QList< stromx::core::Operator* >::iterator iter = m_operators.begin();
        iter != m_operators.end();
        ++iter)
    {
        delete (*iter);
    }
}
