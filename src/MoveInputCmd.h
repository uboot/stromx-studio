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

#ifndef MOVEINPUTCMD_H
#define MOVEINPUTCMD_H

#include <QUndoCommand>

class InputModel;
class ObserverTreeModel;
class StreamModel;

/** \brief Moves an input model to a different position of an observer tree. */
class MoveInputCmd : public QUndoCommand
{
public:
    MoveInputCmd(ObserverTreeModel* observerTree, int srcObserverPos, int srcInputPos,
                 int destObserverPos, int destInputPos, InputModel* input, QUndoCommand* parent = 0);
    
    virtual void undo();
    virtual void redo();
    
private:
    ObserverTreeModel* m_observerTree;
    int m_srcObserverPos;
    int m_srcInputPos;
    int m_destObserverPos;
    int m_destInputPos;
    InputModel* m_input;
};

#endif // MOVEINPUTCMD_H
