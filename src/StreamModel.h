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
#include <QList>

class ObserverTreeModel;
namespace stromx
{
    namespace core
    { 
        class FileInput;
        class FileOutput;
        class Input;
        class Operator;
        class Stream;
        class Thread;
    }
}

class QAbstractItemModel;
class QUndoStack;
class ConnectionModel;
class JoinStreamTask;
class OperatorData;
class OperatorModel;
class OperatorLibraryModel;
class ThreadListModel;
class ThreadModel;

class StreamModel : public QObject
{
    Q_OBJECT
    
    friend class AddOperatorCmd;
    friend class RemoveOperatorCmd;
    friend class InitializeOperatorCmd;
    friend class DeinitializeOperatorCmd;
    friend class AddConnectionCmd;
    friend class RemoveConnectionCmd;
    friend class AddThreadCmd;
    friend class RemoveThreadCmd;
    
public:
    explicit StreamModel(QUndoStack* undoStack, OperatorLibraryModel* operatorLibrary, QObject *parent = 0);
    virtual ~StreamModel();
    
    const QList<OperatorModel*> operators() const { return m_operators; }
    void addOperator(const OperatorData* opData, const QPointF & pos);
    void removeOperator(OperatorModel* op);
    
    const QList<ConnectionModel*> connections() const { return m_connections; }
    void addConnection(OperatorModel* sourceOp, unsigned int outputId,
                       OperatorModel* targetOp, unsigned int inputId);
    void removeConnection(ConnectionModel* connection);
    
    const QList<ThreadModel*> threads() const;
    void addThread();
    void removeThread(ThreadModel* thread);
    
    void initializeOperator(OperatorModel* op);
    void deinitializeOperator(OperatorModel* op);
    
    QUndoStack* undoStack() const { return m_undoStack; }
    QAbstractItemModel* threadListModel() const;
    QAbstractItemModel* observerModel() const;
    
    void write(stromx::core::FileOutput & output, const QString & basename) const;
    void read(stromx::core::FileInput & input, const QString & basename);
    
    bool isActive() const;

public slots:
    void start();
    void pause();
    void stop();
    
private slots:
    void join();
    
signals:
    void modelWasReset();
    void operatorAdded(OperatorModel* op);
    void operatorRemoved(OperatorModel* op);
    void connectionAdded(ConnectionModel* connection);
    void connectionRemoved(ConnectionModel* connection);
    void threadAdded(ThreadModel* thread);
    void threadRemoved(ThreadModel* thread);
    void streamStarted();
    void streamPaused();
    void streamStopped();
    void streamJoined();
    
private:
    static const quint32 MAGIC_NUMBER;
    
    void doAddOperator(OperatorModel* op);
    void doRemoveOperator(OperatorModel* op);
    void doInitializeOperator(OperatorModel* op);
    void doDeinitializeOperator(OperatorModel* op);
    void doAddConnection(ConnectionModel* connection);
    void doRemoveConnection(ConnectionModel* connection);
    void doAddThread(ThreadModel* threadModel);
    void doRemoveThread(ThreadModel* threadModel);
    
    void serializeModel(QByteArray& data) const;
    void deserializeModel(const QByteArray& data);
    
    void updateStream(stromx::core::Stream* stream);
    void deleteAllData();
    OperatorModel* findOperatorModel(const stromx::core::Operator* op) const;
    ConnectionModel* findConnectionModel(const stromx::core::Input & input) const;
    ThreadModel* findThreadModel(const stromx::core::Thread* thread) const;
    
    stromx::core::Stream* m_stream;
    ThreadListModel* m_threadListModel;
    ObserverTreeModel* m_observerModel;
    OperatorLibraryModel* m_operatorLibrary;
    QUndoStack* m_undoStack;
    QList<OperatorModel*> m_operators;
    QList<OperatorModel*> m_onlineOperators;
    QList<OperatorModel*> m_uninitializedOperators;
    QList<ConnectionModel*> m_connections;
    JoinStreamTask* m_joinStreamTask;
};

#endif // STREAMMODEL_H
