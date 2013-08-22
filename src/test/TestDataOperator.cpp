/* 
*  Copyright 2013 Matthias Fuchs
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

#include "test/TestDataOperator.h"

#include <cmath>

#include <stromx/cvsupport/Image.h>
#include <stromx/cvsupport/Matrix.h>
#include <stromx/runtime/DataProvider.h>
#include <stromx/runtime/EnumParameter.h>
#include <stromx/runtime/Id2DataPair.h>
#include <stromx/runtime/NumericParameter.h>
#include <stromx/runtime/OperatorException.h>
#include <stromx/runtime/String.h>

using namespace stromx::runtime;

const std::string TestDataOperator::TYPE("TestDataOperator");
const std::string TestDataOperator::PACKAGE("TestPackage");
const Version TestDataOperator::VERSION(1, 2, 3);

TestDataOperator::TestDataOperator()
  : OperatorKernel(TYPE, PACKAGE, VERSION,
                   setupInputs(), setupOutputs(),
                   setupParameters()),
    m_numBins(10),
    m_numLines(20),
    m_pointOffset(15),
    m_index(0)
{
}

void TestDataOperator::setParameter(unsigned int id, const Data& value)
{
    try
    {
        switch(id)
        {
        case DATA_TYPE:
            m_dataType = data_cast<Enum>(value);
            break;
        case OBJECT_TYPE:
            m_objectType = data_cast<Enum>(value);
            break;
        case SIZE_X:
            m_sizeX = data_cast<UInt32>(value);
            break;
        case SIZE_Y:
            m_sizeY = data_cast<UInt32>(value);
            break;
        case NUM_BINS:
            m_numBins = data_cast<UInt32>(value);
            break;
        case NUM_LINES:
            m_numBins = data_cast<UInt32>(value);
            break;
        case POINT_OFFSET:
            m_pointOffset = data_cast<UInt32>(value);
            break;
        default:
            throw WrongParameterId(id, *this);
        }
    }
    catch(std::bad_cast&)
    {
        throw WrongParameterType(*parameters()[id], *this);
    }
}

const DataRef TestDataOperator::getParameter(const unsigned int id) const
{
    switch(id)
    {
    case DATA_TYPE:
        return m_dataType;
    case OBJECT_TYPE:
        return m_objectType;
    case SIZE_X:
        return m_sizeX;
    case SIZE_Y:
        return m_sizeY;
    case NUM_BINS:
        return m_numBins;
    case NUM_LINES:
        return m_numLines;
    case POINT_OFFSET:
        return m_pointOffset;
    default:
        throw WrongParameterId(id, *this);
    }
}  

void TestDataOperator::activate()
{
    m_index = 0;
}

void TestDataOperator::execute(DataProvider& provider)
{
    Data* data = 0;
    switch(m_objectType)
    {
    case IMAGE_RAMP:
        data = imageRamp();
        break;
    case HISTOGRAM:
        data = histogram();
        break;
    case LINE_SEGMENTS:
        data = lineSegments();
        break;
    case POINTS:
        data = points();
        break;
    default:
        ;
    }
    
    m_index += 1;
    
    if(data)
    {
        Id2DataPair output(OUTPUT, DataContainer(data));
        provider.sendOutputData(output);
    }
}
        
const std::vector<const Description*> TestDataOperator::setupInputs()
{
    return std::vector<const Description*>();
}

const std::vector<const Description*> TestDataOperator::setupOutputs()
{
    std::vector<const Description*> outputs;
    
    Description* description = new Description(OUTPUT, DataVariant::NONE);
    description->setTitle("Output");
    outputs.push_back(description);
    
    return outputs;
}

const std::vector<const Parameter*> TestDataOperator::setupParameters()
{
    std::vector<const Parameter*> parameters;
    
    EnumParameter* enumParam = 0;
    NumericParameter<UInt32>* uint32Param = 0;
    
    enumParam = new EnumParameter(DATA_TYPE);
    enumParam->setTitle("Data type");
    enumParam->setAccessMode(Parameter::ACTIVATED_WRITE);
    enumParam->add(EnumDescription(Enum(IMAGE_MONO_8), "Image mono 8-bit"));
    enumParam->add(EnumDescription(Enum(IMAGE_MONO_16), "Image mono 16-bit"));
    enumParam->add(EnumDescription(Enum(IMAGE_RGB_24), "Image RGB 8-bit"));
    enumParam->add(EnumDescription(Enum(IMAGE_RGB_48), "Image RGB 16-bit"));
    enumParam->add(EnumDescription(Enum(MATRIX_FLOAT_32), "Matrix 32-bit float"));
    parameters.push_back(enumParam);
    
    enumParam = new EnumParameter(OBJECT_TYPE);
    enumParam->setTitle("Object type");
    enumParam->setAccessMode(Parameter::ACTIVATED_WRITE);
    enumParam->add(EnumDescription(Enum(IMAGE_RAMP), "Image ramp"));
    enumParam->add(EnumDescription(Enum(HISTOGRAM), "Histogram"));
    enumParam->add(EnumDescription(Enum(LINE_SEGMENTS), "Line segments"));
    enumParam->add(EnumDescription(Enum(POINTS), "Points"));
    parameters.push_back(enumParam);
    
    uint32Param = new NumericParameter<UInt32>(SIZE_X);
    uint32Param->setAccessMode(Parameter::ACTIVATED_WRITE);
    uint32Param->setTitle("Data size X");
    uint32Param->setMin(UInt32(0));
    parameters.push_back(uint32Param);
    
    uint32Param = new NumericParameter<UInt32>(SIZE_Y);
    uint32Param->setAccessMode(Parameter::ACTIVATED_WRITE);
    uint32Param->setTitle("Data size Y");
    uint32Param->setMin(UInt32(0));
    parameters.push_back(uint32Param);
    
    uint32Param = new NumericParameter<UInt32>(NUM_BINS);
    uint32Param->setAccessMode(Parameter::ACTIVATED_WRITE);
    uint32Param->setTitle("Number of histogram bins");
    uint32Param->setMin(UInt32(1));
    parameters.push_back(uint32Param);
    
    uint32Param = new NumericParameter<UInt32>(NUM_LINES);
    uint32Param->setAccessMode(Parameter::ACTIVATED_WRITE);
    uint32Param->setTitle("Number of lines");
    uint32Param->setMin(UInt32(1));
    parameters.push_back(uint32Param);
    
    uint32Param = new NumericParameter<UInt32>(POINT_OFFSET);
    uint32Param->setAccessMode(Parameter::ACTIVATED_WRITE);
    uint32Param->setTitle("Offset between points");
    uint32Param->setMin(UInt32(1));
    parameters.push_back(uint32Param);
    
    return parameters;
}

Data* TestDataOperator::imageRamp()
{
    Matrix* matrix = 0;
    unsigned int numChannels = 1;
    unsigned int rampHeight = 255;
    
    switch(m_dataType)
    {
    case IMAGE_MONO_8:
        matrix = new stromx::cvsupport::Image(m_sizeX, m_sizeY, Image::MONO_8);
        break;
    case IMAGE_MONO_16:
        matrix = new stromx::cvsupport::Image(m_sizeX, m_sizeY, Image::MONO_16);
        break;
    case IMAGE_RGB_24:
        matrix = new stromx::cvsupport::Image(m_sizeX, m_sizeY, Image::RGB_24);
        numChannels = 3;
        break;
    case IMAGE_RGB_48:
        matrix = new stromx::cvsupport::Image(m_sizeX, m_sizeY, Image::RGB_48);
        numChannels = 3;
        rampHeight = 256 * 255;
        break;
    case MATRIX_FLOAT_32:
        matrix = new stromx::cvsupport::Matrix(m_sizeY, m_sizeX, Matrix::FLOAT_32);
        rampHeight = 10000;
        break;
    default:
        throw WrongArgument("Unsupported data type.");
    }
    
    unsigned int RAMP_WIDTH = 255;
    const unsigned int STEP = rampHeight / RAMP_WIDTH;
    
    for (unsigned int row = 0; row < matrix->rows(); ++row)
    {
        unsigned int remainingPixels = m_sizeX;
        unsigned int activeChannel = 0;
        while(remainingPixels > 0)
        {
            unsigned int numPixels = std::min(remainingPixels, RAMP_WIDTH);
            unsigned int value = m_index % RAMP_WIDTH;
            for (unsigned int offset = 0; offset < numPixels; ++offset)
            {
                unsigned int pixel = m_sizeX - remainingPixels + offset;
                for (unsigned int channel = 0; channel < numChannels; ++channel)
                {
                    unsigned int subpixel = pixel * numChannels + channel;
                    if (channel == activeChannel)
                        setMatrixEntry(matrix, row, subpixel, value);
                    else
                        setMatrixEntry(matrix, row, subpixel, 0);
                }
                value += STEP;
                if (value > rampHeight)
                    value = 0;
            }
            remainingPixels -= numPixels;
            activeChannel = (activeChannel + 1) % numChannels;
        }
    }
    
    return matrix;
}

Data* TestDataOperator::histogram()
{
    Matrix* matrix = 0;
    
    switch(m_dataType)
    {
    case MATRIX_FLOAT_32:
        matrix = new stromx::cvsupport::Matrix(m_numBins, 1, Matrix::FLOAT_32);
        break;
    case MATRIX_FLOAT_64:
        matrix = new stromx::cvsupport::Matrix(m_numBins, 1, Matrix::FLOAT_64);
        break;
    default:
        throw WrongArgument("Unsupported data type.");
    }
    
    for (unsigned int i = 0; i < m_numBins; ++i)
    {
        float arg = 10.0 * float(i + m_index) / float(m_numBins);
        float value = 200 * fabs(sin(arg));
        setMatrixEntry(matrix, i, 0, value);
    }
    
    return matrix;
}

Data* TestDataOperator::lineSegments()
{
    Matrix* matrix = 0;
    
    switch(m_dataType)
    {
    case MATRIX_FLOAT_32:
        matrix = new stromx::cvsupport::Matrix(m_numLines, 4, Matrix::FLOAT_32);
        break;
    case MATRIX_FLOAT_64:
        matrix = new stromx::cvsupport::Matrix(m_numLines, 4, Matrix::FLOAT_64);
        break;
    default:
        throw WrongArgument("Unsupported data type.");
    }
    
    const float step = float(m_sizeX) / float(m_numLines);
    for (unsigned int i = 0; i < m_numLines; ++i)
    {
        const float offset = i * step + m_index % int(step);
        
        setMatrixEntry(matrix, i, 0, offset);
        setMatrixEntry(matrix, i, 1, 0);
        setMatrixEntry(matrix, i, 2, offset);
        setMatrixEntry(matrix, i, 3, m_sizeY);
    }
    
    return matrix;
}

Data* TestDataOperator::points()
{
    Matrix* matrix = 0;
    const unsigned int numPointsX = m_sizeX / m_pointOffset;
    const unsigned int numPointsY = m_sizeY / m_pointOffset;
    const unsigned int numPoints = numPointsX * numPointsY;
    
    switch(m_dataType)
    {
    case MATRIX_FLOAT_32:
        matrix = new stromx::cvsupport::Matrix(numPoints, 2, Matrix::FLOAT_32);
        break;
    case MATRIX_FLOAT_64:
        matrix = new stromx::cvsupport::Matrix(numPoints, 2, Matrix::FLOAT_64);
        break;
    default:
        throw WrongArgument("Unsupported data type.");
    }
    
    for (unsigned int i = 0; i < numPointsX; ++i)
    {
        for (unsigned int j = 0; j < numPointsY; ++j)
        {
            const float xOffset = i * m_pointOffset + m_index % m_pointOffset;
            const float yOffset = j * m_pointOffset + m_index % m_pointOffset;
            
            const unsigned int pos = i * numPointsY + j;
            setMatrixEntry(matrix, pos, 0, xOffset);
            setMatrixEntry(matrix, pos, 1, yOffset);
        }
    }
    
    return matrix;
}

void TestDataOperator::setMatrixEntry(Matrix* matrix, unsigned int row, 
                                      unsigned int col, double value)
{   
    switch(m_dataType)
    {
    case IMAGE_MONO_8:
    case IMAGE_RGB_24:
        matrix->at<uint8_t>(row, col) = value;
        break;
    case IMAGE_MONO_16:
    case IMAGE_RGB_48:
        matrix->at<uint16_t>(row, col) = value;
        break;
    case MATRIX_FLOAT_32:
        matrix->at<float>(row, col) = value;
        break;
    default:
        throw WrongArgument("Unsupported data type.");
    }
}


