#include "test/ObserverSchedulerTest.h"

#include "ObserverScheduler.h"

#include <QtTest/QtTest>

ObserverSchedulerTest::ObserverSchedulerTest()
  : QObject(),
    m_scheduler(new ObserverScheduler(3, 100))
{}

ObserverSchedulerTest::~ObserverSchedulerTest()
{
    delete m_scheduler;
}

void ObserverSchedulerTest::testSchedulePositive()
{
    QVERIFY(m_scheduler->schedule());
    QVERIFY(m_scheduler->schedule());
    QVERIFY(m_scheduler->schedule());
}
