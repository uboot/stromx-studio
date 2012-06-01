#include "ConnectionModel.h"

#include <algorithm>
#include <stromx/core/Thread.h>
#include "Common.h"
#include "OperatorModel.h"
#include "SetThreadCmd.h"
#include "StreamModel.h"
#include "ThreadModel.h"

ConnectionModel::ConnectionModel(OperatorModel* sourceOp, unsigned int outputId,
                                 OperatorModel* targetOp, unsigned int inputId, StreamModel * stream)
  : QAbstractTableModel(stream),
    m_sourceOp(sourceOp),
    m_outputId(outputId),
    m_targetOp(targetOp),
    m_inputId(inputId),
    m_stream(stream),
    m_thread(0)
{
    Q_ASSERT(stream);
}

int ConnectionModel::columnCount(const QModelIndex& index) const
{
    return 2;
}

int ConnectionModel::rowCount(const QModelIndex& index) const
{
    if(index.isValid())
        return 0;
    else
        return 1;
}

QVariant ConnectionModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if(orientation == Qt::Horizontal && role == Qt::DisplayRole)
    {
        if(section == 0)
            return tr("Property");
        else
            return tr("Value");
    }
    
    return QVariant();
}

QVariant ConnectionModel::data(const QModelIndex& index, int role) const
{
    if(index.column() == 0)
    {
        if(role != Qt::DisplayRole)
            return QVariant();
        
        switch(index.row())
        {
        case THREAD:
            return "Thread"; 
        default:
            ;
        }
    }
    else
    {
        switch(index.row())
        {
            case THREAD:
                switch(role)
                {
                case Qt::DisplayRole:
                    if(m_thread)
                        return m_thread->name();
                    else
                        return tr("no thread");
                case Qt::EditRole:
                {
                    if(! m_thread)
                        return 0;
                    
                    int index = 1;
                    foreach(ThreadModel* thread, m_stream->threads())
                    {
                        if(thread == m_thread)
                            return index;
                        ++index;
                    }
                    
                    return -1;
                }
                case ChoicesRole:
                {
                    QList<QVariant> choices;
                    choices << tr("No thread");
                    foreach(ThreadModel* thread, m_stream->threads())
                    {
                        choices << thread->name();
                    }
                    return choices;
                }
                default:
                    ;
                }
                break;
            default:
                ;
        } 
    }
    
    return QVariant();
}

bool ConnectionModel::setData(const QModelIndex& index, const QVariant& value, int role)
{
    if(role != Qt::EditRole)
        return false;
    
    if(index.column() == 0)
        return false;
    
    switch(index.row())
    {
        case THREAD:
        {
            // can not change threads while the stream is active
            if(m_stream->isActive())
                return false;
            
            int index = value.toInt();
            ThreadModel* thread = 0;
            if(index > 0)
            {
                // 0 means no thread; values > 0 must decremented to get
                // the thread index
                Q_ASSERT(index <= m_stream->threads().count());
                thread = m_stream->threads()[index - 1];
            }
            setThread(thread);
            return true;
        }
        default:
            return false;
    } 
}

Qt::ItemFlags ConnectionModel::flags(const QModelIndex& index) const
{
    Qt::ItemFlags flags = QAbstractItemModel::flags(index);
    
    if(m_stream->isActive())
        return flags;
    
    if(index.column() == 1 && index.row() == 0)
        return flags |= Qt::ItemIsEditable;
        
    return flags;
}

void ConnectionModel::setThread(ThreadModel* thread)
{
    if(thread != m_thread)
    {
        QUndoCommand* cmd = new SetThreadCmd(this, thread);
        m_stream->undoStack()->push(cmd);
    }
}

void ConnectionModel::doSetThread(ThreadModel* thread)
{
    if(m_thread != thread)
    {
        // remove from the previous thread
        if(m_thread)
        {
            m_thread->thread()->removeInput(m_targetOp->op(), m_inputId);
            m_thread->disconnect(this);
        }
        
        // add to the new thread
        if(thread)
        {
            // look if the input is part the input sequence of the thread
            const std::vector<stromx::core::Input> & inputs = thread->thread()->inputSequence();
            stromx::core::Input input(m_targetOp->op(), m_inputId);
            std::vector<stromx::core::Input>::const_iterator result = std::find(inputs.begin(), inputs.end(), input); 
                
            // add input only if has not already been added to the thread
            if(result == inputs.end())
                thread->thread()->addInput(m_targetOp->op(), m_inputId);
            
            connect(thread, SIGNAL(colorChanged(QColor)), this, SIGNAL(colorChanged(QColor)));
            connect(thread, SIGNAL(nameChanged(QString)), this, SLOT(updateThread()));
        }
        
        m_thread = thread;
        emit threadChanged(m_thread);
        emit colorChanged(color());
        emit dataChanged(createIndex(THREAD, 1), createIndex(THREAD, 1));
    }
}

void ConnectionModel::updateThread()
{
    emit dataChanged(createIndex(THREAD, 1), createIndex(THREAD, 1));
}

const QColor ConnectionModel::color() const
{
    if(m_thread)
        return m_thread->color();
    else
        return QColor(Qt::gray);
}

void ConnectionModel::connectToOperators()
{
    if(m_sourceOp)
        m_sourceOp->addConnection(this);
    if(m_targetOp)
        m_targetOp->addConnection(this);
}

void ConnectionModel::disconnectFromOperators()
{
    if(m_sourceOp)
        m_sourceOp->removeConnection(this);
    if(m_targetOp)
        m_targetOp->removeConnection(this);
}


