#include <QtTest/QtTest>

#include "ImageTest.h"



int main(int argc, char *argv[])
{
    ImageTest image;
    return QTest::qExec(&image, argc, argv);
}