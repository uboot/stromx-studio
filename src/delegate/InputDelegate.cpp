#include "delegate/InputDelegate.h"

#include <QPainter>
#include <QApplication>

#include "Common.h"
#include "delegate/InputEditWidget.h"
#include "delegate/InputPaintWidget.h"
#include <visualization/Visualization.h>
#include <visualization/VisualizationRegistry.h>

const int InputDelegate::ROW_HEIGHT = 150;
const int InputDelegate::BORDER_OFFSET = 5;

InputDelegate::InputDelegate(QObject* parent)
  : QStyledItemDelegate(parent)
{
}

QWidget* InputDelegate::createEditor(QWidget* parent,
                                     const QStyleOptionViewItem& /*option*/,
                                     const QModelIndex& index) const
{
    InputEditWidget* editor = new InputEditWidget(index, parent);
    connect(editor, SIGNAL(dataChanged()), this, SLOT(emitCommitData()));
    return editor;
}

void InputDelegate::setEditorData(QWidget* editor, const QModelIndex& index) const
{
    InputEditWidget* widget = qobject_cast<InputEditWidget*>(editor);
    QVariant data = index.data(VisualizationStateRole);
    
    widget->setInputTitle(index.data(Qt::DisplayRole).toString());
    
    if (data.canConvert<VisualizationState>())
        widget->setState(data.value<VisualizationState>());
}

void InputDelegate::setModelData(QWidget* editor, QAbstractItemModel* model, const QModelIndex& index) const
{
    InputEditWidget* widget = qobject_cast<InputEditWidget*>(editor);
    VisualizationState state = widget->state();
    
    model->setData(index, QVariant::fromValue(state), VisualizationStateRole);
}

void InputDelegate::paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& /*index*/) const
{
    painter->fillRect(option.rect, option.palette.background());
    return;
}

QSize InputDelegate::sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index) const
{
    QSize size = QStyledItemDelegate::sizeHint(option, index);
    size.setHeight(ROW_HEIGHT);
    
    return size;
}

void InputDelegate::emitCommitData()
{
    QWidget* widget = qobject_cast<QWidget* >(sender());
    emit commitData(widget);
}
