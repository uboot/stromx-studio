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

#ifndef OBTAINREADACCESSTASK_H
#define OBTAINREADACCESSTASK_H

#include <stromx/core/ReadAccess.h>
#include <QObject>
#include "OperatorModel.h"
#include <qfuture.h>

template <class T> class QFuture;
template <class T> class QFutureWatcher;

class ObtainReadAccessTask : public QObject
{
    Q_OBJECT
    
public:
    explicit ObtainReadAccessTask(OperatorModel::ConnectorType type, unsigned int id,
                                  const stromx::core::DataContainer & dataContainer, QObject* parent = 0);
    virtual ~ObtainReadAccessTask();
    
    unsigned int id() const { return m_id; }
    OperatorModel::ConnectorType type() const { return m_type; }
    const stromx::core::ReadAccess<stromx::core::Data> & readAccess() const { return m_access; }
  
signals:
    void finished();
    
private slots:
    void handleFutureFinished();
    
private:
    static stromx::core::ReadAccess<stromx::core::Data> getReadAccess(const stromx::core::DataContainer & container);
    
    static const unsigned int TIMEOUT;
    
    OperatorModel::ConnectorType m_type;
    unsigned int m_id;
    stromx::core::ReadAccess<stromx::core::Data> m_access;
    QFutureWatcher< stromx::core::ReadAccess<stromx::core::Data> >* m_watcher;
};

#endif // OBTAINREADACCESSTASK_H
