#include "TestOperator.h"

#include <stromx/core/DataProvider.h>
#include <stromx/core/Id2DataComposite.h>
#include <stromx/core/Id2DataPair.h>
#include <stromx/core/OperatorException.h>
#include <stromx/core/Primitive.h>

using namespace stromx::core;

const std::string TestOperator::TYPE("TestOperator");
const std::string TestOperator::PACKAGE("TestPackage");
const Version TestOperator::VERSION(1, 2, 3);

TestOperator::TestOperator()
  : OperatorKernel(TYPE, PACKAGE, VERSION, setupInitInputs(), setupInitOutputs(), setupInitParameters())
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

const std::vector< const Description* > TestOperator::setupInitInputs()
{
    std::vector<const Description*> inputs;
    
    return inputs;
}
        
const std::vector<const Description*> TestOperator::setupInputs()
{
    std::vector<const Description*> inputs;
    
    return inputs;
}

const std::vector<const Description*> TestOperator::setupInitOutputs()
{
    std::vector<const Description*> outputs;
    
    return outputs;
}

const std::vector<const Description*> TestOperator::setupOutputs()
{
    std::vector<const Description*> outputs;
    
    return outputs;
}

const std::vector<const Parameter*> TestOperator::setupParameters()
{
    std::vector<const Parameter*> parameters;
    
    return parameters;
}

const std::vector<const Parameter*> TestOperator::setupInitParameters()
{
    std::vector<const Parameter*> parameters;
    
    return parameters;
}
