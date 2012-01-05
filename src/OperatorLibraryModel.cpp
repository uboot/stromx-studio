#include "OperatorLibraryModel.h"

#include "Exception.h"
#include <QFileInfo>
#include <QSettings>
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
    
    
    QSettings settings("stromx", "stromx-studio");
    QStringList loadedLibraries = settings.value("loadedLibraries").toStringList();
    
    try
    {
        loadLibraries(loadedLibraries);
    }
    catch(LoadLibrariesFailed&)
    {
    }
}

OperatorLibraryModel::~OperatorLibraryModel()
{
    delete m_factory;
    
    void* handle = 0;
    foreach(handle, m_libraryHandles)
        dlclose(handle);
    
    QSettings settings("stromx", "stromx-studio");
    settings.setValue("loadedLibraries", m_loadedLibraries);
}


QModelIndex OperatorLibraryModel::index(int row, int column, const QModelIndex& parent) const
{
    // no parent
    if(! parent.isValid())
        return createIndex(row, column, row);
    
    unsigned int id = parent.internalId();
    
    // parent is a package
    if(id < m_package2TypeMap.size())
        return createIndex(row, column, m_package2TypeMap.size() + id);
}

QModelIndex OperatorLibraryModel::parent(const QModelIndex& child) const
{
    unsigned int id = child.internalId();
    
    // child is a package
    if(id < m_package2TypeMap.size())
        return QModelIndex();
    
    // child is an operator
    unsigned int packageId = id - m_package2TypeMap.size();
    return createIndex(packageId, 0, packageId);
}

QVariant OperatorLibraryModel::data(const QModelIndex& index, int role) const
{
    if(role != Qt::DisplayRole)
        return QVariant();
        
    unsigned int id = index.internalId();
    
    // index is a package
    if(id < m_package2TypeMap.size())
        return m_index2PackageMap[id];
    
    // index is an operator
    unsigned int packageId = id - m_package2TypeMap.size();
    QString package = m_index2PackageMap.value(packageId, "");
    QStringList ops = m_package2TypeMap.value(package, QStringList());
    return ops[index.row()];
}

int OperatorLibraryModel::columnCount(const QModelIndex& parent) const
{
    return 1;
}

int OperatorLibraryModel::rowCount(const QModelIndex& parent) const
{
    // parent is invalid
    if(! parent.isValid())
        return m_package2TypeMap.size();
    
    unsigned int id = parent.internalId();
    
    // parent is a package
    if(id < m_package2TypeMap.size())
    {
        QString package = m_index2PackageMap[id];
        return m_package2TypeMap[package].size();
    }
    
    // parent is an operator type
    return 0;
}

void OperatorLibraryModel::loadLibraries(const QStringList& libraries)
{
    QStringList failedLibraries;
    
    for(QStringList::const_iterator iter = libraries.begin();
        iter != libraries.end();
        ++iter)
    {
        QFileInfo info(*iter);
        
        QRegExp regEx("libstromx_(.+)");
        if(regEx.indexIn(info.baseName()) == -1)
        {
            failedLibraries.append(*iter);
            continue;
        }
        
        QString registrationFunctionName = regEx.cap(1);
        registrationFunctionName[0] = registrationFunctionName[0].toUpper();
        registrationFunctionName.prepend("stromxRegister");
        
        void* libHandle;
        void (*registrationFunction)(stromx::core::Registry& registry);
        char* error;
        
        libHandle = dlopen((*iter).toStdString().c_str(), RTLD_LAZY);
        
        if (!libHandle)
        {
            failedLibraries.append(*iter);
            continue;
        }

        registrationFunction = reinterpret_cast<void (*)(stromx::core::Registry& registry)>
            (dlsym(libHandle, registrationFunctionName.toStdString().c_str()));
            
        if ((error = dlerror()) != NULL) 
        {
            failedLibraries.append(*iter);
            dlclose(libHandle);
            continue;
        } 
        
        // store library handle to unload the library after use
        m_libraryHandles.append(libHandle);
        
        // try to register the library
        try
        {
            (*registrationFunction)(*m_factory);
        }
        catch(stromx::core::Exception&)
        {
            // even if an exception was thrown, parts of the library might 
            // have been loaded
            failedLibraries.append(*iter);
            continue;
        }
        
        // remember the library
        m_loadedLibraries.append(*iter);
    }
        
    updateOperators();
    
    if(failedLibraries.size())
        throw LoadLibrariesFailed(failedLibraries);
}

void OperatorLibraryModel::resetLibraries()
{
    delete m_factory;
    m_loadedLibraries.clear();
    
    m_factory = new stromx::core::Factory();
    stromxRegisterCore(*m_factory);
    
    updateOperators();
}

void OperatorLibraryModel::updateOperators()
{
    beginResetModel();
    
    m_package2TypeMap.clear();
    m_index2PackageMap.clear();
    
    unsigned int i = 0;
    typedef std::vector<const stromx::core::OperatorKernel*> OperatorKernelList;
    for(OperatorKernelList::const_iterator iter = m_factory->availableOperators().begin();
        iter != m_factory->availableOperators().end();
        ++iter, ++i)
    {
        QString package = QString::fromStdString((*iter)->package());
        QString type = QString::fromStdString((*iter)->type());
        
        m_package2TypeMap[package].append(type);
        m_index2PackageMap[i] = package;
    }
    
    endResetModel();
}

QVariant OperatorLibraryModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if(orientation == Qt::Horizontal && role == Qt::DisplayRole && section == 0)
        return tr("Operator");
    
    return QVariant();
}

const bool OperatorLibraryModel::isOperator(const QModelIndex& index) const
{
    unsigned int id = index.internalId();
    
    return (id >= m_package2TypeMap.size());
    
}

stromx::core::Operator* OperatorLibraryModel::newOperator(const QModelIndex& index) const
{
    if(! isOperator(index))
        return 0;
    
    unsigned int id = index.internalId();
    unsigned int packageId = id - m_package2TypeMap.size();
    QString package = m_index2PackageMap.value(packageId, "");
    QStringList ops = m_package2TypeMap.value(package, QStringList());
    QString type = ops[index.row()];
    
    return m_factory->newOperator(package.toStdString(), type.toStdString());
}




