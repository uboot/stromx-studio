/* 
*  Copyright 2013 Matthias Fuchs
*
*  This file is part of stromx-studio.
*
*  Stromx-studio is free software: you can redistribute it and/or modify
*  it under the terms of the GNU General Public License as published by
*  the Free Software Foundation, either version 3 of the License, or
*  (at your option) any later version.
*
*  Stromx-studio is distributed in the hope that it will be useful,
*  but WITHOUT ANY WARRANTY; without even the implied warranty of
*  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
*  GNU General Public License for more details.
*
*  You should have received a copy of the GNU General Public License
*  along with stromx-studio.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef INPUTDELEGATE_H
#define INPUTDELEGATE_H

#include <QStyledItemDelegate>

class InputWidget;

class InputDelegate : public QStyledItemDelegate
{
    Q_OBJECT
    
public:
    static const int ROW_HEIGHT;
    static const int BORDER_OFFSET;
    
    explicit InputDelegate(QObject* parent = 0);
    
    virtual void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    virtual QWidget* createEditor(QWidget* parent, const QStyleOptionViewItem & option, const QModelIndex & index) const;
    virtual void setEditorData (QWidget* editor, const QModelIndex & index) const;
    virtual void setModelData(QWidget* editor, QAbstractItemModel* model, const QModelIndex & index ) const;
    virtual QSize sizeHint(const QStyleOptionViewItem & option, const QModelIndex & index) const;
    
private slots:
    void emitCommitData();
    
private:
    static void setInputWidgetData(InputWidget* widget, const QModelIndex & index);
};

#endif // INPUTDELEGATE_H
