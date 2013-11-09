/* 
*  Copyright 2013 Matthias Fuchs
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

#ifndef OBSERVERSCHEDULER_H
#define OBSERVERSCHEDULER_H

#include <QList>
#include <QMutex>
#include <QTime>

class ObserverScheduler
{
public:
    ObserverScheduler(const int numValues, const int minSpanMicroseconds);
    bool schedule();
    
private:
    const int m_numValues;
    const int m_minSpanMicroseconds;
    QMutex m_mutex;
    QTime m_time;
    QList<int> m_values;
    int m_currentSum;
};

#endif // OBSERVERSCHEDULER_H
