/* 
*  Copyright 2011 Matthias Fuchs
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

#ifndef OPERATORLIBRARYMODEL_H
#define OPERATORLIBRARYMODEL_H

#include <QAbstractItemModel>
#include <QMap>
#include <QVector>
#include <QStringList>

namespace stromx 
{
    namespace core 
    {
        class Factory;
    }
}

class OperatorLibraryModel : public QAbstractItemModel
{
public:
    friend class OperatorLibraryModelTest;
    
    explicit OperatorLibraryModel(QObject *parent = 0);
    
    ~OperatorLibraryModel();
    
    QModelIndex index(int row, int column, const QModelIndex & parent) const;
    QModelIndex parent(const QModelIndex & child) const;
    
    QVariant data(const QModelIndex & index, int role = Qt::DisplayRole) const;
    
    int rowCount(const QModelIndex & parent) const;
    int columnCount(const QModelIndex & parent) const;
    
    void loadLibraries(const QStringList & libraries);
private:
    void updateOperators();
    
    QMap<QString, QStringList> m_package2TypeMap;
    QMap<unsigned int, QString> m_index2PackageMap;
    QVector<void*> m_libraryHandles;
    stromx::core::Factory* m_factory;
};

#endif // OPERATORLIBRARYMODEL_H
