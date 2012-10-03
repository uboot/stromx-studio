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

#ifndef EXCEPTIONOBSERVER_H
#define EXCEPTIONOBSERVER_H

#include <QMutex>
#include <stromx/core/ExceptionObserver.h>

class QObject;
class ErrorData;

class ExceptionObserver : public stromx::core::ExceptionObserver
{
public:
    ExceptionObserver(QObject* receiver);
    
    virtual void observe(const stromx::core::ExceptionObserver::Phase phase,
                         const stromx::core::OperatorError & ex,
                         const stromx::core::Thread* const thread) const;
                         
    void sendErrorData(const ErrorData &data) const;
                         
private:
    mutable QMutex m_mutex;
    QObject* m_receiver;
};

#endif // EXCEPTIONOBSERVER_H
