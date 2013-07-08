/* 
*  Copyright 2012 Matthias Fuchs
*
*  This file is part of stromx-studio.
*
*  Stromx-studio is free software: you can redistribute it and/or modify
*  it under the terms of the GNU General Public License as published by
*  the Free Software Foundation, either version 3 of the License, or
*  (at your option) any later version.
*
*  Stromx-studio is distributed in the hope that it will be useful,
*  but WITHOUT ANY WARRANTY; without even the implied warranty of
*  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
*  GNU General Public License for more details.
*
*  You should have received a copy of the GNU General Public License
*  along with stromx-studio.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef IMAGE_H
#define IMAGE_H

#include <QImage>
#include <QUndoCommand>
#include <stromx/runtime/ImageWrapper.h>
#include <stromx/runtime/Version.h>

/** \brief Stromx image wrapper for QImage objects. */
class Image : public stromx::runtime::ImageWrapper
{
public:
    /** Constructs an image from a file. */
    Image(const QString & fileName);
    
    /** Constructs an image from a QImage object. */
    Image(const QImage & image);
    
    /** Copy constructs an image from another image. */
    Image(const Image & image);
            
    virtual const stromx::runtime::Version & version() const { return VERSION; }
    virtual const std::string & type() const { return TYPE; }
    virtual const std::string & package() const { return PACKAGE; }
    
    virtual Data* clone() const;
    
private:
    static const std::string TYPE;
    static const std::string PACKAGE;
    static const stromx::runtime::Version VERSION;
    
    virtual void allocate(const unsigned int width, const unsigned int height,
                          const Image::PixelType pixelType);
                  
    /** 
     * Initializes the stromx::runtime::ImageWrapper parent
     * from the information in m_image.
     */
    void initializeParent();
    
    /** 
     * Helper function for the constructors which initializes the object 
     * from a QImage object.
     */
    void constructFromImage(const QImage & image);
            
    QImage m_image;
};

#endif // IMAGE_H
