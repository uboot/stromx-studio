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

#ifndef ADDCONNECTIONCMD_H
#define ADDCONNECTIONCMD_H

#include <QUndoCommand>
#include <QPointF>

class StreamModel;

class ConnectionModel;
class StreamModel;

class AddConnectionCmd : public QUndoCommand
{
public:
    AddConnectionCmd(StreamModel* stream, ConnectionModel* model, QUndoCommand* parent = 0);
    
    virtual void undo();
    virtual void redo();
    
private:
    StreamModel* m_stream;
    ConnectionModel* m_model;
};

#endif // ADDCONNECTIONCMD_H
