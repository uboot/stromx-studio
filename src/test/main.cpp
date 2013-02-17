#include <QtTest/QtTest>

#include "test/DataConverterTest.h"
#include "test/ImageTest.h"
#include "test/ParameterServerTest.h"
#include "test/StreamModelTest.h"

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);
    
    DataConverterTest dataConverter;
    QTest::qExec(&dataConverter, argc, argv);
    
    ImageTest image;
    QTest::qExec(&image, argc, argv);
    
    ParameterServerTest parameterServer;
    QTest::qExec(&parameterServer, argc, argv);
    
    StreamModelTest streamModel;
    QTest::qExec(&streamModel, argc, argv);
}