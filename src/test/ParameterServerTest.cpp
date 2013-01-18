#include "ParameterServerTest.h"

#include "ParameterOperator.h"
#include "../ParameterServer.h"
#include <QUndoStack>
#include <QtTest/QtTest>
#include <stromx/runtime/Operator.h>

ParameterServerTest::ParameterServerTest()
  : m_op(new stromx::runtime::Operator(new ParameterOperator())),
    m_server(new ParameterServer(m_op, new QUndoStack(this), this))
{
    m_op->initialize();
    m_op->setParameter(ParameterOperator::INT_PARAM, stromx::runtime::Int32(10));
    m_server->refresh();
    QTest::qWait(100);
}

ParameterServerTest::~ParameterServerTest()
{
    delete m_op;
}

void ParameterServerTest::testGetParameter()
{
    QCOMPARE(QVariant(10), m_server->getParameter(ParameterOperator::INT_PARAM, Qt::DisplayRole));
    QCOMPARE(QVariant(tr("False")), m_server->getParameter(ParameterOperator::BOOL_PARAM, Qt::DisplayRole));
}

void ParameterServerTest::testSetParameter()
{
    m_server->setParameter(ParameterOperator::INT_PARAM, QVariant(20));
    QTest::qWait(1000);
    QCOMPARE(QVariant(20), m_server->getParameter(ParameterOperator::INT_PARAM, Qt::DisplayRole));
    
    m_server->setParameter(ParameterOperator::BOOL_PARAM, QVariant(1));
    QTest::qWait(1000);
    QCOMPARE(QVariant(tr("True")), m_server->getParameter(ParameterOperator::BOOL_PARAM, Qt::DisplayRole));
}

void ParameterServerTest::testRefresh()
{
    m_server->refresh();
}


