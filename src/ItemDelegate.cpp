#include "ItemDelegate.h"

#include <QComboBox>
#include "Common.h"

ItemDelegate::ItemDelegate(QObject* parent)
  : QStyledItemDelegate(parent)
{
}

QWidget* ItemDelegate::createEditor(QWidget* parent, const QStyleOptionViewItem& option, const QModelIndex& index) const
{
    QVariant choices = index.model()->data(index, ChoicesRole);
    if(choices.canConvert(QVariant::List))
    {
        QList<QVariant> list = choices.toList();
        QComboBox* comboBox = new QComboBox(parent);
        foreach(QVariant label, list)
            comboBox->addItem(label.toString());
        return comboBox;
    }
    
    QVariant data = index.model()->data(index, ColorRole);
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
    
    return QStyledItemDelegate::createEditor(parent, option, index);
}

void ItemDelegate::setEditorData(QWidget* editor, const QModelIndex& index) const
{
    QVariant choices = index.model()->data(index, ChoicesRole);
    if(choices.canConvert(QVariant::List))
    {
        QVariant data = index.model()->data(index, Qt::EditRole);
        if(QComboBox* comboBox = qobject_cast<QComboBox*>(editor))
            comboBox->setCurrentIndex(data.toInt());
        return;
    }
    
    QVariant data = index.model()->data(index, ColorRole);
    if(data.canConvert(QVariant::Color))
    {
        QColor color = data.value<QColor>();
        if(QComboBox* comboBox = qobject_cast<QComboBox*>(editor))
            comboBox->setCurrentIndex(comboBox->findData(color, int(Qt::DecorationRole)));
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

void ItemDelegate::updateEditorGeometry(QWidget* editor, const QStyleOptionViewItem& option, const QModelIndex& index) const
{
    QStyledItemDelegate::updateEditorGeometry(editor, option, index);
}
