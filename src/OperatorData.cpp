#include "OperatorData.h"


OperatorData::OperatorData(const QString& package, const QString& name)
  : m_package(package),
    m_name(name)
{
    setText(m_package + "/" + m_name);
}

