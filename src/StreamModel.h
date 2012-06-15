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
class ExceptionObserver;
class JoinStreamTask;
class ObserverTreeModel;
class OperatorData;
class OperatorModel;
class OperatorLibraryModel;
class ThreadListModel;
class ThreadModel;

/**
 * \brief Model of a stream
 * 
 * This class represents a stromx::core::Stream object in the stromx-studio application.
 * It records all actions of its public interface to the undo stack. A StreamModel can be 
 * constructed from a file and written to a file.
 * In addition to all properties of a stream as in the stromx library it stores and
 * all information about the visual appearance of the stream in the stromx-studio.
 * Moreover, uninitialized operators and all observers are part of a %StreamModel.
 */
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
    /** Constructs an empty stream model. */
    explicit StreamModel(QUndoStack* undoStack, OperatorLibraryModel* operatorLibrary, QObject *parent = 0);
    
    /**
     * Constructs a stream model from a file input. This functions reads only the stromx stream.
     * To obtain the studio specific data (operator positions, uninitialized operators...)
     * from the file call readStudioData().
     * 
     * \throws ReadStreamFailed
     */
    explicit StreamModel(stromx::core::FileInput & input, const QString & basename, 
                         QUndoStack* undoStack, OperatorLibraryModel* operatorLibrary, QObject *parent = 0);
    
    virtual ~StreamModel();
    /**
     * Reads the studios specific data (operator positions, uninitialized operators...)
     * of the stream from a file input.
     * 
     * \throws ReadStreamFailed
     */
    void readStudioData(stromx::core::FileInput & input, const QString & basename);
    
    /** Returns the operators of the stream model. */
    const QList<OperatorModel*> operators() const { return m_operators; }
     
    /**
     * Returns the current position of the operator in the operator list of the
     * stromx stream. If the operator is not initialized and thus not part of
     * the stromx stream -1 is returned.
     */
    int operatorId(const OperatorModel* op) const;
    
    /** 
     * Constructs an operator model and pushes an add operator command on the 
     * undo stack. 
     */
    void addOperator(const OperatorData* opData, const QPointF & pos);
    
    /** Pushes a remove operator command on the undo stack. */
    void removeOperator(OperatorModel* op);
    
    /** Returns the connections of the stream model. */
    const QList<ConnectionModel*> connections() const { return m_connections; }
    
    /** 
     * Constructs a connection model and pushes a add connection command on
     * the undo stack.
     */
    void addConnection(OperatorModel* sourceOp, unsigned int outputId,
                       OperatorModel* targetOp, unsigned int inputId);
    
    /** Pushes a remove connection on the undo stack. */
    void removeConnection(ConnectionModel* connection);
    
    /** Returns the threads of the stream model. */
    const QList<ThreadModel*> threads() const;
    
    /**
     * Constructs a new thread and pushes an add thread command on the
     * undo stack.
     */
    void addThread();
    
    /** Pushes a remove thread command on the undo stack. */
    void removeThread(ThreadModel* thread);
    
    /** Pushes an initialize operator command on the undo stack. */
    void initializeOperator(OperatorModel* op);
    
    /** Pushes a deinitialize operator command on the undo stack. */
    void deinitializeOperator(OperatorModel* op);
    
    /** Returns the undo stack. */
    QUndoStack* undoStack() const { return m_undoStack; }
    
    /** Returns the thread list of the stream model. */
    QAbstractItemModel* threadListModel() const;
    
    /** Returns the observer list of the current model. */
    ObserverTreeModel* observerModel() const;
    
    /** Writes the content of the stream model. */
    void write(stromx::core::FileOutput & output, const QString & basename) const;
    
    /** Returns true if the underlying stromx stream is active. */
    bool isActive() const;
    
    /** Sets the current exception observer. */
    void setExceptionObserver(ExceptionObserver* observer);

