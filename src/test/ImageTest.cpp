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

void ImageTest::testFileConstructorNullImage()
{
    QImage nullImage;
    Image image(nullImage);

    QCOMPARE(image.width(), (unsigned int)(0));
    QCOMPARE(image.height(), (unsigned int)(0));
    QCOMPARE(image.data(), (uint8_t*)(0));
    QCOMPARE(image.pixelType(), stromx::core::Image::NONE);
}

