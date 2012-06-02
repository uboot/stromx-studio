#include "ObserverWindow.h"

#include <QAction>
#include <QDockWidget>
#include "DataManager.h"
#include "DataVisualizer.h"
#include "ObserverModel.h"
#include "ObserverView.h"

ObserverWindow::ObserverWindow(ObserverModel* observer, QWidget* parent) 
  : QMainWindow(parent, Qt::Window),
    m_observerView(0)
{
    m_visualizer = new DataVisualizer();
    setCentralWidget(m_visualizer);
    
    m_observerView = new ObserverView(observer);
    QDockWidget* inputWidget = new QDockWidget("Inputs");
    inputWidget->setObjectName("InputList");
    inputWidget->setWidget(m_observerView);
    inputWidget->setFeatures(QDockWidget::NoDockWidgetFeatures);
    addDockWidget(Qt::BottomDockWidgetArea, inputWidget);
    
    m_showAct = new QAction(observer->name(), this);
    m_showAct->setStatusTip(tr("Open the observer window"));
    m_showAct->setEnabled(true);
    connect(m_showAct, SIGNAL(triggered()), this, SLOT(show()));
    connect(observer, SIGNAL(nameChanged(QString)), this, SLOT(setWindowTitle(QString)));
    connect(observer, SIGNAL(nameChanged(QString)), this, SLOT(updateWindowTitle(QString)));
    
    setWindowTitle(observer->name());
    
    // allocate the data manager
    new DataManager(observer, m_visualizer, this);
}

void ObserverWindow::updateWindowTitle(const QString& name)
{
    setWindowTitle(name);
    m_showAct->setText(name);
}
