#include "ParameterServer.h"

#include "DataConverter.h"
#include "GetParameterTask.h"
#include "SetParameterCmd.h"
#include "SetParameterTask.h"
#include <stromx/core/Operator.h>
#include <stromx/core/Trigger.h>
#include <stromx/core/OperatorException.h>

ParameterServer::ParameterServer(stromx::core::Operator* op, QUndoStack* undoStack, QObject* parent)
  : QObject(parent),
    m_op(op),
    m_undoStack(undoStack),
    m_accessTimeout(0)
{

}

const QVariant ParameterServer::getParameter(unsigned int id, int role)
{
    const stromx::core::Parameter & param = m_op->info().parameter(id);
    
    if(m_cache.find(id) == m_cache.end())
        return QVariant();
    
    ParameterValue & value = m_cache[id];
    
    if(value.state == CURRENT || role == Qt::EditRole)
    {
        return DataConverter::toQVariant(m_cache[id].value, param, role);
    }
    else
    {
        if(role == Qt::DisplayRole)
        {
            switch(value.state)
            {
                case GETTING:
                    return tr("Getting...");
                case SETTING:
                    return tr("Setting...");
                case TIMED_OUT:
                    return tr("Time out");
                default:
                    Q_ASSERT(false);
            }
        }
    }
    
    return QVariant();
}

bool ParameterServer::setParameter(unsigned int id, const QVariant& value)
{
    const stromx::core::Parameter & param = m_op->info().parameter(id);
    
    if(parameterIsWriteAccessible(param))
    {
        stromx::core::DataRef stromxData = DataConverter::toStromxData(value, param);
        
        if(stromxData.isNull())
            return false;
        
        // test if this data is trigger data
        try
        {
            stromx::core::data_cast<stromx::core::Trigger>(stromxData);
            
            // triggers are set without informing the undo stack (they can not 
            // be undone)
            doSetParameter(id, stromxData);
            
            return true;
        }
        catch(stromx::core::BadCast&)
        {
            // this is no trigger data so continue the exception and continue below
        }
        catch(stromx::core::Exception&)
        {
            // triggering failed
            return false;
        }
            
        // any other parameters are set via an undo stack command
        // obtain the current parameter value
        stromx::core::DataRef currentValue = m_cache[id].value;
        
        // if the new value is different from the old one
        // construct a set parameter command 
        if(! DataConverter::stromxDataEqualsTarget(stromxData, currentValue))
        {
            SetParameterCmd* cmd = new SetParameterCmd(this, id, currentValue, stromxData);
            m_undoStack->push(cmd);
        }
        return true;
    }
    else
    {
        return false;
    }
}

void ParameterServer::refresh()
{
    using namespace stromx::core;
    
    m_cache.clear();
    
    for(std::vector<const Parameter*>::const_iterator iter = m_op->info().parameters().begin();
        iter != m_op->info().parameters().end();
        ++iter)
    {
        const Parameter* param = *iter;
        
        refreshParameter(*param);
    }
}

void ParameterServer::refreshParameter(const stromx::core::Parameter & param)
{
    if(parameterIsReadAccessible(param))
    {
        GetParameterTask* task = new GetParameterTask(m_op, param.id(), m_accessTimeout, this);
        connect(task, SIGNAL(finished()), this, SLOT(handleGetParameterTaskFinished()));
        task->start();
        m_cache[param.id()].state = GETTING;
        emit parameterChanged(param.id());
    }
}

void ParameterServer::doSetParameter(unsigned int paramId, const stromx::core::DataRef& newValue)
{
    SetParameterTask* task = new SetParameterTask(m_op, paramId, newValue, m_accessTimeout, this);
    connect(task, SIGNAL(finished()), this, SLOT(handleSetParameterTaskFinished()));
    task->start();
    m_cache[paramId].state = SETTING;
    emit parameterChanged(paramId);
}

void ParameterServer::handleSetParameterTaskFinished()
{
    SetParameterTask* task = qobject_cast<SetParameterTask*>(sender());
    
    if(task)
    {
        const stromx::core::Parameter & param = m_op->info().parameter(task->id());
        refreshParameter(param);
        
        switch(task->error())
        {
        case GetParameterTask::TIMED_OUT:
            emit parameterAccessTimedOut();
            break;
        case GetParameterTask::EXCEPTION:
            emit parameterErrorOccurred(task->errorData());
            break;
        default:
            ;
        }
    }
}

Qt::ItemFlags ParameterServer::parameterFlags(unsigned int id) const
{
    const stromx::core::Parameter & param = m_op->info().parameter(id);
    
    if(parameterIsWriteAccessible(param))
        return Qt::ItemIsEditable;
    else
        return Qt::ItemFlags(0);
}

bool ParameterServer::parameterIsReadAccessible(const stromx::core::Parameter& par) const
{
    if(m_op->status() != stromx::core::Operator::NONE)
    {
        if (par.accessMode() == stromx::core::Parameter::NO_ACCESS)
            return false;
        else
            return true;
    }
    else
    {
        if(par.accessMode() == stromx::core::Parameter::NONE_READ || par.accessMode() == stromx::core::Parameter::NONE_WRITE)
            return true;
        else
            return false;
    }
}

void ParameterServer::setAccessTimeout(int timeout)
{
    m_accessTimeout = timeout >= 0 ? timeout : 0;
}

bool ParameterServer::parameterIsDisplayed(unsigned int id) const
{
    const stromx::core::Parameter & param = m_op->info().parameter(id);
    
    return parameterIsReadAccessible(param) || param.members().size();
}

bool ParameterServer::parameterIsWriteAccessible(const stromx::core::Parameter& par) const
{
    const ParameterValue & value = m_cache[par.id()];
    
    // parameter whose values are currently being updated can not be written
    if(value.state == GETTING || value.state == SETTING)
        return false;
    
    switch(m_op->status())
    {
        case stromx::core::Operator::NONE:
            if(par.accessMode() == stromx::core::Parameter::NONE_WRITE)
                return true;
            else
                return false;
            
        case stromx::core::Operator::INITIALIZED:
            if(par.accessMode() == stromx::core::Parameter::INITIALIZED_WRITE || par.accessMode() == stromx::core::Parameter::ACTIVATED_WRITE)
                return true;
            else
                return false;
            
        default:
            if(par.accessMode() == stromx::core::Parameter::ACTIVATED_WRITE)
                return true;
            else
                return false;
    }
}

void ParameterServer::handleGetParameterTaskFinished()
{
    GetParameterTask* task = qobject_cast<GetParameterTask*>(sender());

    if(task)
    {
        ParameterValue & value = m_cache[task->id()];
        switch(task->error())
        {
        case GetParameterTask::NO_ERROR:
            value.value = task->value();
            value.state = CURRENT;
            emit parameterChanged(task->id());
            break;
        case GetParameterTask::TIMED_OUT:
            value.state = TIMED_OUT;
            emit parameterAccessTimedOut();
            break;
        case GetParameterTask::EXCEPTION:
            value.state = ERROR;
            emit parameterErrorOccurred(task->errorData());
            break;
        default:
            Q_ASSERT(false);
        }
    }
}


