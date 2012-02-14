#include "ObserverModel.h"

#include "ObserverListModel.h"

ObserverModel::ObserverModel(ConnectionModel* connection, ObserverListModel* parent)
  : QObject(parent),
    m_listModel(parent)
{

}
