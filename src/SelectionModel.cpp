#include "SelectionModel.h"

#include "ConnectionModel.h"
#include "ThreadModel.h"
#include "StreamModel.h"
#include <QAction>
#include <QUndoStack>

SelectionModel::SelectionModel(QObject* parent)
  : PropertyModel(parent)
{
}

bool SelectionModel::isValid() const
{
    return 0 != m_connections.count();
}

StreamModel* SelectionModel::streamModel() const
{
    if(isValid())
        return m_connections[0]->streamModel();
    else
        return 0;
}

void SelectionModel::setSelection(QList< QAbstractItemModel* > selection)
{
    beginResetModel();
    
    m_connections.clear();
    
    foreach(QAbstractItemModel* model, selection)
    {
        if(ConnectionModel* connectionModel = qobject_cast<ConnectionModel*>(model))
        {
            m_connections.append(connectionModel);
        }
        else
        {
            m_connections.clear();
            break;
        }
    }
    
    endResetModel();
}

int SelectionModel::rowCount(const QModelIndex& index) const
{
    if(isValid())
        return model()->rowCount(index);
        
    return 0;
}

QAbstractItemModel* SelectionModel::model() const
{
    return m_connections.count() ? m_connections[0] : 0;
}

QModelIndex SelectionModel::index(int row, int column, const QModelIndex& parent) const
{
    if(isValid())
    {
        QModelIndex index = model()->index(row, column, parent);
        return createIndex(index.row(), index.column(), index.internalPointer());
    }
        
    return QModelIndex();
}

QModelIndex SelectionModel::parent(const QModelIndex& child) const
{
    if(isValid())
        return model()->parent(child);
        
    return QModelIndex();
}

QVariant SelectionModel::data(const QModelIndex& index, int role) const
{
    if(isValid())
    {
        if(index.column() == 1 && role == Qt::DisplayRole)
        {
            // if index points to the data column check if all values have
            // the same data there
            QString data = model()->data(index, role).toString();
            
            foreach(ConnectionModel* model, m_connections)
            {
                // if one model has different data return
                if(data != model->data(index, role).toString())
                    return "-";
            }
        }
        
        // return the data of the first model which should be the same
        // as the data of all other models
        return model()->data(index, role);
    }
        
    return QVariant();
}

Qt::ItemFlags SelectionModel::flags(const QModelIndex& index) const
{
    Qt::ItemFlags flags = QAbstractItemModel::flags(index);
    
    if(isValid())
        return flags | model()->flags(index);
        
    return flags;
}

bool SelectionModel::setData(const QModelIndex& index, const QVariant& value, int role)
{
    if(StreamModel* stream = streamModel())
    {
        stream->undoStack()->beginMacro(tr("set threads"));
        bool success = true;
        foreach(ConnectionModel* model, m_connections)
        {
            if(! model->setData(index, value, role))
                success = false;
        }
        stream->undoStack()->endMacro();
        
        return success;
    }
    
    return false;
}

QList<QAction*> SelectionModel::createThreadActions(QObject* parent) const
{
    QList<QAction*> actions;
    
    if(StreamModel* stream = streamModel())
    {
        if(! stream->isActive())
        {             
            Q_ASSERT(m_connections.count());
            
            ThreadModel* testThread = m_connections[0]->thread();
            bool selectedConnectionsHaveSameThread = true;
            foreach(ConnectionModel* model, m_connections)
            {
                // if one model has a different thread break
                if(model->thread() != testThread)
                {
                    selectedConnectionsHaveSameThread = false;
                    break;
                }
            }
            
            QAction* action = new QAction(tr("None"), parent);
            action->setData(0);
            action->setCheckable(true);
            if(selectedConnectionsHaveSameThread && testThread == 0)
                action->setChecked(true);
            connect(action, SIGNAL(triggered(bool)), this, SLOT(setThread()));
            actions.append(action);
                
            // 0 means no thread; values > 0 must decremented to get
            // the thread index
            int threadId = 0;
            foreach(ThreadModel* thread, stream->threads())
            {
                QAction* action = new QAction(thread->name(), parent);
                action->setData(threadId + 1);
                action->setCheckable(true);
                if(selectedConnectionsHaveSameThread && testThread == thread)
                    action->setChecked(true);
                connect(action, SIGNAL(triggered(bool)), this, SLOT(setThread()));
                actions.append(action);
                
                ++threadId;
            }
        }
    }
    
    return actions;
}

void SelectionModel::setThread()
{
    if(StreamModel* stream = streamModel())
    {
        stream->undoStack()->beginMacro(tr("set threads"));
        if(QAction* action = qobject_cast<QAction*>(sender()))
        {
            int data = action->data().toInt();
            if(data == 0)
            {
                foreach(ConnectionModel* connection, m_connections)
                    connection->setThread(0);
            }
            else if(data > 0)
            {
                int threadId = data - 1;
                foreach(ConnectionModel* connection, m_connections)
                    connection->setThread(stream->threads()[threadId]);
            }
        }
        stream->undoStack()->endMacro();
    }
}


