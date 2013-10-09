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

#ifndef SETINPUTVISUALIZATIONPROPERTIESCMD_H
#define SETINPUTVISUALIZATIONPROPERTIESCMD_H

#include <QColor>
#include <QUndoCommand>

#include "AbstractDataVisualizer.h"
#include "model/InputModel.h"

/** \brief Sets the visualization properties of an input model. */
class SetVisualizationPropertiesCmd : public QUndoCommand
{
public:
    SetVisualizationPropertiesCmd(InputModel* model, 
        const InputModel::VisualizationProperties & newProperties, QUndoCommand* parent = 0);
    
    virtual void undo();
    virtual void redo();
    
private:
    InputModel* m_model;
    InputModel::VisualizationProperties m_oldProperties;
    InputModel::VisualizationProperties m_newProperties;
};

#endif // SETINPUTVISUALIZATIONPROPERTIESCMD_H