#include "ObserverTreeModel.h"

#include <QStringList>
#include "Common.h"
#include "InsertInputCmd.h"
#include "InsertObserverCmd.h"
#include "InputData.h"
#include "InputModel.h"
#include "ConnectionModel.h"
#include "MoveInputCmd.h"
#include "ObserverModel.h"
#include "OperatorModel.h"
#include "RemoveInputCmd.h"
#include "RemoveObserverCmd.h"

ObserverTreeModel::ObserverTreeModel(QUndoStack* undoStack, StreamModel * parent)
  : QAbstractItemModel(parent),
    m_undoStack(undoStack),
    m_stream(parent),
    m_suppressRemoveRows(false)
{
    setSupportedDragActions(Qt::MoveAction);
    
    connect(m_stream, SIGNAL(connectionRemoved(ConnectionModel*)), this, SLOT(removeConnectedInputs(ConnectionModel*)));
}

int ObserverTreeModel::rowCount(const QModelIndex& parent) const
{
    // parent is root
    if(! parent.isValid())
        return m_observers.count();
    
    // parent is an observer
    if(! parent.internalPointer())
        return m_observers[parent.row()]->numInputs();
    
    // parent is an input
    return 0;
}

int ObserverTreeModel::columnCount(const QModelIndex& parent) const
{
    return NUM_VISIBLE_COLUMNS;
}

QVariant ObserverTreeModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if(role != Qt::DisplayRole)
        return QVariant();
        
    if(orientation != Qt::Horizontal)
        return QVariant();
    
    switch(section)
    {
    case OPERATOR:
        return tr("Operator");
    case ID:
        return tr("ID");
    default:
        return QVariant();
    }
}

bool ObserverTreeModel::insertRows(int row, int count, const QModelIndex & parent)
{
    if(parent.isValid())
        return false;
    
    Q_ASSERT(count == 1);
    
    ObserverModel* observer = new ObserverModel(m_undoStack, this);
    connect(observer, SIGNAL(changed(ObserverModel*)), this, SLOT(updateObserver(ObserverModel*)));
    
    QUndoCommand* cmd = new InsertObserverCmd(this, row, observer);
    m_undoStack->push(cmd);
    
    return true;
}

void ObserverTreeModel::doInsertObserver(int pos, ObserverModel* observer)
{
    beginInsertRows(QModelIndex(), pos, pos);
    m_observers.insert(pos, observer);
    foreach(InputModel* input, observer->inputs())
        connect(input, SIGNAL(changed(InputModel*)), this, SLOT(updateInput(InputModel*)));
    endInsertRows();
    
    emit observerAdded(observer);
}

bool ObserverTreeModel::removeRows(int row, int count, const QModelIndex & parent)
{
    Q_ASSERT(count == 1);
    
    if(m_suppressRemoveRows)
    {
        m_suppressRemoveRows = false;
        return true;
    }
    
    if(parent.isValid())
    {
        InputModel* input = m_observers[parent.row()]->input(row);
        QUndoCommand* cmd = new RemoveInputCmd(this, parent.row(), row, input);
        m_undoStack->push(cmd);
    }
    else
    {
        QUndoCommand* cmd = new RemoveObserverCmd(this, row, m_observers[row]);
        m_undoStack->push(cmd);
    }
    
    return true;
}

void ObserverTreeModel::doRemoveObserver(int pos)
{
    beginRemoveRows(QModelIndex(), pos, pos);
    ObserverModel* observer = m_observers[pos];
    m_observers.removeAt(pos);
    // Note that the inputs of the observer are still connected to this tree
    // view model. The inputs which are not part of any other observer could be
    // disconnected.
    endRemoveRows();
    
    emit observerRemoved(observer);
}

void ObserverTreeModel::doRemoveInput(int observerPos, int inputPos)
{
    beginRemoveRows(createIndex(observerPos, 0), inputPos, inputPos);
    ObserverModel* observer = m_observers[observerPos]; 
    InputModel* input = observer->input(inputPos);
    m_observers[observerPos]->removeInput(inputPos);
    // Note that the input is still connected to this tree
    // view model. If the input is not observed at any other place it could
    // be disconnected.
    endRemoveRows();
    
    emit inputRemoved(input, observer, inputPos);
}

QModelIndex ObserverTreeModel::index(int row, int column, const QModelIndex& parent) const
{
    // this is an observer list
    if(! parent.isValid())
        return createIndex(row, column);
    
    // this is an input
    return createIndex(row, column, m_observers[parent.row()]);
}

QModelIndex ObserverTreeModel::parent(const QModelIndex& child) const
{    
    // child is an observer
    if(! child.internalPointer())
        return QModelIndex();
    
    // child is an input
    ObserverModel* observer = reinterpret_cast<ObserverModel*>(child.internalPointer());
    int observerId = m_observers.indexOf(observer);
    return createIndex(observerId, 0);
}

