#include "ExceptionOperator.h"

#include <stromx/core/DataProvider.h>
#include <stromx/core/Id2DataPair.h>
#include <stromx/core/OperatorException.h>
#include <stromx/core/Primitive.h>

using namespace stromx::core;

const std::string ExceptionOperator::TYPE("ExceptionOperator");
const std::string ExceptionOperator::PACKAGE("TestPackage");
const Version ExceptionOperator::VERSION(1, 2, 3);

ExceptionOperator::ExceptionOperator()
  : OperatorKernel(TYPE, PACKAGE, VERSION,
                   setupInputs(), setupOutputs(),
                   setupParameters())
{
}

void ExceptionOperator::setParameter(unsigned int id, const Data& value)
{
    try
    {
        switch(id)
        {
        case THROW_INITIALIZE:
            m_throwInitialize = data_cast<const Bool &>(value);
            break;
        case THROW_ACTIVATE:
            m_throwActivate = data_cast<const Bool &>(value);
            break;
        case THROW_EXECUTE:
            m_throwExecute = data_cast<const Bool &>(value);
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

const Data& ExceptionOperator::getParameter(const unsigned int id) const
{
    switch(id)
    {
    case THROW_INITIALIZE:
        return m_throwInitialize;
    case THROW_ACTIVATE:
        return m_throwActivate;
    case THROW_EXECUTE:
        return m_throwExecute;
    default:
        throw WrongParameterId(id, *this);
    }
}  

void ExceptionOperator::initialize()
{
    if(m_throwInitialize)
        throw OperatorError(*this, "Failed to initialize operator.");
}

void ExceptionOperator::activate()
{
    if(m_throwActivate)
        throw OperatorError(*this, "Failed to activate operator.");
}

void ExceptionOperator::execute(DataProvider& provider)
{
    Id2DataPair input(INPUT);
    
    provider.receiveInputData(input);
    
    provider.sleep(100);
    
    if(m_throwExecute)
        throw OperatorError(*this, "Failed to execute operator.");
}
        
const std::vector<const Description*> ExceptionOperator::setupInputs()
{
    std::vector<const Description*> inputs;
    Description* description = 0;
    
    description = new Description(INPUT, DataVariant::NONE);
    description->setDoc("Input");
    inputs.push_back(description);
    
    return inputs;
}

const std::vector<const Description*> ExceptionOperator::setupOutputs()
{
    std::vector<const Description*> outputs;
    
    return outputs;
}

const std::vector<const Parameter*> ExceptionOperator::setupParameters()
{
    std::vector<const Parameter*> parameters;
    
    Parameter* param = new Parameter(THROW_INITIALIZE, DataVariant::BOOL);
    param->setDoc("Exception on initialize");
    param->setAccessMode(Parameter::NONE_WRITE);
    parameters.push_back(param);
    
    param = new Parameter(THROW_ACTIVATE, DataVariant::BOOL);
    param->setDoc("Exception on activate");
    param->setAccessMode(Parameter::INITIALIZED_WRITE);
    parameters.push_back(param);
    
    param = new Parameter(THROW_EXECUTE, DataVariant::BOOL);
    param->setDoc("Exception on execute");
    param->setAccessMode(Parameter::ACTIVATED_WRITE);
    parameters.push_back(param);
    
    return parameters;
}

