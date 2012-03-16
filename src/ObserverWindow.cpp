#include "ObserverWindow.h"

#include <QAction>
#include <QHeaderView>
#include <QTableView>
#include <QVBoxLayout>
#include <QSplitter>
#include "ObserverModel.h"
#include "DataVisualizer.h"

ObserverWindow::ObserverWindow(ObserverModel* observer, QWidget* parent) 
  : QWidget(parent, Qt::Window),
    m_observer(observer)
{
    QGraphicsView* display = new DataVisualizer();
    QTableView* inputList = new QTableView();
    inputList->setModel(observer);
    inputList->setDragDropMode(QAbstractItemView::DragDrop);
    inputList->setSelectionBehavior(QAbstractItemView::SelectRows);
    inputList->setSelectionMode(QAbstractItemView::SingleSelection);
    inputList->horizontalHeader()->setResizeMode(QHeaderView::Stretch);
    
    QSplitter* splitter = new QSplitter(Qt::Vertical);
    splitter->addWidget(display);
    splitter->addWidget(inputList);
    
    QVBoxLayout* layout = new QVBoxLayout;
    layout->addWidget(splitter);
    setLayout(layout);
    
    m_showAct = new QAction(observer->name(), this);
    m_showAct->setStatusTip(tr("Open the observer window"));
    m_showAct->setEnabled(true);
    connect(m_showAct, SIGNAL(triggered()), this, SLOT(show()));
    connect(observer, SIGNAL(nameChanged(QString)), this, SLOT(setWindowTitle(QString)));
    connect(observer, SIGNAL(nameChanged(QString)), this, SLOT(updateActionText(QString)));
    
    setWindowTitle(observer->name());
}

void ObserverWindow::updateActionText(const QString& name)
{
    setWindowTitle(name);
    m_showAct->setText(name);
}
