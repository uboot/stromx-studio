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

#ifndef SELECTIONMODEL_H
#define SELECTIONMODEL_H

#include "PropertyModel.h"

class ConnectionModel;

class SelectionModel : public PropertyModel
{
    Q_OBJECT
    
public:
    explicit SelectionModel(QObject* parent);
    
    void setSelection(QList<QAbstractItemModel*> selection);
    
    bool isValid() const;
    
    // implementation of QAbstractItemModel
    virtual int rowCount(const QModelIndex & index) const;
    virtual QVariant data(const QModelIndex & index, int role) const;
    virtual bool setData(const QModelIndex & index, const QVariant & value, int role);
    virtual QModelIndex index(int row, int column, const QModelIndex& parent) const;
    virtual QModelIndex parent(const QModelIndex& child) const;
    virtual Qt::ItemFlags flags(const QModelIndex & index) const;
    
private:
    QList<ConnectionModel*> m_connections;
};

#endif // SELECTIONMODEL_H