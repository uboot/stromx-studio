#include "test/ObserverSchedulerTest.h"

#include "ObserverScheduler.h"

#include <QtTest/QtTest>

ObserverSchedulerTest::ObserverSchedulerTest() : QObject()
{}

void ObserverSchedulerTest::testScheduleLittle()
{
    ObserverScheduler scheduler(3, 100);
    
    QVERIFY(scheduler.schedule());
    QVERIFY(scheduler.schedule());
    QVERIFY(scheduler.schedule());
}

void ObserverSchedulerTest::testScheduleTooMany()
{
    ObserverScheduler scheduler(3, 100);
    
    QVERIFY(scheduler.schedule());
    QVERIFY(scheduler.schedule());
    QVERIFY(scheduler.schedule());
    QVERIFY(! scheduler.schedule());
    QVERIFY(! scheduler.schedule());
}

void ObserverSchedulerTest::testScheduleManyWithDelay()
{
    ObserverScheduler scheduler(3, 100);
    
    QVERIFY(scheduler.schedule());
    QVERIFY(scheduler.schedule());
    QVERIFY(scheduler.schedule());

    QTest::qWait(200);
    
    QVERIFY(scheduler.schedule());
}
