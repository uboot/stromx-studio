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

#ifndef PARAMETERSERVER_H
#define PARAMETERSERVER_H

#include <QObject>
#include <QVariant>

class QUndoStack;
class QVariant;

namespace stromx
{
    namespace core
    {
        class Operator;
    }
}

class ParameterServer : public QObject
{
    Q_OBJECT
    
public:
    ParameterServer(stromx::core::Operator* op, QUndoStack* undoStack, QObject* parent = 0);
    
    const QVariant getParameter(unsigned int id, int role);
    void setParameter (unsigned int id, const QVariant& value);
    void refresh();
    
signals:
    void parameterChanged(unsigned int id);
    
private:
    stromx::core::Operator* m_op;
    QUndoStack* m_undoStack;
};

#endif // PARAMETERSERVER_H
