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

#include <stromx/core/Registry.h>

#include "DeadlockOperator.h"
#include "ExceptionOperator.h"
#include "ParameterOperator.h"
#include "RandomDataOperator.h"

#ifdef WIN32
    #define TEST_API __declspec(dllexport)
#else
    #define TEST_API
#endif // WIN32

extern "C"
{
    void TEST_API stromxRegisterTest(stromx::core::Registry& registry)
    {
        using namespace stromx::core;
        
        registry.registerOperator(new DeadlockOperator);
        registry.registerOperator(new ExceptionOperator);
        registry.registerOperator(new ParameterOperator);
        registry.registerOperator(new RandomDataOperator);
    }
}
