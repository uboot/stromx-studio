/* 
*  Copyright 2011 Matthias Fuchs
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

#ifndef INPUTDATA_H
#define INPUTDATA_H

#include <QMimeData>

class InputModel;
class ObserverModel;
class OperatorModel;

class InputData : public QMimeData
{
    Q_OBJECT
    
public:
    /** The possible sources of a drag operation. */
    enum DragSource
    {
        /** The drag started at a tree model. */
        TREE_MODEL,
        /** The drag started at a table model. */
        TABLE_MODEL
    };
    
    InputData(OperatorModel* op, unsigned int id);
    InputData(InputModel* input, ObserverModel* sourceObserver, int sourcePosition, DragSource dragSource);
    
    unsigned int id() const { return m_id; }
    OperatorModel* op() const { return m_op; }
    InputModel* input() const { return m_input; }
    ObserverModel* sourceObserver() const { return m_sourceObserver; }
    int sourcePosition() const { return m_sourcePosition; }
    DragSource dragSource() const { return m_dragSource; }
    void setDragSource(DragSource dragSource);
    
    virtual QStringList formats() const;
    
private:
    unsigned int m_id;
    OperatorModel* m_op;
    InputModel* m_input;
    ObserverModel* m_sourceObserver;
    int m_sourcePosition;
    DragSource m_dragSource;
};

#endif // INPUTDATA_H
