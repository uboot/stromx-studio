#include "DeadlockOperator.h"

#include <stromx/core/DataProvider.h>
#include <stromx/core/Id2DataPair.h>
#include <stromx/core/OperatorException.h>
#include <stromx/core/Primitive.h>
#include <boost/thread.hpp>

using namespace stromx::core;

const std::string DeadlockOperator::TYPE("DeadlockOperator");
const std::string DeadlockOperator::PACKAGE("TestPackage");
const Version DeadlockOperator::VERSION(1, 2, 3);

DeadlockOperator::DeadlockOperator()
  : OperatorKernel(TYPE, PACKAGE, VERSION,
                   setupInputs(), setupOutputs(),
                   setupParameters()),
    m_dataHasBeenLocked(false),
    m_writeAccess(stromx::core::DataContainer(new UInt32()))
{
}

void DeadlockOperator::setParameter(unsigned int id, const Data& value)
{
    try
    {
        switch(id)
        {
        case LOCK_PARAMETERS:
            m_lockParameters = data_cast<const Bool &>(value);
            break;
        case LOCK_DATA:
            m_lockData = data_cast<const Bool &>(value);
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

const Data& DeadlockOperator::getParameter(const unsigned int id) const
{
    switch(id)
    {
    case LOCK_PARAMETERS:
        return m_lockParameters;
    case LOCK_DATA:
        return m_lockData;
    default:
        throw WrongParameterId(id, *this);
    }
}  

void DeadlockOperator::execute(DataProvider& provider)
{
    Id2DataPair input(INPUT);
    provider.receiveInputData(input);
    
    if(m_lockParameters)
        boost::this_thread::sleep(boost::posix_time::seconds(5));
    
    if(! m_dataHasBeenLocked && m_lockData)
    {
        m_writeAccess = WriteAccess<UInt32>(input.data());
        m_dataHasBeenLocked = true;
    }
    else if(m_dataHasBeenLocked && ! m_lockData)
    {
        m_writeAccess = WriteAccess<UInt32>(DataContainer(new UInt32()));
        m_dataHasBeenLocked = false;
    }
    
    Id2DataPair output(OUTPUT, input.data());
    provider.sendOutputData(output);
}
        
const std::vector<const Description*> DeadlockOperator::setupInputs()
{
    std::vector<const Description*> inputs;
    Description* description = 0;
    
    description = new Description(INPUT, DataVariant::UINT_32);
    description->setDoc("Input");
    inputs.push_back(description);
    
    return inputs;
}

const std::vector<const Description*> DeadlockOperator::setupOutputs()
{
    std::vector<const Description*> outputs;
    Description* description = 0;
    
    description = new Description(OUTPUT, DataVariant::UINT_32);
    description->setDoc("Output");
    outputs.push_back(description);
    
    return outputs;
}

const std::vector<const Parameter*> DeadlockOperator::setupParameters()
{
    std::vector<const Parameter*> parameters;
    
    Parameter* param = new Parameter(LOCK_PARAMETERS, DataVariant::BOOL);
    param->setDoc("Deadlock parameter access");
    param->setAccessMode(Parameter::INITIALIZED_WRITE);
    parameters.push_back(param);
    
    param = new Parameter(LOCK_DATA, DataVariant::BOOL);
    param->setDoc("Deadlock data access");
    param->setAccessMode(Parameter::INITIALIZED_WRITE);
    parameters.push_back(param);
    
    return parameters;
}

