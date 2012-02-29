#include "ObserverWindow.h"

#include <QAction>
#include "ObserverModel.h"

ObserverWindow::ObserverWindow(ObserverModel* observer, QWidget* parent) 
  : QWidget(parent, Qt::Window),
    m_observer(observer)
{
    setWindowTitle(observer->name());
    
    m_showAct = new QAction(observer->name(), this);
    m_showAct->setStatusTip(tr("Open the observer window"));
    m_showAct->setEnabled(true);
    connect(m_showAct, SIGNAL(triggered()), this, SLOT(show()));
    connect(observer, SIGNAL(nameChanged(QString)), this, SLOT(setWindowTitle(QString)));
    connect(observer, SIGNAL(nameChanged(QString)), this, SLOT(updateActionText(QString)));
}

void ObserverWindow::updateActionText(const QString& name)
{
    setWindowTitle(name);
    m_showAct->setText(name);
}
