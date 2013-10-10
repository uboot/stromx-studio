#include "delegate/InputDelegate.h"

InputDelegate::InputDelegate(QObject* parent)
  : QStyledItemDelegate(parent)
{
}

QWidget* InputDelegate::createEditor(QWidget* parent, const QStyleOptionViewItem& option, const QModelIndex& index) const
{
   return QStyledItemDelegate::createEditor(parent, option, index);
}

void InputDelegate::setEditorData(QWidget* editor, const QModelIndex& index) const
{

    QStyledItemDelegate::setEditorData(editor, index);
}

void InputDelegate::setModelData(QWidget* editor, QAbstractItemModel* model, const QModelIndex& index) const
{

    QStyledItemDelegate::setModelData(editor, model, index);
}

void InputDelegate::paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const
{
    QStyledItemDelegate::paint(painter, option, index);
}

QSize InputDelegate::sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index) const
{
    QSize size = QStyledItemDelegate::sizeHint(option, index);
    
    return size;
}


