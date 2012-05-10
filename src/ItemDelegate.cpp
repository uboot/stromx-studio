#include "ItemDelegate.h"

#include <QComboBox>
#include <QPushButton>
#include <QApplication>
#include "Common.h"

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
        QStringList colorNames = QColor::colorNames();
        QComboBox* comboBox = new QComboBox(parent);
        for (int i = 0; i < colorNames.size(); ++i)
        {
            QColor color(colorNames[i]);
            comboBox->insertItem(i, colorNames[i]);
            comboBox->setItemData(i, color, Qt::DecorationRole);
        }
        return comboBox;
    }
    
    data = index.model()->data(index, TriggerRole);
    if(data.canConvert(QVariant::String))
    {
        QPushButton* button = new QPushButton(parent);
        connect(button, SIGNAL(clicked()), this, SLOT(commitTriggerEvent()));
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
        QStyledItemDelegate::paint(painter, option, index);
}

void ItemDelegate::commitTriggerEvent()
{
    QPushButton* pushButton = qobject_cast<QPushButton* >(sender());
    emit commitData(pushButton);
}


void ItemDelegate::updateEditorGeometry(QWidget* editor, const QStyleOptionViewItem& option, const QModelIndex& index) const
{
    QStyledItemDelegate::updateEditorGeometry(editor, option, index);
}
