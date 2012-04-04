#include "PropertyEditor.h"

#include <QHeaderView>
#include <QTableView>

#include "ItemDelegate.h"

PropertyEditor::PropertyEditor(QWidget* parent)
  : QDockWidget(parent)
{
    setWindowTitle(tr("Properties"));
    setObjectName("PropertyEditor");
    
    m_table = new QTableView;
    m_table->verticalHeader()->hide();
    setWidget(m_table);
    
    m_table->setItemDelegate(new ItemDelegate(this));
}

void PropertyEditor::setModel(QAbstractTableModel* model)
{
    m_table->setModel(model);
    m_table->horizontalHeader()->setResizeMode(QHeaderView::Stretch);
}

