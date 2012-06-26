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

#ifndef PROPERTYMODEL_H
#define PROPERTYMODEL_H

#include <QAbstractItemModel>

/**
 * \brief Abstract base class of property models.
 * 
 * This class is the base class of all property model in stromx-studio.
 * Property models have two columns. The first one contains the identifiers of
 * the properties of an object, the second one their values.
 */
class PropertyModel : public QAbstractItemModel
{
    Q_OBJECT
    
public:
    /** The columns of the property model. */
    enum Column
    {
        /** The name of the property. */
        PROPERTY,
        /** The value of the property. */
        VALUE,
        /** The total number of columns. */
        NUM_COLUMNS
    };
    
    explicit PropertyModel(QObject* parent = 0);
    
    virtual int columnCount(const QModelIndex & index) const;
    virtual QVariant headerData(int section, Qt::Orientation orientation, int role) const;
};

#endif // PROPERTYMODEL_H