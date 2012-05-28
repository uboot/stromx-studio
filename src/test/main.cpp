#include <QtTest/QtTest>

#include "DataConverterTest.h"
#include "ImageTest.h"

int main(int argc, char *argv[])
{
    DataConverterTest dataConverter;
    QTest::qExec(&dataConverter, argc, argv);
    
    ImageTest image;
    QTest::qExec(&image, argc, argv);
}