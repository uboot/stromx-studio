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
    
    setItemDelegate(new ItemDelegate(this));
    setSelectionBehavior(QAbstractItemView::SelectRows);
    setAlternatingRowColors(true);
    setShowGrid(false);
    verticalHeader()->hide();
    
    setModel(m_model);
    horizontalHeader()->setResizeMode(ErrorListModel::TIME, QHeaderView::Interactive);
    horizontalHeader()->setResizeMode(ErrorListModel::DESCRIPTION, QHeaderView::Stretch);
}
