#include "OperatorLibraryModel.h"

#include "Exception.h"
#include "OperatorData.h"
#include <QFileInfo>
#include <QLibrary>
#include <QSettings>
#include <stromx/core/Core.h>
#include <stromx/core/Factory.h>
#include <stromx/core/Operator.h>
#include <stromx/core/OperatorKernel.h>
#include <iostream>

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
        try
        {
            loadLibrary(file);
        }
        catch(LoadLibraryFailed & e)
        {
            qWarning(e.what());
        }
    }
}

OperatorLibraryModel::~OperatorLibraryModel()
{
    delete m_factory;
}


QModelIndex OperatorLibraryModel::index(int row, int column, const QModelIndex& parent) const
{
    // no parent
    if(! parent.isValid())
        return createIndex(row, column);
    
    // parent is a package
    return createIndex(row, column, (void*)(&m_packages[parent.row()]));
}

QModelIndex OperatorLibraryModel::parent(const QModelIndex& child) const
{
    // child is a package
    if(! child.internalPointer())
        return QModelIndex();
    
    // child is an operator
    const Package* package = reinterpret_cast<const Package*>(child.internalPointer());
    return createIndex(package->id, 0);
}

QVariant OperatorLibraryModel::data(const QModelIndex& index, int role) const
{
    if(role != Qt::DisplayRole)
        return QVariant();
        
    // index is a package
    if(! index.internalPointer())
    {
        if(index.column() == 0)
            return m_packages[index.row()].package;
        else
            return QVariant();
    }
    
    // index is an operator
    const Package* package = reinterpret_cast<const Package*>(index.internalPointer());
    const OperatorKernel* op = package->operators[index.row()];
       
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
        return m_packages.size();
    
    // parent is a package
    if(! parent.internalPointer())
        return m_packages[parent.row()].operators.count();
    
    // parent is an operator type
    return 0;
}

void OperatorLibraryModel::loadLibrary(const QString& libPath)
{
    QFileInfo info(libPath);
    
#ifdef UNIX
    QRegExp regEx("lib(.+)_(.+)");
#endif // UNIX
    
#ifdef WIN32
    QRegExp regEx("(.+)_(.+)");
#endif // WIN32
    
    regEx.indexIn(info.baseName());
    if(regEx.captureCount() != 2)
        throw LoadLibraryFailed();
    
    QString prefix = regEx.cap(1);
    QString postfix = regEx.cap(2);
    postfix[0] = postfix[0].toUpper();
    QString registrationFunctionName = prefix + "Register" + postfix;
    
    QLibrary* lib = new QLibrary(libPath, this);
    
    // resolve the registration function
    void (*registrationFunction)(stromx::core::Registry& registry);
    registrationFunction = reinterpret_cast<void (*)(stromx::core::Registry& registry)>
        (lib->resolve(registrationFunctionName.toStdString().c_str()));
        
    if(! registrationFunction)
    {
        delete lib;
        throw LoadLibraryFailed();
    }
    
    
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
    m_loadedLibraries.append(libPath);
    
    // save the library list
    QSettings settings("stromx", "stromx-studio");
    settings.setValue("loadedLibraries", m_loadedLibraries);
        
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
    
    m_packages.clear();
    
    QMap<QString, int> package2IdMap;
    
    typedef std::vector<const stromx::core::OperatorKernel*> OperatorKernelList;
    for(OperatorKernelList::const_iterator iter = m_factory->availableOperators().begin();
        iter != m_factory->availableOperators().end();
        ++iter)
    {
        QString package = QString::fromStdString((*iter)->package());
        
        // package has already been allocated
        if(package2IdMap.contains(package))
        {
            m_packages[package2IdMap[package]].operators.append(*iter);
        }
        else
        {
            int packageId = m_packages.count();
            package2IdMap[package] = packageId;
            m_packages.append(Package(package, packageId));
            m_packages.back().operators.append(*iter);
        }
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
    return index.parent().isValid();
}

OperatorData* OperatorLibraryModel::newOperatorData(const QModelIndex& index) const
{
    if(! isOperator(index))
        return 0;
    
    const Package* package = reinterpret_cast<const Package*>(index.internalPointer());
    const stromx::core::OperatorKernel* op = package->operators[index.row()];
    
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





