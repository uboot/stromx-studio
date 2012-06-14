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

class ErrorData
{
public:
    ErrorData(const stromx::core::OperatorError & exception);
    
    const QString & title() const { return m_title; }
    const QString & description() const { return m_description; }
    const QDateTime & time() const { return m_time; }
                         
private:
    QString m_title;
    QString m_description;
    QDateTime m_time;
};

#endif // ERRORDATA_H
