#include "PropertyEditor.h"

#include <QHeaderView>
#include <QTreeView>
#include "ItemDelegate.h"

PropertyEditor::PropertyEditor(QWidget* parent)
  : QDockWidget(parent)
{
    setWindowTitle(tr("Properties"));
    setObjectName("PropertyEditor");
    
    m_view = new QTreeView;
    m_view->setEditTriggers(QAbstractItemView::AllEditTriggers);
    setWidget(m_view);
    
    m_view->setItemDelegate(new ItemDelegate(this));
}

void PropertyEditor::setModel(QAbstractItemModel* model)
{
    m_view->setModel(model);
    m_view->header()->setResizeMode(QHeaderView::Stretch);
}

