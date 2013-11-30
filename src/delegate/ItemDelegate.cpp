#include "delegate/ItemDelegate.h"

#include <QApplication>
#include <QComboBox>
#include <QPainter>
#include <QPushButton>
#include <QSpinBox>
#include <QMap>

#include "Common.h"
#include "Matrix.h"
#include "delegate/ChooseImageButton.h"
#include "delegate/EditMatrixButton.h"
#include "delegate/TriggerButton.h"

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
        connect(comboBox, SIGNAL(currentIndexChanged(int)),
                this, SLOT(commitEditEvent()));
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
        connect(comboBox, SIGNAL(currentIndexChanged(int)),
                this, SLOT(commitEditEvent()));
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
    
    data = index.data(MatrixRole);
    if(data.canConvert<Matrix>())
    {
        // get the matrix
        const Matrix matrix = data.value<Matrix>();
        
        // get and information about the matrix dimensions
        int rows = -1;
        int cols = -1;
        data = index.data(NumRowsRole);
        if (data.canConvert(QVariant::Int))
            rows = data.toInt();
        
        data = index.data(NumColsRole);
        if (data.canConvert(QVariant::Int))
            cols = data.toInt();
        
        EditMatrixButton* button = new EditMatrixButton(rows, cols, index, parent);
        connect(button, SIGNAL(editedMatrix()), this, SLOT(commitEditEvent()));
        
        return button;
    }
    
    // all other cases can be handled with the standard editors obtained
    // from the parent class
    QWidget* editor = QStyledItemDelegate::createEditor(parent, option, index);
    
    // if the widget is a spin box try to get reasonable paramters (min, max, step) for it
    if(QSpinBox* spinBox = qobject_cast<QSpinBox*>(editor))
        setSpinBoxParameters(index, spinBox);
    
    // if the widget is a double spin box try to get reasonable paramters (min, max, step) for it
    if(QDoubleSpinBox* spinBox = qobject_cast<QDoubleSpinBox*>(editor))
        setDoubleSpinBoxParameters(index, spinBox);
    
    // return the editor
    return editor;
}

bool ItemDelegate::canConvertToInt(const QVariant value)
{
    if(value.type() == QVariant::UInt)
        return value.toInt() >= 0;
    
    if(value.type() == QVariant::Int)
        return true;
    
    return false;
}

void ItemDelegate::setSpinBoxParameters(const QModelIndex& index, QSpinBox* spinBox) const
{
    QVariant min = index.model()->data(index, MinRole);
    if(canConvertToInt(min))
        spinBox->setMinimum(min.toInt());
    
    QVariant max = index.model()->data(index, MaxRole);
    if(canConvertToInt(max))
        spinBox->setMaximum(max.toInt());
    
    QVariant step = index.model()->data(index, StepRole);
    if(canConvertToInt(step))
        spinBox->setSingleStep(step.toInt());
}

void ItemDelegate::setDoubleSpinBoxParameters(const QModelIndex& index, QDoubleSpinBox* spinBox) const
{
    QVariant min = index.model()->data(index, MinRole);
    if(min.canConvert(QVariant::Double))
        spinBox->setMinimum(min.toDouble());
    
    QVariant max = index.model()->data(index, MaxRole);
    if(max.canConvert(QVariant::Double))
        spinBox->setMaximum(max.toDouble());
    
    QVariant step = index.model()->data(index, StepRole);
    if(step.canConvert(QVariant::Double))
        spinBox->setSingleStep(step.toDouble());
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
    
    data = index.model()->data(index, MatrixRole);
    if(data.canConvert<Matrix>())
    {
        Matrix matrix = data.value<Matrix>();
        if(EditMatrixButton* button = qobject_cast<EditMatrixButton*>(editor))
            button->setMatrix(matrix);
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
    
    data = index.model()->data(index, MatrixRole);
    if(data.canConvert<Matrix>())
    {
        if(EditMatrixButton* button = qobject_cast<EditMatrixButton*>(editor))
        {
            QVariant variantData;
            variantData.setValue(button->matrix());
            model->setData(index, variantData, Qt::EditRole);
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
    QWidget* widget = qobject_cast<QWidget* >(sender());
    emit commitData(widget);
}



