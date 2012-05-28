#include "DataConverterTest.h"

#include <QtTest/QtTest>

#include "../DataConverter.h"
#include <stromx/core/Primitive.h>
#include <stromx/core/String.h>

void DataConverterTest::testStromxDataEqualsTarget()
{
    QCOMPARE(false, DataConverter::stromxDataEqualsTarget(stromx::core::Bool(), stromx::core::UInt32()));
    QCOMPARE(true, DataConverter::stromxDataEqualsTarget(stromx::core::Bool(), stromx::core::Bool()));
    QCOMPARE(false, DataConverter::stromxDataEqualsTarget(stromx::core::Bool(true), stromx::core::Bool(false)));
}