QVariant ObserverTreeModel::data(const QModelIndex& index, int role) const
{
    // this is an observer
    if(! index.internalPointer())
    {
        if(index.column() != 0)
            return QVariant();
        
        switch(role)
        {
        case Qt::DisplayRole:
        case Qt::EditRole:
            return m_observers[index.row()]->name();
        default: 
            return QVariant();
        }
    }
    
    // this is an input
    ObserverModel* observer = reinterpret_cast<ObserverModel*>(index.internalPointer());
    const InputModel* input = observer->input(index.row());
    switch(role)
    {
    case Qt::DisplayRole:
        switch(index.column())
        {
        case OPERATOR:
            return input->op()->name();
        case ID:
            return input->id();
        case COLOR:
        {
            QColor color = input->color();
            foreach(QString name, QColor::colorNames())
            {
                if(QColor(name) == color)
                    return name;
            }
            return color;
        }
        default:
            return QVariant();
        }
    case Qt::DecorationRole:
        switch(index.column())
        {
        case COLOR:
            return input->color();
        default:
            return QVariant();
        }
    case ColorRole:
        switch(index.column())
        {
        case COLOR:
            return input->color();
        default:
            return QVariant();
        }
    default:
        return QVariant();
    }
}

bool ObserverTreeModel::setData(const QModelIndex& index, const QVariant& value, int role)
{
    if(role != Qt::EditRole)
        return false;
        
    // the index points to an observer name
    if(index.isValid() && ! index.internalPointer() && index.column() == OPERATOR)
    {
        QString newName = value.toString();
        
        if(newName.isEmpty())
            return false;
        
        m_observers[index.row()]->setName(newName);
        emit dataChanged(index, index);
    }
    
    // the index points to an input color
    if(index.isValid() && index.internalPointer() && index.column() == COLOR)
    {
        // get the input
        ObserverModel* observer = reinterpret_cast<ObserverModel*>(index.internalPointer());
        InputModel* input = observer->input(index.row());
        
        // set the color
        QColor color(value.toString());
        input->setColor(color);
        emit dataChanged(index, index);
        return true;
    }
    
    return false;
}

Qt::ItemFlags ObserverTreeModel::flags(const QModelIndex& index) const
{
    Qt::ItemFlags flags = QAbstractItemModel::flags(index);
    
    // top level can not accept drags
    if(! index.isValid())
        return flags;
    
    // observer names are editable
    if(! index.internalPointer() && index.column() == OPERATOR)
        return flags |= Qt::ItemIsEditable | Qt::ItemIsDropEnabled;
    
    return flags |= Qt::ItemIsDragEnabled;
}

bool ObserverTreeModel::dropMimeData(const QMimeData *data,
     Qt::DropAction action, int row, int column, const QModelIndex &parent)
{
    if(action == Qt::IgnoreAction)
        return true;
    
    if(action == Qt::MoveAction || action == Qt::CopyAction)
    {
        if(const InputData* inputData = qobject_cast<const InputData*>(data))
        {
            int inputPos = row;
            if(inputPos < 0)
                inputPos = 0;
            
            InputModel* input = inputData->input();
            if(! input)
            {
                // a new input was inserted from
                Q_ASSERT(action == Qt::CopyAction);
                input = new InputModel(inputData->op(), inputData->id(), m_undoStack, this);
                QUndoCommand* cmd = new InsertInputCmd(this, parent.row(), inputPos, input);
                m_undoStack->push(cmd);
            }
            else
            {
                // a input was moved within the model
                Q_ASSERT(action == Qt::MoveAction);
                
                // if the data originated from the tree view removeRows() is automatically called
                // because the row is removed manually here, this call has to suppressed
                if(inputData->dragSource() == InputData::TREE_MODEL)
                    m_suppressRemoveRows = true;
                
                int srcObserverPos = m_observers.indexOf(inputData->sourceObserver());
                int srcInputPos = inputData->sourcePosition();
                int destObserverPos = parent.row();
                int destInputPos = inputPos;
                
                QUndoCommand* cmd = 0;
                if(srcObserverPos == destObserverPos && srcInputPos == destInputPos)
                    return true;
                
                // the input is moved within the same observer
                if(srcObserverPos == destObserverPos)
                {
                    int numInputs = m_observers[srcInputPos]->numInputs();
                    Q_ASSERT(numInputs);
                    
                    // the input was moved to the end of the list
                    if(destInputPos >= numInputs)
                    {
                        cmd = new MoveInputCmd(this, srcObserverPos, srcInputPos,
                                                     destObserverPos, destInputPos - 1, input);
                    }
                    else
                    {
                        cmd = new MoveInputCmd(this, srcObserverPos, srcInputPos,
                                                     destObserverPos, destInputPos, input);
                    } 
                }
                else
                {
                    cmd = new MoveInputCmd(this, srcObserverPos, srcInputPos,
                                           destObserverPos, destInputPos, input);
                }
                m_undoStack->push(cmd);
            }
            
            return true;
        }
    }
    
    return false;
}

