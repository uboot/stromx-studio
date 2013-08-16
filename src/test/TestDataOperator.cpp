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

#include <stromx/example/Matrix.h>
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
                   setupParameters())
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
            m_sizeX = data_cast<Enum>(value);
            break;
        case SIZE_Y:
            m_sizeY = data_cast<Enum>(value);
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
    default:
        throw WrongParameterId(id, *this);
    }
}  

void TestDataOperator::execute(DataProvider& provider)
{
    Data* data = 0;
    switch(m_dataType)
    {
    default:
        ;
    }
    
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
    
    return parameters;
}

