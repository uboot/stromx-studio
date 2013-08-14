#include "test/TestDataOperator.h"

#include <stromx/example/Matrix.h>
#include <stromx/runtime/DataProvider.h>
#include <stromx/runtime/EnumParameter.h>
#include <stromx/runtime/Id2DataPair.h>
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
    default:
        throw WrongParameterId(id, *this);
    }
}  

void TestDataOperator::execute(DataProvider& provider)
{
    Data* data = 0;
    switch(m_dataType)
    {
    case LINE_SEGMENTS:
    {
        Matrix* segments = new stromx::example::Matrix(2, 4, Matrix::FLOAT_64);
        double* doubleData = reinterpret_cast<double*>(segments->data());
        
        data = segments;
        break;
    }
    case STRING:
        data = new stromx::runtime::String("Random string");
        break;
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
    
    EnumParameter* param = new EnumParameter(DATA_TYPE);
    param->setTitle("Data type");
    param->setAccessMode(Parameter::ACTIVATED_WRITE);
    param->add(EnumDescription(Enum(LINE_SEGMENTS), "Line segments"));
    param->add(EnumDescription(Enum(STRING), "String"));
    parameters.push_back(param);
    
    return parameters;
}

