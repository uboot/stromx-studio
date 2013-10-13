#include "delegate/InputDelegate.h"

#include <QPainter>
#include <QApplication>

#include "Common.h"
#include "widget/InputEditWidget.h"
#include "widget/InputPaintWidget.h"

const int InputDelegate::ROW_HEIGHT = 100;
const int InputDelegate::BORDER_OFFSET = 5;

InputDelegate::InputDelegate(QObject* parent)
  : QStyledItemDelegate(parent)
{
}

QWidget* InputDelegate::createEditor(QWidget* parent,
                                     const QStyleOptionViewItem& /*option*/,
                                     const QModelIndex& /*index*/) const
{
    InputEditWidget* editor = new InputEditWidget(parent);
    connect(editor, SIGNAL(dataChanged()), this, SLOT(commitEditEvent()));
    return editor;
}

void InputDelegate::setEditorData(QWidget* editor, const QModelIndex& index) const
{
    InputEditWidget* widget = qobject_cast<InputEditWidget*>(editor);
    setInputWidgetData(widget, index);
}

void InputDelegate::setModelData(QWidget* editor, QAbstractItemModel* model, const QModelIndex& index) const
{
    InputEditWidget* widget = qobject_cast<InputEditWidget*>(editor);
    AbstractDataVisualizer::VisualizationProperties properties;
    
    properties["color"] = widget->inputColor();
    properties["active"] = widget->inputActive();
    properties["visualization"] = widget->visualizationType();
    
    model->setData(index, properties, VisualizationPropertiesRole);
}

void InputDelegate::paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const
{
    if (option.state & QStyle::State_Selected)
    {
        painter->fillRect(option.rect, option.palette.background());
        return;
    }
    
    InputPaintWidget box;
    setInputWidgetData(&box, index);
    QRect widgetRect(0, 0, option.rect.width(), option.rect.height());
    box.setGeometry(widgetRect);
    box.setInputTitle(index.data(Qt::DisplayRole).toString());
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

void InputDelegate::setInputWidgetData(InputWidget* widget, const QModelIndex& index)
{
    widget->setInputTitle(index.data(Qt::DisplayRole).toString());
    
    AbstractDataVisualizer::VisualizationProperties properties
        = index.data(VisualizationPropertiesRole).toMap();
        
    QColor color = properties.value("color", Qt::black).value<QColor>();
    widget->setInputColor(color);
    
    bool isActive = properties.value("active", true).toBool();
    widget->setInputActive(isActive);
    
    int visualizationType = properties.value("visualization", 0).toInt();
    widget->setVisualizationType(visualizationType);
}

void InputDelegate::commitEditEvent()
{
    QWidget* widget = qobject_cast<QWidget* >(sender());
    emit commitData(widget);
}
