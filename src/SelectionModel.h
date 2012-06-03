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

/** 
 * \brief Model of a selection of connection and operator models.
 * 
 * This model represents a set of connection and operator models as a
 * PropertyModel object. It allows to display and set the common properties
 * of the set together. Currently only connection models are supported, i.e.
 * selections which include at least one operator model result in an empty
 * model.
 */
class SelectionModel : public PropertyModel
{
    Q_OBJECT
    
public:
    /** Constructs an empty selection model. */
    explicit SelectionModel(QObject* parent);
    
    /** 
     * Sets a selection of item models. Note that while any
     * item model is allowed in \c selection only connection and operator models
     * are recognized by this functions. Any other models are ignored.
     */
    void setSelection(QList<QAbstractItemModel*> selection);
    
    /**
     * Returns true if the model contains a meaningful selection, i.e. a selection
     * of objects of the same type. If this function returns false it makes no sense
     * to display the model because it will not provide any values to an item view.
     */
    bool isValid() const;
    
    // implementation of QAbstractItemModel
    virtual int rowCount(const QModelIndex & index) const;
    virtual QVariant data(const QModelIndex & index, int role) const;
    virtual bool setData(const QModelIndex & index, const QVariant & value, int role);
    virtual QModelIndex index(int row, int column, const QModelIndex& parent) const;
    virtual QModelIndex parent(const QModelIndex& child) const;
    virtual Qt::ItemFlags flags(const QModelIndex & index) const;
    
private:
    /** Returns the first model of the current selection. */
    QAbstractItemModel* model() const;
    
    QList<ConnectionModel*> m_connections;
    
};

#endif // SELECTIONMODEL_H