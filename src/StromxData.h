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

#ifndef STROMXDATA_H
#define STROMXDATA_H

#include <QMimeData>
#include <QSet>

namespace stromx 
{
    namespace core 
    {
        class Operator;
    }
}

class StromxData : public QMimeData
{
    Q_OBJECT
    
public:
    StromxData(const QSet<stromx::core::Operator*> & operators);
    StromxData(stromx::core::Operator* const op);
    
    void deleteData();
    const QSet<stromx::core::Operator*> & operators() const { return m_operators; }
    
private:
    QSet<stromx::core::Operator*> m_operators;
};

#endif // STROMXDATA_H
