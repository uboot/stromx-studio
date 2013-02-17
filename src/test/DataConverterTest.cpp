#include "DataConverterTest.h"

#include <QtTest/QtTest>

#include "DataConverter.h"
#include <stromx/runtime/Primitive.h>
#include <stromx/runtime/String.h>

void DataConverterTest::testStromxDataEqualsTarget()
{
    QCOMPARE(false, DataConverter::stromxDataEqualsTarget(stromx::runtime::Bool(), stromx::runtime::UInt32()));
    QCOMPARE(true, DataConverter::stromxDataEqualsTarget(stromx::runtime::Bool(), stromx::runtime::Bool()));
    QCOMPARE(false, DataConverter::stromxDataEqualsTarget(stromx::runtime::Bool(true), stromx::runtime::Bool(false)));
}
