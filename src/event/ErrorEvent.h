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

#ifndef ERROREVENT_H
#define ERROREVENT_H

#include <QEvent>

#include "Common.h"
#include "data/ErrorData.h"

/** \brief Event which contains error data. */
class ErrorEvent : public QEvent
{
public:
    static const unsigned int TYPE = QEvent::User + Error;
    
    /** Construct an error event from error data. */
    ErrorEvent(const ErrorData & data) : QEvent(Type(TYPE)), m_data(data) {}
    
    /** Returns the error data. */
    const ErrorData & errorData() const { return m_data; }
    
private:
    ErrorData m_data;
};

#endif // ERROREVENT_H
