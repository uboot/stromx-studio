#include "ErrorListView.h"
#include "Common.h"

#include <QHeaderView>
#include "ErrorListModel.h"
#include "ItemDelegate.h"

ErrorListView::ErrorListView(QWidget* parent)
  : QTableView(parent),
    m_model(0)
{
    m_model = new ErrorListModel(this);
    
    setModel(m_model);
    setItemDelegate(new ItemDelegate(this));
    horizontalHeader()->setResizeMode(QHeaderView::Stretch);
    setAlternatingRowColors(true);
    setShowGrid(false);
    verticalHeader()->hide();
}
