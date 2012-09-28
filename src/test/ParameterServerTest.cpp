#include "ParameterServerTest.h"

#include "ParameterOperator.h"
#include "../ParameterServer.h"
#include <QUndoStack>
#include <stromx/core/Operator.h>

ParameterServerTest::ParameterServerTest()
  : m_op(new stromx::core::Operator(new ParameterOperator())),
    m_server(new ParameterServer(m_op, new QUndoStack(this), this))
{
}

ParameterServerTest::~ParameterServerTest()
{
    delete m_op;
}

void ParameterServerTest::testGetParameter()
{

}

void ParameterServerTest::testSetParameter()
{

}

void ParameterServerTest::testRefresh()
{

}
