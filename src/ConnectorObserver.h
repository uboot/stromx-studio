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

#ifndef CONNECTOROBSERVER_H
#define CONNECTOROBSERVER_H

#include <QMutex>
#include <stromx/runtime/ConnectorObserver.h>

#include "ObserverScheduler.h"

namespace stromx
{
    namespace runtime
    { 
        class DataContainer;
        class Input;
    }
}

class QCoreApplication;
class QObject;

class ConnectorObserver : public stromx::runtime::ConnectorObserver
{
public:
    ConnectorObserver(QObject* receiver);
        
    virtual void observe(const stromx::runtime::Connector &connector,
                         const stromx::runtime::DataContainer &data,
                         const stromx::runtime::Thread* const thread) const;
                         
    void setObserveData(bool observe);
                         
private:
    const static int NUM_VALUES;
    const static int MIN_SPAN_MILLISECONDS;
    
    static ObserverScheduler gScheduler;
    
    QObject* m_receiver;
    bool m_observeData;
    mutable QMutex m_mutex;
};

#endif // CONNECTOROBSERVER_H
