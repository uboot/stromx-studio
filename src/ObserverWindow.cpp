#include "ObserverWindow.h"

#include <QAction>
#include <QDockWidget>
#include <QMenuBar>
#include "DataManager.h"
#include "DataVisualizer.h"
#include "LimitUndoStack.h"
#include "ObserverView.h"
#include "model/ObserverModel.h"
#include "model/ObserverTreeModel.h"

ObserverWindow::ObserverWindow(ObserverModel* observer, LimitUndoStack* undoStack, QWidget* parent) 
  : QMainWindow(parent, Qt::Window),
    m_observerView(0),
    m_showAct(0),
    m_visualizer(0),
    m_undoAct(0),
    m_redoAct(0),
    m_resetZoomAct(0),
    m_undoStack(undoStack)
{
    // place the visualizer in the window center
    m_visualizer = new DataVisualizer();
    setCentralWidget(m_visualizer);
    
    // create the list of observers as dock widget
    m_observerView = new ObserverView(observer);
    QDockWidget* inputWidget = new QDockWidget("Inputs");
    inputWidget->setObjectName("InputList");
    inputWidget->setWidget(m_observerView);
    inputWidget->setFeatures(QDockWidget::NoDockWidgetFeatures);
    addDockWidget(Qt::BottomDockWidgetArea, inputWidget);
    
    // create the actions and the menus
    createActions();
    createMenus();
    
    m_showAct = new QAction(observer->name(), this);
    m_showAct->setStatusTip(tr("Open the observer window"));
    m_showAct->setEnabled(true);
    connect(m_showAct, SIGNAL(triggered()), this, SLOT(show()));
    connect(m_showAct, SIGNAL(triggered()), this, SLOT(raise()));
    connect(observer, SIGNAL(nameChanged(QString)), this, SLOT(setWindowTitle(QString)));
    connect(observer, SIGNAL(nameChanged(QString)), this, SLOT(updateWindowTitle(QString)));
    
    setWindowTitle(observer->name());
    
    // allocate the data manager
    DataManager* dataManager = new DataManager(observer, m_visualizer, this);
    connect(dataManager, SIGNAL(dataAccessTimedOut()), observer->parentModel()->streamModel(), SIGNAL(accessTimedOut()));
}

void ObserverWindow::createActions()
{
    m_undoAct = m_undoStack->createLimitUndoAction(this);
    m_undoAct->setShortcuts(QKeySequence::Undo);
    m_redoAct = m_undoStack->createRedoAction(this);
    m_redoAct->setShortcuts(QKeySequence::Redo);

    m_resetZoomAct = m_visualizer->createResetZoomAction(this);
}

void ObserverWindow::createMenus()
{
    QMenu* editMenu = menuBar()->addMenu(tr("&Edit"));
    editMenu->addAction(m_undoAct);
    editMenu->addAction(m_redoAct);

    QMenu* viewMenu = menuBar()->addMenu(tr("&View"));
    viewMenu->addAction(m_resetZoomAct);
}

void ObserverWindow::updateWindowTitle(const QString& name)
{
    setWindowTitle(name);
    m_showAct->setText(name);
}
