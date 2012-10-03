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
#include <stromx/core/DataRef.h>

class QUndoStack;
class QVariant;

namespace stromx
{
    namespace core
    {
        class Operator;
        class Parameter;
        class ParameterError;
    }
}

class ErrorData;

class ParameterServer : public QObject
{
    Q_OBJECT
    
    friend class SetParameterCmd;
    
public:
    /** 
     * Constructs a parameter server for \c op. Note that the cache of the server has to be updated
     * by calling refresh() before the server can be used.
     */
    ParameterServer(stromx::core::Operator* op, QUndoStack* undoStack, QObject* parent = 0);
    
    const QVariant getParameter(unsigned int id, int role);
    bool setParameter (unsigned int id, const QVariant& value);
    void refresh();
    
    Qt::ItemFlags parameterFlags(unsigned int id) const;
    
    /** 
     * Returns true if the parameter \c par is read-accessible or it is a non-empty parameter
     * group, i.e. it has at least one member. 
     */
    bool parameterIsDisplayed(unsigned int id) const;
    
signals:
    /** The value of the parameter \c id has changed. */
    void parameterChanged(unsigned int id);
    
    /** An operation accessing a parameter of the operator timed out. */
    void parameterAccessTimedOut();
    
    /** A parameter occurred while setting a parameter. */
    void parameterErrorOccurred(const ErrorData & errorData) const;
    
private slots:
    /**
     * If the task was successful this functions updates the value cache for the parameter.
     * Otherwise it emits time out or error signals. 
     */
    void handleGetParameterTaskFinished();
    
    /** 
     * If the task was successful this functions starts another task which gets the 
     * value of the updated parameter.
     */
    void handleSetParameterTaskFinished();
    
private:
    enum ParameterState
    {
        CURRENT,
        SETTING,
        GETTING,
        TIMED_OUT,
        ERROR
    };
    
    struct ParameterValue
    {
        ParameterState state;
        stromx::core::DataRef value;
    };
    
    /** Returns the stromx operator whose parameters are served. */
    stromx::core::Operator* op() const { return m_op; }
    
    /** Returns whether the parameter \c par can currently be read. */
    bool parameterIsReadAccessible(const stromx::core::Parameter& par) const;
    
    /** Returns whether the parameter \c par can currently be written. */
    bool parameterIsWriteAccessible(const stromx::core::Parameter& par) const;
    
    /** Sets the parameter \c paramId to \c newValue. */
    void doSetParameter(unsigned int paramId, const stromx::core::DataRef& newValue);
    
    /** Refreshes the cached value for the parameter \c param. */
    void refreshParameter(const stromx::core::Parameter & param);
    
    stromx::core::Operator* m_op;
    QUndoStack* m_undoStack;
    QMap<unsigned int, ParameterValue> m_cache;
};

#endif // PARAMETERSERVER_H
