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
        class Data;
        class Operator;
        class Parameter;
        class ParameterError;
    }
}

class ParameterServer : public QObject
{
    Q_OBJECT
    
    friend class SetParameterCmd;
    
public:
    ParameterServer(stromx::core::Operator* op, QUndoStack* undoStack, QObject* parent = 0);
    
    const QVariant getParameter(unsigned int id, int role);
    bool setParameter (unsigned int id, const QVariant& value);
    void refresh();
    stromx::core::Operator* op() const { return m_op; }
    
    Qt::ItemFlags parameterFlags(unsigned int id) const;
    
    /** 
     * Returns true if the parameter \c par is read-accessible or it is a non-empty parameter
     * group, i.e. it has at least one member. 
     */
    bool parameterIsDisplayed(unsigned int id) const;
    
signals:
    void parameterChanged(unsigned int id);
    
    /** An operation accessing a parameter of the operator timed out. */
    void parameterAccessTimedOut();
    
    /** A parameter occurred while setting a parameter. */
    void parameterErrorOccurred(const stromx::core::ParameterError &) const;
    
private:
    /** Returns whether the parameter \c par can currently be read. */
    bool parameterIsReadAccessible(const stromx::core::Parameter& par) const;
    
    /** Returns whether the parameter \c par can currently be written. */
    bool parameterIsWriteAccessible(const stromx::core::Parameter& par) const;
    
    /** Sets the parameter \c paramId to \c newValue. */
    void doSetParameter(unsigned int paramId, const stromx::core::Data& newValue);
    
    static const unsigned int TIMEOUT;
    
    stromx::core::Operator* m_op;
    QUndoStack* m_undoStack;
};

#endif // PARAMETERSERVER_H
