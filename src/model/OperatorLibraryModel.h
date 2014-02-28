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
    namespace runtime 
    {
        class Factory;
        class OperatorKernel;
    }
}

class OperatorData;

/** 
 * \brief Model of an operator library.
 * 
 * An operator library contains a list of operators which serve as 
 * prototypes for new operators. The operators are categorized into 
 * packages. This class implements this tree structure as an item model
 * which can provide data for a tree view.
 * 
 * Operators can be added to the library by loading stromx packages.
 * Stromx packages are contained in shared libraries providing a function
 * which registers the operator and the data types of the package with
 * the library.
 * 
 * Internally, the operators in the operator list are stored in a stromx
 * factory.
 */
class OperatorLibraryModel : public QAbstractItemModel
{
    Q_OBJECT
    
public:
    /** The columns of the operator library. */
    enum Column
    {
        /** The name of the operator or the package. */
        OPERATOR,
        /** The version of the operator. */
        VERSION,
        /** The total number of columns. */
        NUM_COLUMNS
    };
    
    /** 
     * Constructs an operator library model which contains all operators
     * of the packages \em runtime and \em base.
     * \attention In the future, \em base will \em not be loaded in the
     *            constructor.
     */
    explicit OperatorLibraryModel(QObject *parent = 0);
    virtual ~OperatorLibraryModel();
    
    virtual QModelIndex index(int row, int column, const QModelIndex & parent) const;
    virtual QModelIndex parent(const QModelIndex & child) const;
    virtual QVariant data(const QModelIndex & index, int role = Qt::DisplayRole) const;
    virtual QVariant headerData(int section, Qt::Orientation orientation, int role) const;
    
    virtual int rowCount(const QModelIndex & parent) const;
    virtual int columnCount(const QModelIndex & parent) const;
    
    /** Load the operator package located at \c packagePath. */
    void loadPackage(const QString& packagePath);
    
    /** 
     * Removes all previously loaded packages from the library.
     * All packages which have been loaded in the constructor are
     * preserved (i.e. they are automatically reloaded after resetting).
     */
    void resetLibrary();
    
    /** Returns true if \c index points to a valid operator. */
    bool isOperator(const QModelIndex & index) const;
    
    /** 
     * Allocates an operator data object for the operator which \c index
     * points to and returns a pointer to the object. The client is responsible
     * for deleting the object. If \c index does not point to a valid operator
     * 0 is returned.
     */
    OperatorData* newOperatorData(const QModelIndex & index) const;
    
    /** 
     * Allocates and returns a stromx operator from \c data. The client is 
     * responsible for deleting the returned operator. If the operator 
     * can not be allocated (e.g. \c data does not refer to an operator contained
     * in the library) 0 is returned.
     */
    stromx::runtime::OperatorKernel* newOperator(const OperatorData* data) const;
    
    /**  Returns a reference to the stromx factory of the operator library. */
    stromx::runtime::Factory* factory() const { return m_factory; }
    
private:
    struct Item
    {
        Item() : op(0), parent(0) {}
        ~Item();
        
        QString package;
        const stromx::runtime::OperatorKernel* op;
        Item* parent;
        QList<Item*> children;
    };
    
    void updateOperators();
    void setupFactory();
    
    Item* m_root;
    QStringList m_loadedPackages;
    stromx::runtime::Factory* m_factory;
};

#endif // OPERATORLIBRARYMODEL_H
