/* 
*  Copyright 2011 xxx yyy
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

#ifndef PROPERTYVIEW_H
#define PROPERTYVIEW_H

#include <QTreeView>

class QAbstractItemModel;
class QTreeView;

/** 
 * \brief Tree view to display the properties of operators and connections. 
 * 
 * This class displays the contents of an item model. It is intended to be used
 * to display and edit operator models and connection models.
 */
class PropertyView : public QTreeView
{
    Q_OBJECT

public:
    /** Constructs a property view. */
    explicit PropertyView(QWidget *parent = 0);
    
public slots:
    virtual void setModel(QAbstractItemModel* model);
};

#endif // PROPERTYVIEW_H
