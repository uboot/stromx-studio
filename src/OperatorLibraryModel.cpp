#include "OperatorLibraryModel.h"

#include "OperatorData.h"
#include <QFileInfo>
#include <QSettings>
#include <stromx/core/Core.h>
#include <stromx/core/Factory.h>
#include <stromx/core/Operator.h>
#include <stromx/core/OperatorKernel.h>
#include <iostream>

#ifdef UNIX
    #include <dlfcn.h>
#endif // UNIX

#ifdef WIN32
#endif // WIN32

using namespace stromx::core;
    
OperatorLibraryModel::OperatorLibraryModel(QObject* parent)
  : QAbstractItemModel(parent),
    m_factory(0)
{
    m_factory = new stromx::core::Factory();
    stromxRegisterCore(*m_factory);
    
    updateOperators();
    
    QSettings settings("stromx", "stromx-studio");
    QStringList loadedLibraries = settings.value("loadedLibraries").toStringList();
    
    foreach(QString file, loadedLibraries)
    {
        // try to load the library and ignore any failures
        loadLibrary(file);
    }
}

OperatorLibraryModel::~OperatorLibraryModel()
{
    delete m_factory;
    
#ifdef UNIX
    foreach(void* handle, m_libraryHandles)
        dlclose(handle);
#endif // UNIX
    
    QSettings settings("stromx", "stromx-studio");
    settings.setValue("loadedLibraries", m_loadedLibraries);
}


QModelIndex OperatorLibraryModel::index(int row, int column, const QModelIndex& parent) const
{
    // no parent
    if(! parent.isValid())
        return createIndex(row, column, row);
    
    int id = parent.internalId();
    
    // parent is a package
    if(id < m_package2OperatorMap.size())
        return createIndex(row, column, m_package2OperatorMap.size() + id);
    
    return QModelIndex();
}

QModelIndex OperatorLibraryModel::parent(const QModelIndex& child) const
{
    int id = child.internalId();
    
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
        
    int id = index.internalId();
    
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
    
    int id = parent.internalId();
    
    // parent is a package
    if(id < m_package2OperatorMap.size())
    {
        QString package = m_index2PackageMap[id];
        return m_package2OperatorMap[package].size();
    }
    
    // parent is an operator type
    return 0;
}

bool OperatorLibraryModel::loadLibrary(const QString& library)
{
#ifdef UNIX
    QFileInfo info(library);
    
    QRegExp regEx("libstromx_(.+)");
    if(regEx.indexIn(info.baseName()) == -1)
        return false;
    
    QString registrationFunctionName = regEx.cap(1);
    registrationFunctionName[0] = registrationFunctionName[0].toUpper();
    registrationFunctionName.prepend("stromxRegister");
    
    void* libHandle;
    void (*registrationFunction)(stromx::core::Registry& registry);
    char* error;
    
    libHandle = dlopen(library.toStdString().c_str(), RTLD_LAZY);
    
    if (!libHandle)
        return false;

    registrationFunction = reinterpret_cast<void (*)(stromx::core::Registry& registry)>
        (dlsym(libHandle, registrationFunctionName.toStdString().c_str()));
        
    if ((error = dlerror()) != NULL) 
    {
        dlclose(libHandle);
        return false;
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
        return false;
    }
    
    // remember the library
    m_loadedLibraries.append(library);
        
    updateOperators();
#endif // UNIX
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
    int id = index.internalId();
    
    return (id >= m_package2OperatorMap.size());
    
}

OperatorData* OperatorLibraryModel::newOperatorData(const QModelIndex& index) const
{
    if(! isOperator(index))
        return 0;
    
    unsigned int id = index.internalId();
    unsigned int packageId = id - m_package2OperatorMap.size();
    QString package = m_index2PackageMap.value(packageId, "");
    QList<const stromx::core::OperatorKernel*> ops = m_package2OperatorMap.value(package, QList<const stromx::core::OperatorKernel*>());
    const stromx::core::OperatorKernel* op = ops[index.row()];
    
    return new OperatorData(QString::fromStdString(op->package()), QString::fromStdString(op->type()));
}

stromx::core::Operator* OperatorLibraryModel::newOperator(const OperatorData* data) const
{
    try
    {
        return m_factory->newOperator(data->package().toStdString(), data->type().toStdString());
    }
    catch(stromx::core::WrongArgument &)
    {
        return 0;
    }
}





