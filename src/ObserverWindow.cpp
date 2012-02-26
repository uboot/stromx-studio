#include "ObserverWindow.h"

#include "ObserverModel.h"

ObserverWindow::ObserverWindow(ObserverModel* observer, QWidget* parent) 
  : QWidget(parent, Qt::Window),
    m_observer(observer)
{
    setWindowTitle(observer->name());
}
