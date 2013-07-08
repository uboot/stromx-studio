#include "Image.h"

#include "Config.h"

const std::string Image::TYPE = "Image";
const std::string Image::PACKAGE = STROMX_STUDIO_PACKAGE_NAME;
const stromx::runtime::Version Image::VERSION = stromx::runtime::Version(0, 1, 0);
               
Image::Image(const QString& fileName)
{
    QImage image = QImage(fileName);
    
    constructFromImage(image);
}

Image::Image(const QImage& image)
{
    constructFromImage(image);
}

Image::Image(const Image& image)
    : stromx::runtime::ImageWrapper()
{
    m_image = image.m_image;
    initializeParent();
}

void Image::constructFromImage(const QImage& image)
{
    switch(image.format())
    {
    case QImage::Format_Indexed8:
        m_image = image;
        break;
    case QImage::Format_RGB32:
    case QImage::Format_ARGB32:
        m_image = image.convertToFormat(QImage::Format_RGB888);
        break;
    default:
        return;
    }
    
    initializeParent();
}

stromx::runtime::Data* Image::clone() const
{
    return new Image(*this);
}

void Image::allocate(const unsigned int, const unsigned int, const Image::PixelType)
{
    throw stromx::runtime::NotImplemented("Image allocation is not implemented.");
}


void Image::initializeParent()
{
    stromx::runtime::Image::PixelType pixelType = stromx::runtime::Image::MONO_8;
    
    switch(m_image.format())
    {
    case QImage::Format_Indexed8:
        pixelType = stromx::runtime::Image::MONO_8;
        break;
    case QImage::Format_RGB888:
        pixelType = stromx::runtime::Image::RGB_24;
        break;
    default:
        m_image = QImage();
    }
    
    setBuffer(reinterpret_cast<uint8_t*>(m_image.bits()), m_image.byteCount());
    
    initializeImage(m_image.width(), m_image.height(), m_image.bytesPerLine(),
                    m_image.bits(), pixelType);
}
