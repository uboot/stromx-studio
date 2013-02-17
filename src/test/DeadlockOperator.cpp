#include "test/DeadlockOperator.h"

#include <stromx/runtime/DataProvider.h>
#include <stromx/runtime/Id2DataPair.h>
#include <stromx/runtime/OperatorException.h>
#include <stromx/runtime/Primitive.h>

#ifdef WIN32
#include <Windows.h>
#endif // WIN32
#ifdef UNIX
#include <unistd.h>
#endif // UNIX


using namespace stromx::runtime;

const std::string DeadlockOperator::TYPE("DeadlockOperator");
const std::string DeadlockOperator::PACKAGE("TestPackage");
const Version DeadlockOperator::VERSION(1, 2, 3);

DeadlockOperator::DeadlockOperator()
  : OperatorKernel(TYPE, PACKAGE, VERSION,
                   setupInputs(), setupOutputs(),
                   setupParameters()),
    m_dataHasBeenLocked(false),
    m_writeAccess(stromx::runtime::DataContainer(new UInt32()))
{
}

void DeadlockOperator::setParameter(unsigned int id, const Data& value)
{
    try
    {
        switch(id)
        {
        case LOCK_PARAMETERS:
            m_lockParameters = data_cast<Bool>(value);
            break;
        case LOCK_DATA:
            m_lockData = data_cast<Bool>(value);
            break;
        case DUMMY:
            m_dummy = data_cast<UInt8>(value);
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

const DataRef DeadlockOperator::getParameter(const unsigned int id) const
{
    switch(id)
    {
    case LOCK_PARAMETERS:
        return m_lockParameters;
    case LOCK_DATA:
        return m_lockData;
    case DUMMY:
        return m_dummy;
    default:
        throw WrongParameterId(id, *this);
    }
}  

void DeadlockOperator::activate()
{
    m_dataHasBeenLocked = false;
}

void DeadlockOperator::execute(DataProvider& provider)
{
    Id2DataPair input(INPUT);
    provider.receiveInputData(input);
    
    if(m_lockParameters)
    {
#ifdef WIN32
        Sleep(1000); // sleep 1 second
#endif // WIN32
#ifdef UNIX
        sleep(1); // sleep 1 second
#endif // UNIX  
    }
    
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
    description->setTitle("Input");
    inputs.push_back(description);
    
    return inputs;
}

const std::vector<const Description*> DeadlockOperator::setupOutputs()
{
    std::vector<const Description*> outputs;
    Description* description = 0;
    
    description = new Description(OUTPUT, DataVariant::UINT_32);
    description->setTitle("Output");
    outputs.push_back(description);
    
    return outputs;
}

const std::vector<const Parameter*> DeadlockOperator::setupParameters()
{
    std::vector<const Parameter*> parameters;
    
    Parameter* param = new Parameter(LOCK_PARAMETERS, DataVariant::BOOL);
    param->setTitle("Deadlock parameter access");
    param->setAccessMode(Parameter::INITIALIZED_WRITE);
    parameters.push_back(param);
    
    param = new Parameter(LOCK_DATA, DataVariant::BOOL);
    param->setTitle("Deadlock data access");
    param->setAccessMode(Parameter::INITIALIZED_WRITE);
    parameters.push_back(param);
    
    param = new Parameter(DUMMY, DataVariant::UINT_8);
    param->setTitle("Dummy parameter");
    param->setAccessMode(Parameter::ACTIVATED_WRITE);
    parameters.push_back(param);
    
    return parameters;
}

