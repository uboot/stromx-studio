#include "model/ErrorListModel.h"

#include <QDebug>
#include "event/ErrorEvent.h"

const qint32 ErrorListModel::MAX_ERRORS = 100;

ErrorListModel::ErrorListModel(QObject* parent)
  : QAbstractTableModel(parent),
    m_observer(this)
{
}

int ErrorListModel::columnCount(const QModelIndex& /*parent*/) const
{
    return 2;
}

int ErrorListModel::rowCount(const QModelIndex& /*parent*/) const
{
    return m_errorList.count();
}

QVariant ErrorListModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if(orientation == Qt::Vertical)
        return QVariant();
    
    if(role != Qt::DisplayRole)
        return QVariant();
    
    switch(section)
    {
    case TIME:
        return tr("Time");
    case DESCRIPTION:
        return tr("Description");
    default:
        return QVariant();
    }
}

QVariant ErrorListModel::data(const QModelIndex& index, int role) const
{
    if(role != Qt::DisplayRole)
        return QVariant();
    
    if(index.row() >= m_errorList.count())
        return QVariant();
     
    QString value;
    const ErrorData & data = m_errorList[index.row()];
    
    switch(index.column())
    {
    case TIME:
        return data.time().time().toString();
    case DESCRIPTION:
        if(data.description().isEmpty())
            return data.title();
        else
            return tr("%1: %2").arg(data.title()).arg(data.description());
    default:
        return QVariant();
    }
        
        
}

void ErrorListModel::customEvent(QEvent* event)
{
    if(event->type() == ErrorEvent::TYPE)
    {
        ErrorEvent* errorEvent = reinterpret_cast<ErrorEvent*>(event);
        
        // add the error to the list
        beginInsertRows(QModelIndex(), 0, 0);
        m_errorList.push_front(errorEvent->errorData());
        endInsertRows();
        
        // if necessary remove errors at the end of the list
        if(m_errorList.count() > MAX_ERRORS)
        {
            beginRemoveRows(QModelIndex(), MAX_ERRORS, m_errorList.count());
            while(m_errorList.count() > MAX_ERRORS)
                m_errorList.removeLast();
            endRemoveRows();
        }
    }
}

void ErrorListModel::clear()
{
    m_errorList.clear();
    reset();
}

