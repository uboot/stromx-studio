#include "PropertyEditor.h"

#include <QHeaderView>
#include <QTableView>

PropertyEditor::PropertyEditor(QWidget* parent)
  : QDockWidget(parent)
{
    setWindowTitle(tr("Properties"));
    setObjectName("PropertyEditor");
    
    m_table = new QTableView;
    m_table->verticalHeader()->hide();
    setWidget(m_table);
}

void PropertyEditor::setModel(QAbstractTableModel* model)
{
    m_table->setModel(model);
}

