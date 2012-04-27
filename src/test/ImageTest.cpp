#include "ImageTest.h"

#include <QtTest/QtTest>

#include "../Image.h"

void ImageTest::testFileConstructorColor()
{
     Image image("lenna.jpg");
     
     QCOMPARE(image.pixelType(), stromx::core::Image::RGB_24);
}

void ImageTest::testFileConstructorGray()
{
     Image image("lenna_bw.jpg");
     
     QCOMPARE(image.pixelType(), stromx::core::Image::MONO_8);
}
