#include "DataManager.h"

DataManager::DataManager(ObserverModel* observer, AbstractDataVisualizer* visualizer, QObject* parent)
  : QObject(parent),
    m_observer(observer),
    m_visualizer(visualizer)
{

}
