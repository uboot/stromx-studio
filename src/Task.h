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

#ifndef TASK_H
#define TASK_H

#include <stromx/core/DataRef.h>
#include <QObject>
#include "ErrorData.h"
#include "OperatorModel.h"

template <class T> class QFutureWatcher;

/** 
 * \brief Task which asynchronously obtain a read access to a data container.
 *
 * This class asynchronously runs a task. After the task has run a finish
 * signal is emitted and the task destroys itself. If the task is deleted
 * the destructor will stop until the task is finished.
 */
class Task : public QObject
{
    Q_OBJECT
    
public:
    enum ErrorCode
    {
        NO_ERROR,
        TIMED_OUT,
        EXCEPTION
    };
    
    /** Constructs a task object. Call run() to actually start the task. */
    explicit Task(QObject* parent = 0);
    
    /** Waits for the task to finish. */
    virtual ~Task();
    
    /** Starts the task. */
    void start();
    
signals:
    /** Emitted after the task finished. */
    void finished();
    
protected:
    virtual void run() = 0;
    
private slots:
    /**
     * Obtains the result of the future and emits the finished signal. After
     * this signal is emitted the object deletes itself.
     */
    void handleFutureFinished();
    
private:
    /** Calls the run() function. */
    static void runTask(Task* task);
   
    QFutureWatcher<void>* m_watcher;
};

#endif // TASK_H
