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

#ifndef REMOVECONNECTIONCMD_H
#define REMOVECONNECTIONCMD_H

#include <QUndoCommand>

class ConnectionModel;
class StreamModel;

/** \brief Removes a connection between operators from the stream. */
class RemoveConnectionCmd : public QUndoCommand
{
public:
    RemoveConnectionCmd(StreamModel* stream, ConnectionModel* model, QUndoCommand* parent = 0);
    
    virtual void undo();
    virtual void redo();
    
private:
    StreamModel* m_stream;
    ConnectionModel* m_model;
};

#endif // REMOVECONNECTIONCMD_H
