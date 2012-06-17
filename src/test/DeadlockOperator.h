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

#ifndef DEADLOCKOPERATOR_H
#define DEADLOCKOPERATOR_H

#include <stromx/core/OperatorKernel.h>
#include <stromx/core/Primitive.h>

class DeadlockOperator : public stromx::core::OperatorKernel
{
    enum Inputs 
    { 
        INPUT,
        OUTPUT
    };
    
    enum Parameters
    { 
        LOCK_PARAMETERS,
        LOCK_DATA
    };
    
public:
    DeadlockOperator();
    
    virtual OperatorKernel* const clone() const { return new DeadlockOperator; }
    virtual void setParameter(const unsigned int id, const stromx::core::Data& value);
    const stromx::core::Data& getParameter(const unsigned int id) const;
    virtual void execute(stromx::core::DataProvider& provider);
    
private:
    static const std::vector<const stromx::core::Description*> setupInputs();
    static const std::vector<const stromx::core::Description*> setupOutputs();
    static const std::vector<const stromx::core::Parameter*> setupParameters();
    
    static const std::string TYPE;
    static const std::string PACKAGE;
    static const stromx::core::Version VERSION;
    
    stromx::core::Bool m_lockParameters;
    stromx::core::Bool m_lockData;
};

#endif // DEADLOCKOPERATOR_H
