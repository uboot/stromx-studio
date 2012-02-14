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

#ifndef TESTOPERATOR_H
#define TESTOPERATOR_H

#include <stromx/core/OperatorKernel.h>
#include <stromx/core/Primitive.h>

class TestOperator : public stromx::core::OperatorKernel
{
    enum Inputs 
    { 
        INPUT_1, 
        INPUT_2
    };
    
    enum Outputs 
    { 
        OUTPUT_1, 
        OUTPUT_2
    };
    
    enum Parameters
    { 
        INITIALIZE_PARAM,
        INT_PARAM,
        ENUM_PARAM
    };
    
public:
    TestOperator();
    
    virtual OperatorKernel* const clone() const { return new TestOperator; }
    virtual void setParameter(const unsigned int id, const stromx::core::Data& value);
    const stromx::core::Data& getParameter(const unsigned int id) const;
    virtual void execute(stromx::core::DataProvider& provider);
    virtual void initialize();
    
private:
    static const std::vector<const stromx::core::Description*> setupInputs();
    static const std::vector<const stromx::core::Description*> setupOutputs();
    static const std::vector<const stromx::core::Parameter*> setupInitParameters();
    static const std::vector<const stromx::core::Parameter*> setupParameters();
    
    static const std::string TYPE;
    static const std::string PACKAGE;
    static const stromx::core::Version VERSION;
    
    stromx::core::UInt32 m_initializeParam;
    stromx::core::Int32 m_intParam;
    stromx::core::Enum m_enumParam;
};

#endif // TESTOPERATOR_H
