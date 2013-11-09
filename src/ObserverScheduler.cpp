#include "ObserverScheduler.h"
#include <boost/graph/graph_concepts.hpp>

ObserverScheduler::ObserverScheduler(const int numValues, const int minSpanMicroseconds)
  :  m_numValues(numValues),
     m_minSpanMicroseconds(minSpanMicroseconds),
     m_values(new int[numValues])
{

}

ObserverScheduler::~ObserverScheduler()
{
    delete [] m_values;
}

bool ObserverScheduler::schedule()
{
    return true;
}

