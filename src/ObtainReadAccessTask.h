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

/** 
 * \brief Task which asynchronously obtain a read access to a data container.
 *
 * Upon construction this class asynchronously tries to obtain read access to the input data container
 * within a certain timeout. After it was either successful or reached the timeout a finish
 * signal is emitted and the task destroys itself. If the object is deleted while waiting for
 * a read access the destructor will stop until the task is finished.
 */
class ObtainReadAccessTask : public QObject
{
    Q_OBJECT
    
public:
    /** 
     * Constructs a task object for the given parameters. The task stars right after
     * its construction.
     */
    explicit ObtainReadAccessTask(OperatorModel::ConnectorType type, unsigned int id,
                                  const stromx::core::DataContainer & dataContainer, QObject* parent = 0);
    
    /** Waits for the task to finish. */
    virtual ~ObtainReadAccessTask();
    
    /** Returns the input ID specified in the constructor. */
    unsigned int id() const { return m_id; }
    
    /** Returns the input type specified in the constructor. */
    OperatorModel::ConnectorType type() const { return m_type; }
    
    /** 
     * Returns read access. The read access is empty if no access could be obtained within
     * the timeout.*/
    const stromx::core::ReadAccess<stromx::core::Data> & readAccess() const { return m_access; }
  
signals:
    /** 
     * Emitted when the function either a read access has been obtained or the 
     * task timed out. 
     */
    void finished();
    
private slots:
    /**
     * Obtains the result of getReadAccess() and emits the finished signal. After
     * this signal is emitted the this object is deleted.
     */
    void handleFutureFinished();
    
private:
    /** 
     * Tries to get a read access to \c container. If successful a non-empty read access
     * is returned. If a timeout occurs while obtaining the read access an empty
     * read access is returned.
     */
    static stromx::core::ReadAccess<stromx::core::Data> getReadAccess(
        const stromx::core::DataContainer & container);
    
    /** Maximal time to wait for a read access in milliseconds. */
    static const unsigned int TIMEOUT;
    
    OperatorModel::ConnectorType m_type;
    unsigned int m_id;
    stromx::core::ReadAccess<stromx::core::Data> m_access;
    QFutureWatcher< stromx::core::ReadAccess<stromx::core::Data> >* m_watcher;
};

#endif // OBTAINREADACCESSTASK_H
