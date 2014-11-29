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

#include <exception>
#include <QString>

class Exception : public std::exception
{
public:
    Exception(const QString & message) : m_message(message) {}
    
    virtual const char* what() const throw() { return m_message.toUtf8(); }
    virtual ~Exception() throw() {}
    
private:
    QString m_message;
}; 

class LoadPackageFailed : public Exception
{
public:
    LoadPackageFailed(const QString & message = "") : Exception(message) {}
};

class ReadStreamFailed : public Exception
{
public:
    ReadStreamFailed(const QString & message = "") : Exception(message) {}
};

class ReadObserverDataFailed : public Exception
{
public:
    ReadObserverDataFailed(const QString & message = "") : Exception(message) {}
};

class ReadStudioDataFailed : public Exception
{
public:
    ReadStudioDataFailed(const QString & message = "") : Exception(message) {}
};

class WriteStreamFailed : public Exception
{
public:
    WriteStreamFailed(const QString & message = "") : Exception(message) {}
};

#endif // EXCEPTION_H
