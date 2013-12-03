#include "delegate/InputDelegate.h"

#include <QPainter>
#include <QApplication>

#include "Common.h"
#include "delegate/InputEditWidget.h"
#include "delegate/InputPaintWidget.h"
#include <visualization/Visualization.h>
#include <visualization/VisualizationRegistry.h>

const int InputDelegate::ROW_HEIGHT = 100;
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
    
    if (data.canConvert<VisualizationState>())
        widget->setState(data.value<VisualizationState>());
}

void InputDelegate::setModelData(QWidget* editor, QAbstractItemModel* model, const QModelIndex& index) const
{
    InputEditWidget* widget = qobject_cast<InputEditWidget*>(editor);
    VisualizationState state = widget->state();
    
    model->setData(index, QVariant::fromValue(state), VisualizationStateRole);
}

void InputDelegate::paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const
{
    if (option.state & QStyle::State_Selected)
    {
        painter->fillRect(option.rect, option.palette.background());
        return;
    }
    
    InputPaintWidget box;
    QRect widgetRect(0, 0, option.rect.width(), option.rect.height());
    box.setGeometry(widgetRect);
    box.setInputTitle(index.data(Qt::DisplayRole).toString());
    QVariant stateVariant = index.data(VisualizationStateRole);
    if (stateVariant.canConvert<VisualizationState>())
    {
        VisualizationState state = stateVariant.value<VisualizationState>();
        box.setInputActive(state.isActive());
        const Visualization* visualization = VisualizationRegistry::visualization(state.currentVisualization());
        if (visualization)
            box.setVisualizationType(visualization->name());
    }
    
    QPixmap pixmap(option.rect.size());
    QPainter widgetPainter;
 
    widgetPainter.begin(&pixmap);
    box.render(&widgetPainter);
    widgetPainter.end();
    
    painter->drawPixmap(option.rect, pixmap, widgetRect);
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
