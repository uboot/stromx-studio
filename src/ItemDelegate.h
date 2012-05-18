/* 
*  Copyright 2012 Matthias Fuchs
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

#ifndef ITEMDELEGATE_H
#define ITEMDELEGATE_H

#include <QStyledItemDelegate>

/** 
 * \brief Delegate to support custom data in item views.
 * 
 * This class provides the following custom item view editors:
 * - A popup menu if the item view returns a QStringList object
 *   for the role Roles::ChoicesRole. 
 * - A popup menu with colors if the item view returns a QColor
 *   object for the role Roles::ColorRole.
 * - A push button if the item view returns a QString object for 
 *   the role Roles::TriggerRole.
 */
class ItemDelegate : public QStyledItemDelegate
{
    Q_OBJECT
    
public:
    explicit ItemDelegate(QObject* parent = 0);
    
    virtual void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    virtual QWidget* createEditor(QWidget* parent, const QStyleOptionViewItem & option, const QModelIndex & index) const;
    virtual void setEditorData (QWidget* editor, const QModelIndex & index) const;
    virtual void updateEditorGeometry(QWidget * editor, const QStyleOptionViewItem & option, const QModelIndex & index) const;
    virtual void setModelData(QWidget* editor, QAbstractItemModel* model, const QModelIndex & index ) const;

private slots:
    void commitEditEvent();
};

#endif // ITEMDELEGATE_H
