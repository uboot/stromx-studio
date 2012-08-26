#include "RandomDataOperator.h"

#include <stromx/base/Matrix.h>
#include <stromx/core/DataProvider.h>
#include <stromx/core/EnumParameter.h>
#include <stromx/core/Id2DataPair.h>
#include <stromx/core/OperatorException.h>

#include <cstdlib>

using namespace stromx::core;

const std::string RandomDataOperator::TYPE("RandomDataOperator");
const std::string RandomDataOperator::PACKAGE("TestPackage");
const Version RandomDataOperator::VERSION(1, 2, 3);

RandomDataOperator::RandomDataOperator()
  : OperatorKernel(TYPE, PACKAGE, VERSION,
                   setupInputs(), setupOutputs(),
                   setupParameters())
{
}

void RandomDataOperator::setParameter(unsigned int id, const Data& value)
{
    try
    {
        switch(id)
        {
        case DATA_TYPE:
            m_dataType = data_cast<const Enum &>(value);
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

const Data& RandomDataOperator::getParameter(const unsigned int id) const
{
    switch(id)
    {
    case DATA_TYPE:
        return m_dataType;
    default:
        throw WrongParameterId(id, *this);
    }
}  

void RandomDataOperator::execute(DataProvider& provider)
{
    Matrix* segments = new stromx::base::Matrix(2, 4, Matrix::DOUBLE);
    
    double* doubleData = reinterpret_cast<double*>(segments->data());
    doubleData[0] = 50 + uniform(20);
    doubleData[1] = 100 + uniform(20);
    doubleData[2] = 200 + uniform(20);
    doubleData[3] = 150 + uniform(20);
    
    doubleData[4] = 400 + uniform(20);
    doubleData[5] = 30 + uniform(20);
    doubleData[6] = 50 + uniform(20);
    doubleData[7] = 300 + uniform(20);
    
    Id2DataPair output(OUTPUT, DataContainer(segments));
    provider.sendOutputData(output);
}
        
const std::vector<const Description*> RandomDataOperator::setupInputs()
{
    return std::vector<const Description*>();
}

const std::vector<const Description*> RandomDataOperator::setupOutputs()
{
    std::vector<const Description*> outputs;
    
    Description* description = new Description(OUTPUT, DataVariant::NONE);
    description->setDoc("Output");
    outputs.push_back(description);
    
    return outputs;
}

const std::vector<const Parameter*> RandomDataOperator::setupParameters()
{
    std::vector<const Parameter*> parameters;
    
    EnumParameter* param = new EnumParameter(DATA_TYPE);
    param->setDoc("Data type");
    param->setAccessMode(Parameter::NONE_WRITE);
    param->add(EnumDescription(LINE_SEGMENTS, "Line segments"));
    parameters.push_back(param);
    
    return parameters;
}

double RandomDataOperator::uniform(double limit)
{
    return (double(rand()) / RAND_MAX - 0.5) * 2.0 * limit;
}


