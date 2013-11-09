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
    m_currentSum += elapsed;
        
    // check if enough data has been collected to compute a decision
    if (m_values.count() >= m_numValues)
    {
        // if this is long enough ago update the data and return with success
        if (m_currentSum > m_minSpanMicroseconds)
        {
            const int first = m_values.front();
            m_values.removeFirst();
            m_values.append(elapsed);
            m_currentSum -= first;
            
            return true;
        }
        else
        {
            m_values.last() += elapsed;
            return false;
        }
    }
    else
    {
        // add the data and return
        m_values.append(elapsed);
        return true;
    }
}

