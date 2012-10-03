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

#ifndef ERRORDATA_H
#define ERRORDATA_H

#include <QDateTime>
#include <QString>

namespace stromx
{
    namespace core
    {
        class OperatorError;
    }
}

/**
 * \brief A description of an operator error.
 * 
 * This class holds information about an stromx operator exception. In addition
 * to the data contained in the exception it stores its \em type and the time
 * when the exception was caught.
 */
class ErrorData
{
public:
    /** The type of error, i.e. the state the error happened in. */
    enum Type
    {
        /** Undefined error. */
        UNDEFINED,
        /** Initialization of the operator. */
        INITIALIZATION,
        /** Activation of the operator. */
        ACTIVATION,
        /** Execution of the operator. */
        EXECUTION,
        /** Deactivation of the operator. */
        DEACTIVATION,
        /** Deinitialization of the operator. */
        DEINITIALIZATION,
        /** Accessing a parameter. */
        PARAMETER_ACCESS
    };
    
    /** Constructs an error data of an undefined error. */
    ErrorData();
    
    /** Constructs an error data from a stromx exception and the type of the exception. */
    ErrorData(const stromx::core::OperatorError & exception, Type type);
    
    /** Returns the title of the error. */
    const QString & title() const { return m_title; }
    
    /**
     * Returns a more detailed description of the error such as the message text of the
     * original exception.
     */
    const QString & description() const { return m_description; }
    
    /** Returns the time when the exception was caught. */
    const QDateTime & time() const { return m_time; }
    
    /**  Returns the type of the exception. */
    Type type() const { return m_type; }
                         
private:
    QString m_title;
    QString m_description;
    QDateTime m_time;
    Type m_type;
};

#endif // ERRORDATA_H
