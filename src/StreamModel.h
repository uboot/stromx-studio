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

#ifndef STREAMMODEL_H
#define STREAMMODEL_H

#include <QObject>
#include <QPointF>

namespace stromx
{
    namespace core
    { 
        class Stream;
        class Operator;
    }
}

class QAbstractTableModel;
class QUndoStack;
class ConnectionModel;
class OperatorModel;
class ThreadListModel;
class ThreadModel;

class StreamModel : public QObject
{
    Q_OBJECT
    
    friend class AddOperatorCmd;
    
public:
    explicit StreamModel(QUndoStack* undoStack, QObject *parent = 0);
    virtual ~StreamModel();
    
    void addOperator(stromx::core::Operator* const op, const QPointF & pos);
    void removeOperator(OperatorModel* op);
    
    void addConnection(OperatorModel* sourceOp, unsigned int outputId,
                       OperatorModel* targetOp, unsigned int inputId);
    void removeConnection(ConnectionModel* connection);
    
    void addThread();
    void removeThread(ThreadModel* thread);
    
    void initializeOperator(OperatorModel* op);
    void deinitializeOperator(OperatorModel* op);
    
    ThreadListModel* threadListModel() const { return m_threadListModel; }
    
signals:
    void operatorAdded(OperatorModel* op);
    void operatorRemoved(OperatorModel* op);
    void connectionAdded(ConnectionModel* connection);
    void connectionRemoved(ConnectionModel* connection);
    void threadAdded(ThreadModel* thread);
    void threadRemoved(ThreadModel* thread);
    
private:
    void doAddOperator(OperatorModel* op);
    void doRemoveOperator(OperatorModel* op);
    
    stromx::core::Stream* m_stream;
    ThreadListModel* m_threadListModel;
    QUndoStack* m_undoStack;
};

#endif // STREAMMODEL_H
