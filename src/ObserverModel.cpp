#include "ObserverModel.h"

#include "InputModel.h"
#include "ObserverTreeModel.h"
#include "RenameObserverCmd.h"

ObserverModel::ObserverModel(QUndoStack* undoStack, ObserverTreeModel* parent)
  : QAbstractTableModel(parent),
    m_undoStack(undoStack),
    m_parent(parent)
{
    m_name = "New observer";
    
    setSupportedDragActions(Qt::MoveAction);
    
    connect(m_parent, SIGNAL(rowsAboutToBeInserted(QModelIndex, int, int)),
            this, SLOT(handleRowsAboutToBeInserted(QModelIndex, int, int)));
    connect(m_parent, SIGNAL(rowsInserted(QModelIndex, int, int)),
            this, SLOT(handleRowsInserted(QModelIndex, int, int)));
    
    connect(m_parent, SIGNAL(rowsAboutToBeRemoved(QModelIndex, int, int)),
            this, SLOT(handleRowsAboutToBeRemoved(QModelIndex, int, int)));
    connect(m_parent, SIGNAL(rowsRemoved(QModelIndex, int, int)),
            this, SLOT(handleRowsRemoved(QModelIndex, int, int)));
}

void ObserverModel::setName(const QString& name)
{
    if(name != m_name)
    {
        QUndoCommand* cmd = new RenameObserverCmd(this, name);
        m_undoStack->push(cmd);
    }
}

void ObserverModel::doSetName(const QString& name)
{
    m_name = name;
    emit nameChanged(name);
    emit changed(this);
}

void ObserverModel::insertInput(int position, InputModel* input)
{
    m_inputs.insert(position, input);
}

void ObserverModel::removeInput(int position)
{
    m_inputs.removeAt(position);
}

InputModel* ObserverModel::input(int position)
{
    return m_inputs[position];
}

int ObserverModel::columnCount(const QModelIndex& parent) const
{
    return 3;
}

int ObserverModel::rowCount(const QModelIndex& parent) const
{
    return numInputs();
}

QVariant ObserverModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if(orientation == Qt::Horizontal && role == Qt::DisplayRole)
    {
        switch(section)
        {
        case 0:
            return tr("Operator");
        case 1:
            return tr("ID");
        case 2:
            return tr("Color");
        }
    }
    
    return QVariant();
}

QModelIndex ObserverModel::index(int row, int column, const QModelIndex& parent) const
{
    return createIndex(row, column, const_cast<ObserverModel*>(this));
}

QVariant ObserverModel::data(const QModelIndex& index, int role) const
{
    return m_parent->data(index, role);
}

void ObserverModel::handleRowsAboutToBeInserted(const QModelIndex& parent, int start, int end)
{
    if(concernsThisObserver(parent))
        beginInsertRows(QModelIndex(), start, end);
}

void ObserverModel::handleRowsInserted(const QModelIndex& parent, int start, int end)
{
    if(concernsThisObserver(parent))
        endInsertRows();
}

void ObserverModel::handleRowsAboutToBeRemoved(const QModelIndex& parent, int start, int end)
{
    if(concernsThisObserver(parent))
        beginRemoveRows(QModelIndex(), start, end);
}

void ObserverModel::handleRowsRemoved(const QModelIndex& parent, int start, int end)
{
    if(concernsThisObserver(parent))
        endRemoveRows();
}

void ObserverModel::handleDataChanged(const QModelIndex& topLeft, const QModelIndex& bottomRight)
{
    if(concernsThisObserver(topLeft) && concernsThisObserver(bottomRight))
        emit dataChanged(topLeft, bottomRight);
}

bool ObserverModel::concernsThisObserver(const QModelIndex & parentModelIndex) const
{
    int observerPos = m_parent->observers().indexOf(const_cast<ObserverModel*>(this));
    return parentModelIndex.isValid() && parentModelIndex.row() == observerPos;
}

QStringList ObserverModel::mimeTypes() const
{
    return m_parent->mimeTypes();
}

Qt::DropActions ObserverModel::supportedDropActions() const
{
    return Qt::MoveAction;
}

bool ObserverModel::dropMimeData(const QMimeData* data, Qt::DropAction action, int row, int column, const QModelIndex& parent)
{
    int observerPos = m_parent->observers().indexOf(const_cast<ObserverModel*>(this));
    if(observerPos >= 0)
        return m_parent->dropMimeData(data, action, parent.row(), 0, createIndex(observerPos, 0));
    
    return false;
}

QMimeData* ObserverModel::mimeData(const QModelIndexList& indexes) const
{
    return m_parent->mimeData(indexes);
}

Qt::ItemFlags ObserverModel::flags(const QModelIndex& index) const
{
    Qt::ItemFlags flags = QAbstractTableModel::flags(index);
    
    return flags |= (Qt::ItemIsDragEnabled | Qt::ItemIsDropEnabled);
}

bool ObserverModel::removeRows(int row, int count, const QModelIndex & parent)
{
    if(concernsThisObserver(parent))
    {
        int observerPos = m_parent->observers().indexOf(const_cast<ObserverModel*>(this));
        return m_parent->removeRows(row, count, createIndex(observerPos, 0));
    }
    
    return false;
}


