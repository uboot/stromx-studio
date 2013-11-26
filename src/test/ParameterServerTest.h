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

#ifndef PARAMETERSERVERTEST_H
#define PARAMETERSERVERTEST_H

#include <QObject>

class ParameterServer;

namespace stromx
{
    namespace runtime
    {
        class OperatorTester;
    }
}

class ParameterServerTest : public QObject
{
    Q_OBJECT
    
public:
    explicit ParameterServerTest();
    virtual ~ParameterServerTest();
    
private slots:
    void testGetParameter();
    void testSetParameter();
    void testRefresh();
    
private:
    stromx::runtime::OperatorTester* m_op;
    ParameterServer* m_server;
};

#endif // PARAMETERSERVERTEST_H
