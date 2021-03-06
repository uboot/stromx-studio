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

#ifndef RENAMEOBSERVERCMD_H
#define RENAMEOBSERVERCMD_H

#include <QUndoCommand>

class ObserverModel;

/** \brief Sets the name of the observer model. */
class RenameObserverCmd : public QUndoCommand
{
public:
    RenameObserverCmd(ObserverModel* model, const QString & newName, QUndoCommand* parent = 0);
    
    virtual void undo();
    virtual void redo();
    
private:
    ObserverModel* m_model;
    QString m_oldName;
    QString m_newName;
};

#endif // RENAMEOBSERVERCMD_H
