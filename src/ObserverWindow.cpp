#include "ObserverWindow.h"

#include <QAction>
#include <QDockWidget>
#include <QHeaderView>
#include <QTableView>
#include <QVBoxLayout>
#include <QSplitter>
#include "DataVisualizer.h"
#include "DataManager.h"
#include "DataVisualizer.h"
#include "ItemDelegate.h"
#include "ObserverModel.h"

ObserverWindow::ObserverWindow(ObserverModel* observer, QWidget* parent) 
  : QMainWindow(parent, Qt::Window),
    m_observer(observer)
{
    DataVisualizer* visualizer = new DataVisualizer();
    setCentralWidget(visualizer);
    
    QTableView* inputList = new QTableView();
    inputList->setModel(observer);
    inputList->setDragDropMode(QAbstractItemView::DragDrop);
    inputList->setSelectionBehavior(QAbstractItemView::SelectRows);
    inputList->setSelectionMode(QAbstractItemView::SingleSelection);
    inputList->horizontalHeader()->setResizeMode(QHeaderView::Stretch);
    inputList->setItemDelegate(new ItemDelegate(this));
    inputList->setEditTriggers(QAbstractItemView::AllEditTriggers);
    
    QDockWidget* inputWidget = new QDockWidget("Inputs");
    inputWidget->setObjectName("InputList");
    inputWidget->setWidget(inputList);
    inputWidget->setFeatures(QDockWidget::NoDockWidgetFeatures);
    addDockWidget(Qt::BottomDockWidgetArea, inputWidget);
    
    m_showAct = new QAction(observer->name(), this);
    m_showAct->setStatusTip(tr("Open the observer window"));
    m_showAct->setEnabled(true);
    connect(m_showAct, SIGNAL(triggered()), this, SLOT(show()));
    connect(observer, SIGNAL(nameChanged(QString)), this, SLOT(setWindowTitle(QString)));
    connect(observer, SIGNAL(nameChanged(QString)), this, SLOT(updateActionText(QString)));
    
    setWindowTitle(observer->name());
    
    // allocate the data manager
    new DataManager(observer, visualizer, this);
}

void ObserverWindow::updateActionText(const QString& name)
{
    setWindowTitle(name);
    m_showAct->setText(name);
}
