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
    return 1;
}

QVariant ErrorListModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    return QVariant();
}

QVariant ErrorListModel::data(const QModelIndex& index, int role) const
{
    return QVariant();
}

void ErrorListModel::customEvent(QEvent* event)
{
    if(ErrorEvent* errorEvent = dynamic_cast<ErrorEvent*>(event))
    {
        qWarning() << errorEvent->errorData().title();
    }
}
