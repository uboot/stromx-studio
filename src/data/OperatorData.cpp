#include "data/OperatorData.h"


OperatorData::OperatorData(const QString& package, const QString& type)
  : m_package(package),
    m_type(type)
{
    setText(m_package + "/" + m_type);
}

