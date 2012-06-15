#include "ErrorListModel.h"

#include <QDebug>
#include "ErrorEvent.h"

ErrorListModel::ErrorListModel(QObject* parent)
  : QAbstractTableModel(parent),
    m_observer(this)
{
}

int ErrorListModel::columnCount(const QModelIndex& parent) const
{
    return 1;
}

int ErrorListModel::rowCount(const QModelIndex& parent) const
{
    return m_errorList.count();
}

QVariant ErrorListModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    return tr("Error");
}

QVariant ErrorListModel::data(const QModelIndex& index, int role) const
{
    if(role != Qt::DisplayRole)
        return QVariant();
    
    if(index.row() >= m_errorList.count())
        return QVariant();
        
    QString value;
    const ErrorData & data = m_errorList[index.row()];
    
    if(data.description().isEmpty())
        return data.title();
    else
        return tr("%1: %2").arg(data.title()).arg(data.description());
        
        
}

void ErrorListModel::customEvent(QEvent* event)
{
    if(ErrorEvent* errorEvent = dynamic_cast<ErrorEvent*>(event))
    {
        beginInsertRows(QModelIndex(), 0, 0);
        m_errorList.push_front(errorEvent->errorData());
        endInsertRows();
    }
}

void ErrorListModel::clearList()
{
    m_errorList.clear();
    reset();
}

