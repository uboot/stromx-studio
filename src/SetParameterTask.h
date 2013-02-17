/* 
*  Copyright 2012 Matthias Fuchs
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

#ifndef SETPARAMETERTASK_H
#define SETPARAMETERTASK_H

#include <stromx/runtime/DataRef.h>
#include "ErrorData.h"
#include "Task.h"
#include "model/OperatorModel.h"

/** 
 * \brief Task which asynchronously sets the value of an operator parameter.
 *
 * This class asynchronously tries to write the value of an operator parameter
 * within a certain timeout. After it was either successful or reached the timeout a finish
 * signal is emitted and the task destroys itself. If the object is deleted while waiting for
 * task to finish the destructor will stop until the task is finished.
 */
class SetParameterTask : public Task
{
    Q_OBJECT
    
public:
    enum ErrorCode
    {
        NO_ERROR,
        TIMED_OUT,
        EXCEPTION
    };
    
    /** Constructs a task object for the given parameters. Call run() to actually start the task. */
    explicit SetParameterTask(stromx::runtime::Operator* op, unsigned int id,
                              const stromx::runtime::DataRef & value, int timeout,
                              QObject* parent = 0);
    
    /** Returns the parameter ID specified in the constructor. */
    unsigned int id() const { return m_id; }
    
    /** Returns whether the an error occured when the attempt was made to write the parameter. */
    ErrorCode error() const { return m_errorCode; }
    
    /** Returns a message explaining any errors which happened while writing the parameter. */
    const ErrorData & errorData() const { return m_errorData; }
    
private:
    /** Tries to set the parameter and stores any error data in the class members. */
    void run();
    
    stromx::runtime::Operator* m_op;
    unsigned int m_id;
    stromx::runtime::DataRef m_data;
    ErrorCode m_errorCode;
    ErrorData m_errorData;
    int m_timeout;
};

#endif // SETPARAMETERTASK_H
