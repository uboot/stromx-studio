#include "ItemDelegate.h"

#include <QApplication>
#include <QComboBox>
#include <QPainter>
#include <QPushButton>
#include <QMap>
#include "Common.h"
#include "ChooseImageButton.h"
#include "TriggerButton.h"

const int ItemDelegate::ROW_HEIGHT = 25;

ItemDelegate::ItemDelegate(QObject* parent)
  : QStyledItemDelegate(parent)
{
}

QWidget* ItemDelegate::createEditor(QWidget* parent, const QStyleOptionViewItem& option, const QModelIndex& index) const
{
    QVariant data = index.model()->data(index, ChoicesRole);
    if(data.canConvert(QVariant::List))
    {
        QList<QVariant> list = data.toList();
        QComboBox* comboBox = new QComboBox(parent);
        foreach(QVariant label, list)
            comboBox->addItem(label.toString());
        return comboBox;
    }
    
    data = index.model()->data(index, ColorRole);
    if(data.canConvert(QVariant::Color))
    {
        QComboBox* comboBox = new QComboBox(parent);
        
        int i = 0;
        QMapIterator<QString, QColor> iter(colorTable());
        while (iter.hasNext())
        {
            iter.next();
            comboBox->insertItem(i, iter.key());
            comboBox->setItemData(i, iter.value(), Qt::DecorationRole);
            ++i;
        }
        return comboBox;
    }
    
    data = index.model()->data(index, TriggerRole);
    if(data.canConvert(QVariant::String))
    {
        TriggerButton* button = new TriggerButton(parent);
        connect(button, SIGNAL(pushedTriggerButton()), this, SLOT(commitEditEvent()));
        return button;
    }
    
    data = index.model()->data(index, ImageRole);
    if(data.canConvert(QVariant::String))
    {
        QPushButton* button = new ChooseImageButton(parent);
        connect(button, SIGNAL(choseImage()), this, SLOT(commitEditEvent()));
        return button;
    }
    
    return QStyledItemDelegate::createEditor(parent, option, index);
}

void ItemDelegate::setEditorData(QWidget* editor, const QModelIndex& index) const
{
    QVariant data = index.model()->data(index, ChoicesRole);
    if(data.canConvert(QVariant::List))
    {
        QVariant intData = index.model()->data(index, Qt::EditRole);
        if(QComboBox* comboBox = qobject_cast<QComboBox*>(editor))
            comboBox->setCurrentIndex(intData.toInt());
        return;
    }
    
    data = index.model()->data(index, ColorRole);
    if(data.canConvert(QVariant::Color))
    {
        QColor color = data.value<QColor>();
        if(QComboBox* comboBox = qobject_cast<QComboBox*>(editor))
            comboBox->setCurrentIndex(comboBox->findData(color, int(Qt::DecorationRole)));
        return;
    }
    
    data = index.model()->data(index, TriggerRole);
    if(data.canConvert(QVariant::String))
    {
        QString text = data.value<QString>();
        if(QPushButton* button = qobject_cast<QPushButton*>(editor))
            button->setText(text);
        return;
    }
    
    QStyledItemDelegate::setEditorData(editor, index);
}

void ItemDelegate::setModelData(QWidget* editor, QAbstractItemModel* model, const QModelIndex& index) const
{
    QVariant choices = index.model()->data(index, ChoicesRole);
    if(choices.canConvert(QVariant::List))
    {
        if(QComboBox* comboBox = qobject_cast<QComboBox*>(editor))
            model->setData(index, comboBox->currentIndex(), Qt::EditRole); 
        return;
    }
    
    QVariant data = index.model()->data(index, ColorRole);
    if(data.canConvert(QVariant::Color))
    {
        if(QComboBox* comboBox = qobject_cast<QComboBox*>(editor))
        {
            QVariant color = comboBox->itemData(comboBox->currentIndex(), Qt::DecorationRole);
            model->setData(index, color, Qt::EditRole);
        }
        return;
    }
    
    data = index.model()->data(index, TriggerRole);
    if(data.canConvert(QVariant::String))
    {
        if(TriggerButton* button = qobject_cast<TriggerButton*>(editor))
        {
            if(button->triggerPending())
                model->setData(index, true, Qt::EditRole);
            button->reset();
        }
        return;
    }
    
    data = index.model()->data(index, ImageRole);
    if(data.canConvert(QVariant::String))
    {
        if(ChooseImageButton* button = qobject_cast<ChooseImageButton*>(editor))
        {
            QImage image = button->image();
            if(! image.isNull())
                model->setData(index, image, Qt::EditRole);
            button->reset();
        }
        return;
    }
    
    QStyledItemDelegate::setModelData(editor, model, index);
}

void ItemDelegate::paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const
{
    QVariant trigger = index.model()->data(index, TriggerRole);
    if(trigger.canConvert(QVariant::String))
    {
        QStyleOptionButton buttonOption;
        buttonOption.rect = option.rect;
        buttonOption.text = QString(trigger.toString());
        QApplication::style()->drawControl(QStyle::CE_PushButton, &buttonOption, painter);
    }
    else
    {
        QStyledItemDelegate::paint(painter, option, index);
    }
}

QSize ItemDelegate::sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index) const
{
    QSize size = QStyledItemDelegate::sizeHint(option, index);
    size.setHeight(ROW_HEIGHT);
    
    return size;
}

void ItemDelegate::commitEditEvent()
{
    QWidget* pushButton = qobject_cast<QWidget* >(sender());
    emit commitData(pushButton);
}

void ItemDelegate::updateEditorGeometry(QWidget* editor, const QStyleOptionViewItem& option, const QModelIndex& index) const
{
    QStyledItemDelegate::updateEditorGeometry(editor, option, index);
}


