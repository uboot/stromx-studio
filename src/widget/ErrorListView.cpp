#include "widget/ErrorListView.h"

#include <QBoxLayout>
#include <QHeaderView>
#include <QPushButton>
#include <QTableView>
#include "Common.h"
#include "delegate/ItemDelegate.h"
#include "model/ErrorListModel.h"

ErrorListView::ErrorListView(QWidget* parent)
  : QWidget(parent),
    m_model(0)
{
    m_model = new ErrorListModel(this);
    
    QTableView* tableView = new QTableView;
    
    tableView->setItemDelegate(new ItemDelegate);
    tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    tableView->setAlternatingRowColors(true);
    tableView->setShowGrid(false);
    tableView->verticalHeader()->hide();
    
    tableView->setModel(m_model);
    tableView->horizontalHeader()->setResizeMode(ErrorListModel::TIME, QHeaderView::Interactive);
    tableView->horizontalHeader()->setResizeMode(ErrorListModel::DESCRIPTION, QHeaderView::Stretch);
    
    QPushButton* clearButton = new QPushButton(tr("Clear error log"));
    connect(clearButton, SIGNAL(clicked()), m_model, SLOT(clear()));
    QHBoxLayout* buttonLayout = new QHBoxLayout;
    buttonLayout->addWidget(clearButton);
    buttonLayout->addStretch();
    
    QVBoxLayout* mainLayout = new QVBoxLayout;
    mainLayout->addWidget(tableView);
    mainLayout->addLayout(buttonLayout);
    setLayout(mainLayout);
}
