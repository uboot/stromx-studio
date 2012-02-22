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
class OperatorModel;

class InputData : public QMimeData
{
    Q_OBJECT
    
public:
    InputData(OperatorModel* op, unsigned int id);
    InputData(InputModel* input);
    
    unsigned int id() const { return m_id; }
    OperatorModel* op() const { return m_op; }
    InputModel* input() const { return m_input; }
    virtual QStringList formats() const;
    
private:
    unsigned int m_id;
    OperatorModel* m_op;
    InputModel* m_input;
};

#endif // INPUTDATA_H
