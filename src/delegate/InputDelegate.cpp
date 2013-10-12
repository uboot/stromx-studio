#include "delegate/InputDelegate.h"

#include <QGroupBox>
#include <QPainter>

const int InputDelegate::ROW_HEIGHT = 50;
const int InputDelegate::BORDER_OFFSET = 5;

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
    if (option.state & QStyle::State_Selected)
        painter->fillRect(option.rect, option.palette.highlight());
    else if (option.state)
        painter->fillRect(option.rect, option.palette.background());
    
    QGroupBox box;
    QRect boxRect(0, 0, 
                  option.rect.width() - 2 * BORDER_OFFSET, 
                  option.rect.height() - 2 * BORDER_OFFSET);
    QRect itemRect(option.rect.adjusted(BORDER_OFFSET, BORDER_OFFSET,
                                        -BORDER_OFFSET, -BORDER_OFFSET));
    box.setGeometry(boxRect);
    box.setTitle(index.data(Qt::DisplayRole).toString());
    QPixmap pixmap(boxRect.size());
    QPainter widgetPainter;
 
    widgetPainter.begin(&pixmap);
    box.render(&widgetPainter);
    widgetPainter.end();
    
    painter->drawPixmap(itemRect, pixmap, boxRect);
}

QSize InputDelegate::sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index) const
{
    QSize size = QStyledItemDelegate::sizeHint(option, index);
    size.setHeight(ROW_HEIGHT);
    
    return size;
}


