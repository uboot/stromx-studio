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

#ifndef EXCEPTION_H
#define EXCEPTION_H

#include <string>
#include <exception>
#include <QString>
#include <QStringList>

/** \brief Abstract exception. */
class Exception : public std::exception
{
public:
    virtual ~Exception() throw() {}
    
    /** Returns a description of the exception. */
    virtual const char* what() const throw() { return m_message.toStdString().c_str(); }
    
protected:
    /** Constructs an exception. */
    Exception(const QString & message) : m_message(message) {}
                
private:
    QString m_message; 
};  

/** \brief Failed to load an operator library. */
class LoadLibraryFailed : public Exception
{
public:
    LoadLibraryFailed(const QString & message = "LoadLibraryFailed")
      : Exception(message)
    {}
}; 


#endif // EXCEPTION_H