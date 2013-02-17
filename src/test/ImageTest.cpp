#include "ImageTest.h"

#include <QtTest/QtTest>

#include "Image.h"

void ImageTest::testFileConstructorColor()
{
    Image image("lenna.jpg");
     
    QCOMPARE(image.pixelType(), stromx::runtime::Image::RGB_24);  
}

void ImageTest::testFileConstructorGray()
{
    Image image("lenna_bw.jpg");
     
    QCOMPARE(image.pixelType(), stromx::runtime::Image::MONO_8);
}

void ImageTest::testFileConstructorNullImage()
{
    QImage nullImage;
    Image image(nullImage);

    QCOMPARE(image.width(), (unsigned int)(0));
    QCOMPARE(image.height(), (unsigned int)(0));
    QCOMPARE(image.data(), (uint8_t*)(0));
    QCOMPARE(image.pixelType(), stromx::runtime::Image::NONE);
}

void ImageTest::testQImageConstructor()
{
    QImage qImage("lenna.jpg");
    Image image(qImage);
    
    QImage out(image.data(), image.width(), image.height(), image.stride(), QImage::Format_RGB888);
    out.save("ImageTest_testQImageConstructor.png");
}


