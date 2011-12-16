#include "OperatorLibraryModel.h"

#include <stromx/core/Core.h>
#include <stromx/core/Factory.h>
#include <stromx/core/OperatorKernel.h>


using namespace stromx::core;
    
OperatorLibraryModel::OperatorLibraryModel(QObject* parent)
  : QAbstractItemModel(parent),
    m_factory(0)
{
    m_factory = new stromx::core::Factory();
    stromxRegisterCore(*m_factory);
}

OperatorLibraryModel::~OperatorLibraryModel()
{
    delete m_factory;
}


QModelIndex OperatorLibraryModel::index(int row, int column, const QModelIndex& parent) const
{
    if(! m_factory)
        return QModelIndex();
    
    if(! parent.isValid())
        return createIndex(0, 0);
    
    OperatorKernel* op = static_cast<OperatorKernel*>(parent.internalPointer());
    
    if(op == 0)
        return createIndex(row, column, (void*)(m_factory->availableOperators()[row]));
}

QModelIndex OperatorLibraryModel::parent(const QModelIndex& child) const
{
    
    OperatorKernel* op = static_cast<OperatorKernel*>(child.internalPointer());
    
    if(op == 0)
        return QModelIndex();
    
    return createIndex(0, 0);
}

QVariant OperatorLibraryModel::data(const QModelIndex& index, int role) const
{
    if(role != Qt::DisplayRole)
        return QVariant();
        
    if(! m_factory)
        return QVariant();
    
    OperatorKernel* op = static_cast<OperatorKernel*>(index.internalPointer());
    
    if(op == 0)
        return "Core";
        
    return QVariant(op->type().c_str());
}

int OperatorLibraryModel::columnCount(const QModelIndex& parent) const
{
    return 1;
}

int OperatorLibraryModel::rowCount(const QModelIndex& parent) const
{
    if(! m_factory)
        return 0;
    
    if(! parent.isValid())
        return 1;
    
    OperatorKernel* op = static_cast<OperatorKernel*>(parent.internalPointer());
        
    if(op == 0)
        return m_factory->availableOperators().size();
    
    return 0;
}


