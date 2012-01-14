#include "ThreadEditor.h"

#include <QHBoxLayout>
#include <QHeaderView>
#include <QTableView>

ThreadEditor::ThreadEditor(QWidget* parent)
  : QWidget(parent)
{    
    m_table = new QTableView;
    m_table->verticalHeader()->hide();
    
    QHBoxLayout* layout = new QHBoxLayout;
    layout->addWidget(m_table);
    
    setLayout(layout);
}

void ThreadEditor::setModel(QAbstractTableModel* model)
{
    m_table->setModel(model);
    m_table->horizontalHeader()->setResizeMode(QHeaderView::Stretch);
}