void ObserverTreeModel::doInsertInput(int observerPos, int inputPos, InputModel* input)
{
    beginInsertRows(createIndex(observerPos, 0), inputPos, inputPos);
    ObserverModel* observer = m_observers[observerPos]; 
    m_observers[observerPos]->insertInput(inputPos, input);
    connect(input, SIGNAL(changed(InputModel*)), this, SLOT(updateInput(InputModel*)));
    endInsertRows();
    
    emit inputAdded(input, observer, inputPos);
}

QStringList ObserverTreeModel::mimeTypes() const
{
    return QStringList("stromx/input");
}

QMimeData* ObserverTreeModel::mimeData(const QModelIndexList& indexes) const
{
    if(indexes.empty())
        return 0;
    
    const QModelIndex & index = indexes[0];
    
    // this is an observer
    if(! index.internalPointer())
        return 0;
    
    ObserverModel* observer = reinterpret_cast<ObserverModel*>(index.internalPointer());
    InputModel* input = observer->input(index.row());
    return new InputData(input, observer, index.row(), InputData::TREE_MODEL);
}

Qt::DropActions ObserverTreeModel::supportedDropActions() const
{
    return Qt::MoveAction | Qt::CopyAction;
}

void ObserverTreeModel::removeConnectedInputs(ConnectionModel* connection)
{
    for(int i = 0; i < m_observers.count(); ++i)
    {
        ObserverModel* observer = m_observers[i];
        
        for(int j = 0; j < m_observers[i]->numInputs();)
        {
            if(observer->input(j)->op() == connection->targetOp()
                && observer->input(j)->id() == connection->inputId())
            {
                removeRows(j, 1, createIndex(i, 0));
            }
            else
            {
                ++j;
            }
        }        
    }
}

void ObserverTreeModel::updateObserver(ObserverModel *observer)
{
    int pos = m_observers.indexOf(observer);
    if(pos >= 0)
        emit dataChanged(createIndex(pos, 0), createIndex(pos, NUM_COLUMNS - 1));
}

void ObserverTreeModel::doMoveInput(int srcObserverPos, int srcInputPos, int destObserverPos, int destInputPos, InputModel* input)
{
    ObserverModel* srcObserver = m_observers[srcObserverPos]; 
    ObserverModel* destObserver = m_observers[destObserverPos];
    
    beginRemoveRows(createIndex(srcObserverPos, 0), srcInputPos, srcInputPos);
    m_observers[srcObserverPos]->removeInput(srcInputPos);
    endRemoveRows();
    
    beginInsertRows(createIndex(destObserverPos, 0), destInputPos, destInputPos);
    m_observers[destObserverPos]->insertInput(destInputPos, input);
    endInsertRows();
    
    emit inputMoved(input, srcObserver, srcInputPos, destObserver, destInputPos);
}

void ObserverTreeModel::updateInput(InputModel* input)
{
    foreach(ObserverModel* observer, m_observers)
    {
        int pos = observer->inputs().indexOf(input);
        if(pos >= 0)
            emit dataChanged(createIndex(pos, 0, observer), createIndex(pos, NUM_COLUMNS - 1, observer));
    }
}

QDataStream& operator<<(QDataStream& stream, const ObserverTreeModel* model)
{
    stream << qint32(model->m_observers.count());
    foreach(ObserverModel* observer, model->m_observers)
    {
        stream << observer->name();
        stream << qint32(observer->numInputs());
        foreach(InputModel* input, observer->inputs())
        {
            int opId = model->m_stream->operatorId(input->op());
            Q_ASSERT(opId >=  0);
            
            stream << model->m_stream->operatorId(input->op());
            stream << input->id();
            stream << input->color();
        }
    }
    
    return stream;
}

QDataStream& operator>>(QDataStream& stream, ObserverTreeModel* model)
{
    qint32 observerCount = 0;
    stream >> observerCount;
    
    for(int i = 0; i < observerCount; ++i)
    {
        ObserverModel* observer = new ObserverModel(model->m_undoStack, model);
        QObject::connect(observer, SIGNAL(changed(ObserverModel*)), model, SLOT(updateObserver(ObserverModel*)));
        QString str;
        stream >> str;
        observer->setName(str);
        qint32 inputCount = 0;
        stream >> inputCount;
        
        for(int j = 0; j < inputCount; ++j)
        {
            qint32 opId;
            qint32 inputId;
            QColor color;
            
            stream >> opId;
            stream >> inputId;
            stream >> color;
            
            OperatorModel* op = model->m_stream->operators()[opId];
            InputModel* input = new InputModel(op, inputId, model->m_undoStack, model);
            input->doSetColor(color);
            observer->insertInput(observer->numInputs(), input);
            model->connect(input, SIGNAL(changed(InputModel*)), model, SLOT(updateInput(InputModel*)));
        }
        
        model->m_observers.append(observer);
    }
    
    return stream;
}





