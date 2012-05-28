#include "TestOperator.h"

#include <stromx/core/DataProvider.h>
#include <stromx/core/EnumParameter.h>
#include <stromx/core/Id2DataComposite.h>
#include <stromx/core/Id2DataPair.h>
#include <stromx/core/NumericParameter.h>
#include <stromx/core/OperatorException.h>
#include <stromx/core/ParameterGroup.h>
#include <stromx/core/Primitive.h>

using namespace stromx::core;

const std::string TestOperator::TYPE("TestOperator");
const std::string TestOperator::PACKAGE("TestPackage");
const Version TestOperator::VERSION(1, 2, 3);

TestOperator::TestOperator()
  : OperatorKernel(TYPE, PACKAGE, VERSION, setupInitParameters())
{
}

void TestOperator::initialize()
{
    OperatorKernel::initialize(setupInputs(), setupOutputs(), setupParameters());
}

void TestOperator::setParameter(unsigned int id, const Data& value)
{
    try
    {
        switch(id)
        {
        case INT_PARAM:
            m_intParam = data_cast<const Int32 &>(value);
            break;
        case INITIALIZE_PARAM:
            m_initializeParam = data_cast<const UInt32 &>(value);
            break;
        case ENUM_PARAM:
            m_enumParam = data_cast<const Enum &>(value);
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

const Data& TestOperator::getParameter(const unsigned int id) const
{
    switch(id)
    {
    case INT_PARAM:
        return m_intParam;
    case INITIALIZE_PARAM:
        return m_initializeParam;
    case ENUM_PARAM:
        return m_enumParam;
    default:
        throw WrongParameterId(id, *this);
    }
}  

void TestOperator::execute(DataProvider& provider)
{
    Id2DataPair input1(INPUT_1);
    Id2DataPair input2(INPUT_2);
    
    provider.receiveInputData(input1 && input2);
    
    // execute...
    
    Id2DataPair output1(OUTPUT_1, input1.data());
    Id2DataPair output2(OUTPUT_2, input2.data());
    provider.sendOutputData(output1 && output2);
}
        
const std::vector<const Description*> TestOperator::setupInputs()
{
    std::vector<const Description*> inputs;
    Description* description = 0;
    
    description = new Description(INPUT_1, DataVariant::NONE);
    description->setName("Input 1");
    inputs.push_back(description);
    
    description = new Description(INPUT_2, DataVariant::NONE);
    description->setName("Input 2");
    inputs.push_back(description);
    
    return inputs;
}

const std::vector<const Description*> TestOperator::setupOutputs()
{
    std::vector<const Description*> outputs;
    Description* description = 0;
    
    description = new Description(OUTPUT_1, DataVariant::NONE);
    description->setName("Output 1");
    outputs.push_back(description);
    
    description = new Description(OUTPUT_2, DataVariant::NONE);
    description->setName("Output 2");
    outputs.push_back(description);
    
    return outputs;
}

const std::vector<const Parameter*> TestOperator::setupParameters()
{
    std::vector<const Parameter*> parameters;
    
    NumericParameter<Int32>* intParam = new NumericParameter<Int32>(INT_PARAM, DataVariant::INT_32);
    intParam->setName("Integer parameter");
    intParam->setMax(Int32(1000));
    intParam->setMin(Int32(-1000));
    intParam->setAccessMode(Parameter::INITIALIZED_WRITE);
    parameters.push_back(intParam);
    
    ParameterGroup* group = new ParameterGroup(PARAM_GROUP);
    group->setName("Group");
    parameters.push_back(group);
    
    EnumParameter* enumParam = new EnumParameter(ENUM_PARAM, group);
    enumParam->setName("Enum parameter");
    enumParam->setAccessMode(Parameter::ACTIVATED_WRITE);
    enumParam->add(EnumDescription(Enum(0), "Choice 0"));
    enumParam->add(EnumDescription(Enum(1), "Choice 1"));
    enumParam->add(EnumDescription(Enum(2), "Choice 2"));
    parameters.push_back(enumParam);
    
    return parameters;
}

const std::vector<const Parameter*> TestOperator::setupInitParameters()
{
    std::vector<const Parameter*> parameters;
    
    Parameter* initializeParam = new Parameter(INITIALIZE_PARAM, DataVariant::INT_32);
    initializeParam->setName("Initialize parameter");
    initializeParam->setAccessMode(Parameter::NONE_WRITE);
    parameters.push_back(initializeParam);
    
    return parameters;
}