public slots:
    /** Starts the stromx stream. Returns true if successful. */
    bool start();
    
    /** Pauses the stromx stream. Returns true if successful. */
    bool pause();
    
    /** Stops the stromx stream. Returns true if successful. */
    bool stop();
    
    /** 
     * Activates or deactivates the operator delay. If the delay is activated
     * the data processing seems to be in slow-motion.
     */
    void setDelay(bool active);
    
private slots:
    /** Joins the stromx stream. */
    void join();
    
signals:
    /** An operator was added. */
    void operatorAdded(OperatorModel* op);
    
    /** An operator was removed. */
    void operatorRemoved(OperatorModel* op);
    
    /** A connection was added. */
    void connectionAdded(ConnectionModel* connection);
    
    /** A connection was removed. */
    void connectionRemoved(ConnectionModel* connection);
    
    /** A thread was added. */
    void threadAdded(ThreadModel* thread);
    
    /** A thread was removed. */
    void threadRemoved(ThreadModel* thread);
    
    /** The stromx stream was started. */
    void streamStarted();
    
    /** The stromx stream was paused. */
    void streamPaused();
    
    /** The stromx stream was stopped. */
    void streamStopped();
    
    /** The stromx stream was joined. */
    void streamJoined();
    
private:
    /** The operator delay in milliseconds. */
    static const unsigned int DELAY;
    
    /** Magic number which identifies the first 4 bytes of stromx-studio data files. */
    static const quint32 MAGIC_NUMBER;
    
    /** Allocates all submodel and sets up the necessary connections. */
    void initializeSubModels();
    
    /** Adds an operator. */
    void doAddOperator(OperatorModel* op);
    
    /** Removes an operator. */
    void doRemoveOperator(OperatorModel* op);
    
    /** Initializes an operator. */
    void doInitializeOperator(OperatorModel* op);
    
    /** Deinitializes an operator. */
    void doDeinitializeOperator(OperatorModel* op);
    
    /** Adds a connection. */
    void doAddConnection(ConnectionModel* connection);
    
    /** Removes a connection. */
    void doRemoveConnection(ConnectionModel* connection);
    
    /** Adds a thread. */
    void doAddThread(ThreadModel* threadModel);
    
    /**
     * Removes a thread. Sets the thread model of all connections which
     * are part of the thread to 0.
     */
    void doRemoveThread(ThreadModel* threadModel);
    
    /** 
     * Serializes all data of the stream which is not stored in the stromx XML,
     * as e.g. operator positions.
     */
    void serializeModel(QByteArray& data) const;
    
    /** 
     * Deaerializes all data of the stream which is not stored in the stromx XML,
     * as e.g. operator positions.
     */
    void deserializeModel(const QByteArray& data);
    
    /**
     * Iterates over \c stream and allocates models for all operators, threads
     * and connections in the stream.
     */
    void allocateObjects(stromx::core::Stream* stream);
    
    /** Deletes the stromx stream and all models. */ 
    void deleteAllData();
    
    /** Finds the operator model which wraps \c op. Returns 0 if no such model exists. */
    OperatorModel* findOperatorModel(const stromx::core::Operator* op) const;
    
    /** Finds the connection model which connects to \c input. Returns 0 if no such model exists. */
    ConnectionModel* findConnectionModel(const stromx::core::Input & input) const;
    
    /** Finds the thread model which wraps \c thread. Returns 0 if no such model exists. */
    ThreadModel* findThreadModel(const stromx::core::Thread* thread) const;
    
    stromx::core::Stream* m_stream;
    ThreadListModel* m_threadListModel;
    ObserverTreeModel* m_observerModel;
    OperatorLibraryModel* m_operatorLibrary;
    QUndoStack* m_undoStack;
    QList<OperatorModel*> m_operators;
    QList<OperatorModel*> m_initializedOperators;
    QList<OperatorModel*> m_uninitializedOperators;
    QList<ConnectionModel*> m_connections;
    JoinStreamTask* m_joinStreamTask;
    ExceptionObserver* m_exceptionObserver;
};

#endif // STREAMMODEL_H
