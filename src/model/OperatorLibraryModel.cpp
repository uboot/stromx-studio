#include "model/OperatorLibraryModel.h"

#include <QFileInfo>
#include <QLibrary>
#include <QSettings>
#include <QtDebug>
#include <stromx/cvcore/Cvcore.h>
#include <stromx/cvimgproc/Cvimgproc.h>
#include <stromx/cvsupport/Cvsupport.h>
#include <stromx/example/Example.h>
#include <stromx/runtime/Runtime.h>
#include <stromx/runtime/Factory.h>
#include <stromx/runtime/Operator.h>
#include <stromx/runtime/OperatorKernel.h>
#include <iostream>
#include "Exception.h"
#include "data/OperatorData.h"

using namespace stromx::runtime;
    
OperatorLibraryModel::OperatorLibraryModel(QObject* parent)
  : QAbstractItemModel(parent),
    m_factory(0)
{
    setupFactory();
    updateOperators();
    
    QSettings settings("stromx", "stromx-studio");
    QStringList loadedPackages = settings.value("loadedPackages").toStringList();
            
    // reset the library list
    settings.setValue("loadedPackages", QStringList());
    
    foreach(QString file, loadedPackages)
    {
        // try to load the package and ignore any failures
        try
        {
            loadPackage(file);
        }
        catch(LoadPackageFailed & e)
        {
            qWarning() << e.what();
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
                          .arg(op->version().revision());
        return version;
    }
}

int OperatorLibraryModel::columnCount(const QModelIndex& /*parent*/) const
{
    return NUM_COLUMNS;
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

void OperatorLibraryModel::loadPackage(const QString& packagePath)
{
    QFileInfo info(packagePath);
    
#ifdef UNIX
    QRegExp regEx("lib(.+)_(.+)");
#endif // UNIX
    
#ifdef WIN32
    QRegExp regEx("(.+)_(.+)");
#endif // WIN32
    
    regEx.indexIn(info.baseName());
    if(regEx.captureCount() != 2)
        throw LoadPackageFailed();
    
    QString prefix = regEx.cap(1);
    QString postfix = regEx.cap(2);
    postfix[0] = postfix[0].toUpper();
    QString registrationFunctionName = prefix + "Register" + postfix;
    
    QLibrary* lib = new QLibrary(packagePath, this);
    
    // resolve the registration function
    void (*registrationFunction)(stromx::runtime::Registry& registry);
    registrationFunction = reinterpret_cast<void (*)(stromx::runtime::Registry& registry)>
        (lib->resolve(registrationFunctionName.toStdString().c_str()));
        
    if(! registrationFunction)
    {
        delete lib;
        throw LoadPackageFailed();
    }
    
    // try to register the library
    try
    {
        (*registrationFunction)(*m_factory);
    }
    catch(stromx::runtime::Exception&)
    {
        // even if an exception was thrown, parts of the library might have been loaded
        // therefore the library is not closed
        throw LoadPackageFailed();
    }
    
    // remember the package
    m_loadedPackages.append(packagePath);
    
    // save the package list
    QSettings settings("stromx", "stromx-studio");
    settings.setValue("loadedPackages", m_loadedPackages);
        
    updateOperators();
}

void OperatorLibraryModel::resetLibrary()
{
    delete m_factory;
    m_factory = 0;
    m_loadedPackages.clear();
    
    setupFactory();
    updateOperators();
    
    // reset the package list
    QSettings settings("stromx", "stromx-studio");
    settings.setValue("loadedPackages", QStringList());
}

void OperatorLibraryModel::setupFactory()
{
    Q_ASSERT(m_factory == 0);
    
    m_factory = new stromx::runtime::Factory();
    stromxRegisterRuntime(*m_factory);
    stromxRegisterExample(*m_factory);
    stromxRegisterCvcore(*m_factory);
    stromxRegisterCvimgproc(*m_factory);
    stromxRegisterCvsupport(*m_factory);
}

void OperatorLibraryModel::updateOperators()
{
    beginResetModel();
    
    m_packages.clear();
    
    QMap<QString, int> package2IdMap;
    
    typedef std::vector<const stromx::runtime::OperatorKernel*> OperatorKernelList;
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
        if(section == OPERATOR)
            return tr("Operator");
        else
            return tr("Version");
    }
    
    return QVariant();
}

bool OperatorLibraryModel::isOperator(const QModelIndex& index) const
{
    return index.parent().isValid();
}

OperatorData* OperatorLibraryModel::newOperatorData(const QModelIndex& index) const
{
    if(! isOperator(index))
        return 0;
    
    const Package* package = reinterpret_cast<const Package*>(index.internalPointer());
    const stromx::runtime::OperatorKernel* op = package->operators[index.row()];
    
    return new OperatorData(QString::fromStdString(op->package()), QString::fromStdString(op->type()));
}

stromx::runtime::Operator* OperatorLibraryModel::newOperator(const OperatorData* data) const
{
    try
    {
        return m_factory->newOperator(data->package().toStdString(), data->type().toStdString());
    }
    catch(stromx::runtime::WrongArgument &)
    {
        return 0;
    }
}





