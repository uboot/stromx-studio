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

#ifndef INSERTOBSERVERCMD_H
#define INSERTOBSERVERCMD_H

#include <QUndoCommand>

class ObserverModel;
class ObserverTreeModel;
class StreamModel;

/** \brief Inserts an observer model into an observer tree. */
class InsertObserverCmd : public QUndoCommand
{
public:
    InsertObserverCmd(ObserverTreeModel* observerTree, int pos, ObserverModel* observer, QUndoCommand* parent = 0);
    
    virtual void undo();
    virtual void redo();
    
private:
    ObserverTreeModel* m_observerTree;
    ObserverModel* m_observer;
    int m_pos;
};

#endif // INSERTOBSERVERCMD_H
