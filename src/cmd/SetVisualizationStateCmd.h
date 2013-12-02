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

#ifndef SETINPUTVISUALIZATIONSTATECMD_H
#define SETINPUTVISUALIZATIONSTATECMD_H

#include <QUndoCommand>

#include "VisualizationState.h"

class InputModel;

/** \brief Sets the visualization state of an input model. */
class SetVisualizationStateCmd : public QUndoCommand
{
public:
    SetVisualizationStateCmd(InputModel* model, 
        const VisualizationState & newState, QUndoCommand* parent = 0);
    
    virtual void undo();
    virtual void redo();
    
private:
    InputModel* m_model;
    VisualizationState m_oldState;
    VisualizationState m_newState;
};

#endif // SETINPUTVISUALIZATIONSTATECMD_H