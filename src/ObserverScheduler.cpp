#include "ObserverScheduler.h"

ObserverScheduler::ObserverScheduler(const int numValues, const int minSpanMicroseconds)
  :  m_numValues(numValues),
     m_minSpanMicroseconds(minSpanMicroseconds),
     m_currentSum(0)
{
    m_time.start();
}


bool ObserverScheduler::schedule()
{
    QMutexLocker lock(&m_mutex);
    const int elapsed = m_time.restart();
        
    if (m_values.count() < m_numValues)
    {
        m_values.append(elapsed);
        m_currentSum += elapsed;
        return true;
    }
    else
    {
        const int first = m_values.front();
        m_values.removeFirst();
        m_values.append(elapsed);
        
        m_currentSum -= first;
        m_currentSum += elapsed;
        
        return m_currentSum > m_minSpanMicroseconds;
    }
}

