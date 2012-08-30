#include "ParameterOperator.h"

#include <stromx/core/DataProvider.h>
#include <stromx/core/EnumParameter.h>
#include <stromx/core/Id2DataComposite.h>
#include <stromx/core/Id2DataPair.h>
#include <stromx/core/NumericParameter.h>
#include <stromx/core/OperatorException.h>
#include <stromx/core/ParameterGroup.h>
#include <stromx/core/Primitive.h>

using namespace stromx::core;

const std::string ParameterOperator::TYPE("ParameterOperator");
const std::string ParameterOperator::PACKAGE("TestPackage");
const Version ParameterOperator::VERSION(1, 2, 3);

ParameterOperator::ParameterOperator()
  : OperatorKernel(TYPE, PACKAGE, VERSION, setupInitParameters())
{
}

void ParameterOperator::initialize()
{
    OperatorKernel::initialize(setupInputs(), setupOutputs(), setupParameters());
}

void ParameterOperator::setParameter(unsigned int id, const Data& value)
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
        case BOOL_PARAM:
            m_boolParam = data_cast<const Bool &>(value);
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

const Data& ParameterOperator::getParameter(const unsigned int id) const
{
    switch(id)
    {
    case INT_PARAM:
        return m_intParam;
    case INITIALIZE_PARAM:
        return m_initializeParam;
    case ENUM_PARAM:
        return m_enumParam;
    case BOOL_PARAM:
        return m_boolParam;
    default:
        throw WrongParameterId(id, *this);
    }
}  

void ParameterOperator::execute(DataProvider& provider)
{
    Id2DataPair input1(INPUT_1);
    Id2DataPair input2(INPUT_2);
    
    provider.receiveInputData(input1 && input2);
    
    // execute...
    
    Id2DataPair output1(OUTPUT_1, input1.data());
    Id2DataPair output2(OUTPUT_2, input2.data());
    provider.sendOutputData(output1 && output2);
}
        
const std::vector<const Description*> ParameterOperator::setupInputs()
{
    std::vector<const Description*> inputs;
    Description* description = 0;
    
    description = new Description(INPUT_1, DataVariant::NONE);
    description->setDoc("Input 1");
    inputs.push_back(description);
    
    description = new Description(INPUT_2, DataVariant::NONE);
    description->setDoc("Input 2");
    inputs.push_back(description);
    
    return inputs;
}

const std::vector<const Description*> ParameterOperator::setupOutputs()
{
    std::vector<const Description*> outputs;
    Description* description = 0;
    
    description = new Description(OUTPUT_1, DataVariant::NONE);
    description->setDoc("Output 1");
    outputs.push_back(description);
    
    description = new Description(OUTPUT_2, DataVariant::NONE);
    description->setDoc("Output 2");
    outputs.push_back(description);
    
    return outputs;
}

const std::vector<const Parameter*> ParameterOperator::setupParameters()
{
    std::vector<const Parameter*> parameters;
    
    ParameterGroup* group1 = new ParameterGroup(PARAM_GROUP_1);
    group1->setDoc("Group 1");
    parameters.push_back(group1);
    
    ParameterGroup* group2 = new ParameterGroup(PARAM_GROUP_2, group1);
    group2->setDoc("Group 2");
    parameters.push_back(group2);
    
    NumericParameter<Int32>* intParam = new NumericParameter<Int32>(INT_PARAM, group1);
    intParam->setDoc("Integer parameter");
    intParam->setMax(Int32(1000));
    intParam->setMin(Int32(-1000));
    intParam->setAccessMode(Parameter::INITIALIZED_WRITE);
    parameters.push_back(intParam);
    
    EnumParameter* enumParam = new EnumParameter(ENUM_PARAM, group2);
    enumParam->setDoc("Enum parameter");
    enumParam->setAccessMode(Parameter::ACTIVATED_WRITE);
    enumParam->add(EnumDescription(Enum(0), "Choice 0"));
    enumParam->add(EnumDescription(Enum(1), "Choice 1"));
    enumParam->add(EnumDescription(Enum(2), "Choice 2"));
    parameters.push_back(enumParam);
    
    Parameter* boolParam = new Parameter(BOOL_PARAM, DataVariant::BOOL);
    boolParam->setDoc("Boolean parameter");
    boolParam->setAccessMode(Parameter::INITIALIZED_WRITE);
    parameters.push_back(boolParam);
    
    return parameters;
}

const std::vector<const Parameter*> ParameterOperator::setupInitParameters()
{
    std::vector<const Parameter*> parameters;
    
    Parameter* initializeParam = new Parameter(INITIALIZE_PARAM, DataVariant::UINT_32);
    initializeParam->setDoc("Initialize parameter");
    initializeParam->setAccessMode(Parameter::NONE_WRITE);
    parameters.push_back(initializeParam);
    
    return parameters;
}
