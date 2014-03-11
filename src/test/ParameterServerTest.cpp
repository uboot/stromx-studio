#include "test/ParameterServerTest.h"

#include <QUndoStack>
#include <QtTest/QtTest>
#include <stromx/runtime/OperatorTester.h>
#include <stromx/test/ParameterOperator.h>
#include "ParameterServer.h"

ParameterServerTest::ParameterServerTest()
  : m_op(new stromx::runtime::OperatorTester(new stromx::test::ParameterOperator())),
    m_server(new ParameterServer(m_op, new QUndoStack(this), this))
{
    m_op->initialize();
    m_op->setParameter(stromx::test::ParameterOperator::INT_PARAM, stromx::runtime::Int32(10));
    m_server->refresh();
    QTest::qWait(100);
}

ParameterServerTest::~ParameterServerTest()
{
    delete m_op;
}

void ParameterServerTest::testGetParameter()
{
    QCOMPARE(QVariant(10), m_server->getParameter(stromx::test::ParameterOperator::INT_PARAM, Qt::DisplayRole));
    QCOMPARE(QVariant(tr("False")), m_server->getParameter(stromx::test::ParameterOperator::BOOL_PARAM, Qt::DisplayRole));
}

void ParameterServerTest::testSetParameter()
{
    m_server->setParameter(stromx::test::ParameterOperator::INT_PARAM, QVariant(20));
    QTest::qWait(1000);
    QCOMPARE(QVariant(20), m_server->getParameter(stromx::test::ParameterOperator::INT_PARAM, Qt::DisplayRole));
    
    m_server->setParameter(stromx::test::ParameterOperator::BOOL_PARAM, QVariant(1));
    QTest::qWait(1000);
    QCOMPARE(QVariant(tr("True")), m_server->getParameter(stromx::test::ParameterOperator::BOOL_PARAM, Qt::DisplayRole));
}

void ParameterServerTest::testRefresh()
{
    m_server->refresh();
}


