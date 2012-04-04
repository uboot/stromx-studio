#include "ItemDelegate.h"

#include <QComboBox>
#include "Common.h"

ItemDelegate::ItemDelegate(QObject* parent)
  : QStyledItemDelegate(parent)
{
}

QWidget* ItemDelegate::createEditor(QWidget* parent, const QStyleOptionViewItem& option, const QModelIndex& index) const
{
    QVariant data = index.model()->data(index, ChoicesRole);
    if(! data.isValid())
        return QStyledItemDelegate::createEditor(parent, option, index);
    
    QList<QVariant> choices = data.toList();
    QComboBox* comboBox = new QComboBox(parent);
    foreach(QVariant label, choices)
        comboBox->addItem(label.toString());
    return comboBox;
}

void ItemDelegate::setEditorData(QWidget* editor, const QModelIndex& index) const
{
    QVariant choices = index.model()->data(index, ChoicesRole);
    if(! choices.isValid())
        QStyledItemDelegate::setEditorData(editor, index);
    
    QVariant data = index.model()->data(index, Qt::EditRole);
    if(QComboBox* comboBox = qobject_cast<QComboBox*>(editor))
        comboBox->setCurrentIndex(data.toInt());    
}

void ItemDelegate::setModelData(QWidget* editor, QAbstractItemModel* model, const QModelIndex& index) const
{
    QVariant choices = index.model()->data(index, ChoicesRole);
    if(! choices.isValid())
        QStyledItemDelegate::setModelData(editor, model, index);
    
    if(QComboBox* comboBox = qobject_cast<QComboBox*>(editor))
        model->setData(index, comboBox->currentIndex(), Qt::EditRole); 
}

void ItemDelegate::updateEditorGeometry(QWidget* editor, const QStyleOptionViewItem& option, const QModelIndex& index) const
{
    QStyledItemDelegate::updateEditorGeometry(editor, option, index);
}
