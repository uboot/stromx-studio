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

#ifndef SETPARAMETERCMD_H
#define SETPARAMETERCMD_H

#include <QUndoCommand>
#include <stromx/core/DataRef.h>

class ParameterServer;

/** \brief Sets the parameter of an operator. */
class SetParameterCmd : public QUndoCommand
{
public:
    SetParameterCmd(ParameterServer* server, unsigned int parameter,
                    const stromx::core::DataRef& newValue, const stromx::core::DataRef& oldValue,
                    QUndoCommand* parent = 0);
    virtual ~SetParameterCmd();
    virtual void undo();
    virtual void redo();
    
private:
    ParameterServer* m_server;
    unsigned int m_parameter;
    stromx::core::DataRef m_oldValue;
    stromx::core::DataRef m_newValue;
};

#endif // SETPARAMETERCMD_H
