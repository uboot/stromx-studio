#include "OperatorLibraryModel.h"

#include "Exception.h"
#include <stromx/core/Core.h>
#include <stromx/core/Factory.h>
#include <stromx/core/OperatorKernel.h>
#include <dlfcn.h>
#include <iostream>


using namespace stromx::core;
    
OperatorLibraryModel::OperatorLibraryModel(QObject* parent)
  : QAbstractItemModel(parent),
    m_factory(0)
{
    m_factory = new stromx::core::Factory();
    stromxRegisterCore(*m_factory);
    
    updateOperators();
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

void OperatorLibraryModel::loadLibrary(const QStringList& libraries)
{
    QStringList failedLibraries;
    
    for(QStringList::const_iterator iter = libraries.begin();
        iter != libraries.end();
        ++iter)
    {
        void* libHandle;
        void (*registrationFunction)(stromx::core::Registry& registry);
        char* error;
        
        libHandle = dlopen((*iter).toStdString().c_str(), RTLD_LAZY);
        
        if (!libHandle)
        {
            failedLibraries.append(*iter);
            continue;
        }
        
        QRegExp regEx("libstromx_(\\c?).so\\c*");
        if(regEx.indexIn(*iter) < 0)
        {
            failedLibraries.append(*iter);
            dlclose(libHandle);
            continue;
        }
        
        QString libName("register" + regEx.cap(1));

        registrationFunction = reinterpret_cast<void (*)(stromx::core::Registry& registry)>
            (dlsym(libHandle, libName.toStdString().c_str()));
            
        if ((error = dlerror()) != NULL) 
        {
            failedLibraries.append(*iter);
            dlclose(libHandle);
            continue;
        } 
        
        (*registrationFunction)(*m_factory);

        dlclose(libHandle);
        
        if(failedLibraries.size())
            throw LoadLibrariesFailed(failedLibraries);
    }
}

void OperatorLibraryModel::updateOperators()
{
    m_operators.clear();
    
    typedef std::vector<const stromx::core::OperatorKernel*> OperatorKernelList;
    for(OperatorKernelList::const_iterator iter = m_factory->availableOperators().begin();
        iter != m_factory->availableOperators().end();
        ++iter)
    {
        QString package = QString::fromStdString((*iter)->package());
        QString type = QString::fromStdString((*iter)->type());
        
        m_operators[package].append(type);
    }
}




