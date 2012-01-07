#include "OperatorLibraryModel.h"

#include "Exception.h"
#include <QFileInfo>
#include <QSettings>
#include <stromx/core/Core.h>
#include <stromx/core/Factory.h>
#include <stromx/core/OperatorKernel.h>
#include <dlfcn.h>
#include <iostream>
#include <stromx/core/Operator.h>


using namespace stromx::core;
    
OperatorLibraryModel::OperatorLibraryModel(QObject* parent)
  : QAbstractItemModel(parent),
    m_factory(0)
{
    m_factory = new stromx::core::Factory();
    stromxRegisterCore(*m_factory);
    
    
    QSettings settings("stromx", "stromx-studio");
    QStringList loadedLibraries = settings.value("loadedLibraries").toStringList();
    
    QString file;
    foreach(file, loadedLibraries)
    {
        try
        {
            loadLibrary(file);
        }
        catch(LoadLibraryFailed&)
        {
        }
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
    if(id < m_package2OperatorMap.size())
        return createIndex(row, column, m_package2OperatorMap.size() + id);
}

QModelIndex OperatorLibraryModel::parent(const QModelIndex& child) const
{
    unsigned int id = child.internalId();
    
    // child is a package
    if(id < m_package2OperatorMap.size())
        return QModelIndex();
    
    // child is an operator
    unsigned int packageId = id - m_package2OperatorMap.size();
    return createIndex(packageId, 0, packageId);
}

QVariant OperatorLibraryModel::data(const QModelIndex& index, int role) const
{
    if(role != Qt::DisplayRole)
        return QVariant();
        
    unsigned int id = index.internalId();
    
    // index is a package
    if(id < m_package2OperatorMap.size())
    {
        if(index.column() == 0)
            return m_index2PackageMap[id];
        else
            return QVariant();
    }
    
    // index is an operator
    unsigned int packageId = id - m_package2OperatorMap.size();
    QString package = m_index2PackageMap.value(packageId, "");
    QList<const stromx::core::OperatorKernel*> ops = m_package2OperatorMap.value(package, QList<const stromx::core::OperatorKernel*>());
    const stromx::core::OperatorKernel* op = ops[index.row()];
        
    if(index.column() == 0)
    {
        return QString::fromStdString(op->type());
    }
    else
    {
        QString version = QString("%1.%2.%3")
                          .arg(op->version().major())
                          .arg(op->version().minor())
                          .arg(op->version().patch());
        return version;
    }
}

int OperatorLibraryModel::columnCount(const QModelIndex& parent) const
{
    return 2;
}

int OperatorLibraryModel::rowCount(const QModelIndex& parent) const
{
    // parent is invalid
    if(! parent.isValid())
        return m_package2OperatorMap.size();
    
    unsigned int id = parent.internalId();
    
    // parent is a package
    if(id < m_package2OperatorMap.size())
    {
        QString package = m_index2PackageMap[id];
        return m_package2OperatorMap[package].size();
    }
    
    // parent is an operator type
    return 0;
}

void OperatorLibraryModel::loadLibrary(const QString& library)
{
    QFileInfo info(library);
    
    QRegExp regEx("libstromx_(.+)");
    if(regEx.indexIn(info.baseName()) == -1)
        throw LoadLibraryFailed();
    
    QString registrationFunctionName = regEx.cap(1);
    registrationFunctionName[0] = registrationFunctionName[0].toUpper();
    registrationFunctionName.prepend("stromxRegister");
    
    void* libHandle;
    void (*registrationFunction)(stromx::core::Registry& registry);
    char* error;
    
    libHandle = dlopen(library.toStdString().c_str(), RTLD_LAZY);
    
    if (!libHandle)
        throw LoadLibraryFailed();

    registrationFunction = reinterpret_cast<void (*)(stromx::core::Registry& registry)>
        (dlsym(libHandle, registrationFunctionName.toStdString().c_str()));
        
    if ((error = dlerror()) != NULL) 
    {
        dlclose(libHandle);
        throw LoadLibraryFailed();
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
        // even if an exception was thrown, parts of the library might have been loaded
        // therefore the library is not closed
        throw LoadLibraryFailed();
    }
    
    // remember the library
    m_loadedLibraries.append(library);
        
    updateOperators();
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
    
    m_package2OperatorMap.clear();
    m_index2PackageMap.clear();
    
    unsigned int i = 0;
    typedef std::vector<const stromx::core::OperatorKernel*> OperatorKernelList;
    for(OperatorKernelList::const_iterator iter = m_factory->availableOperators().begin();
        iter != m_factory->availableOperators().end();
        ++iter, ++i)
    {
        QString package = QString::fromStdString((*iter)->package());
        
        m_package2OperatorMap[package].append(*iter);
        m_index2PackageMap[i] = package;
    }
    
    endResetModel();
}

QVariant OperatorLibraryModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if(orientation == Qt::Horizontal && role == Qt::DisplayRole)
    {
        if(section == 0)
            return tr("Operator");
        else
            return tr("Version");
    }
    
    return QVariant();
}

const bool OperatorLibraryModel::isOperator(const QModelIndex& index) const
{
    unsigned int id = index.internalId();
    
    return (id >= m_package2OperatorMap.size());
    
}

stromx::core::Operator* OperatorLibraryModel::newOperator(const QModelIndex& index) const
{
    if(! isOperator(index))
        return 0;
    
    unsigned int id = index.internalId();
    unsigned int packageId = id - m_package2OperatorMap.size();
    QString package = m_index2PackageMap.value(packageId, "");
    QList<const stromx::core::OperatorKernel*> ops = m_package2OperatorMap.value(package, QList<const stromx::core::OperatorKernel*>());
    const stromx::core::OperatorKernel* op = ops[index.row()];
    
    return m_factory->newOperator(op->package(), op->type());
}




