#include "model/OperatorLibraryModel.h"

#include <QCoreApplication>
#include <QDebug>
#include <QDirIterator>
#include <QFileInfo>
#include <QLibrary>
#include <QSettings>
#include <stromx/runtime/Runtime.h>
#include <stromx/runtime/Factory.h>
#include <stromx/runtime/Operator.h>
#include <stromx/runtime/OperatorKernel.h>
#include <iostream>
#include "Config.h"
#include "Exception.h"
#include "data/OperatorData.h"

using namespace stromx::runtime;

OperatorLibraryModel::Item::~Item()
{
    foreach (Item* item, children)
        delete item;
    children.clear();
}
    
OperatorLibraryModel::OperatorLibraryModel(QObject* parent)
  : QAbstractItemModel(parent),
    m_root(new Item),
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
            //loadPackage(file);
        }
        catch(LoadPackageFailed & e)
        {
            qWarning() << e.what();
        }
    }
    
}

OperatorLibraryModel::~OperatorLibraryModel()
{
    delete m_root;
    delete m_factory;
}


QModelIndex OperatorLibraryModel::index(int row, int column, const QModelIndex& parent) const
{
    // no parent
    if(! parent.isValid())
        return createIndex(row, column, m_root->children[row]);
    
    // parent is a package
    Item* parentItem = static_cast<Item*>(parent.internalPointer());
    return createIndex(row, column, parentItem->children[row]);
}

QModelIndex OperatorLibraryModel::parent(const QModelIndex& child) const
{
    Item* item = static_cast<Item*>(child.internalPointer());
    
    // child is a top-level package
    if (item->parent == 0)
        return QModelIndex();
    
    // parent is a child of root
    Item* parentItem = item->parent;
    int parentRow = 0;
    if (parentItem->parent == 0)
    {
        parentRow = m_root->children.indexOf(parentItem);
    }
    else
    {
        Item* parentOfParent = parentItem->parent;
        parentRow = parentOfParent->children.indexOf(parentItem);
    }
        
    return createIndex(parentRow, 0, parentItem);
}

QVariant OperatorLibraryModel::data(const QModelIndex& index, int role) const
{
    if(role != Qt::DisplayRole)
        return QVariant();
        
    Item* item = static_cast<Item*>(index.internalPointer());
    
    // index is a package
    if (item->op == 0)
    {
        if(index.column() == 0)
            return item->package;
        else
            return QVariant();
    }
    
    // index is an operator
    if(index.column() == 0)
    {
        return QString::fromStdString(item->op->type());
    }
    else
    {
        QString version = QString("%1.%2.%3")
                          .arg(item->op->version().major())
                          .arg(item->op->version().minor())
                          .arg(item->op->version().revision());
        return version;
    }
}

int OperatorLibraryModel::columnCount(const QModelIndex& /*parent*/) const
{
    return NUM_COLUMNS;
}

int OperatorLibraryModel::rowCount(const QModelIndex& parent) const
{
    if (! parent.isValid())
        return m_root->children.count();
    
    Item* item = static_cast<Item*>(parent.internalPointer());
    return item->children.count();
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
    QString registrationFunctionName = prefix + postfix + "Register";
    
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
    stromxRuntimeRegister(*m_factory);
}

void OperatorLibraryModel::updateOperators()
{
    beginResetModel();
    
    delete m_root;
    m_root = new Item;
    
    QMap<QString, Item*> package2ItemMap;
    
    typedef std::vector<const stromx::runtime::OperatorKernel*> OperatorKernelList;
    for(OperatorKernelList::const_iterator iter = m_factory->availableOperators().begin();
        iter != m_factory->availableOperators().end();
        ++iter)
    {
        QString package = QString::fromStdString((*iter)->package());
        
        QStringList parts = package.split("::");
        Item* root = m_root;
        foreach (QString part, parts)
        {
            // find the item
            Item* partItem = 0;
            foreach (Item* item, root->children)
            {
                if (item->package == part)
                {
                    partItem = item;
                    break;
                }
            }
            
            if (partItem == 0)
            {
                partItem = new Item;
                partItem->package = part;
                partItem->parent = root;
                root->children.append(partItem);
            }
            
            root = partItem;
        }
        
        Item* opItem = new Item;
        opItem->op = *iter;
        opItem->parent = root;
        root->children.append(opItem);
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
    Item* item = static_cast<Item*>(index.internalPointer());
    return item->op != 0;
}

OperatorData* OperatorLibraryModel::newOperatorData(const QModelIndex& index) const
{
    Item* item = static_cast<Item*>(index.internalPointer());
    
    if (! item->op)
        return 0;
    
    const stromx::runtime::OperatorKernel* op = item->op;
    return new OperatorData(QString::fromStdString(op->package()), QString::fromStdString(op->type()));
}

stromx::runtime::OperatorKernel* OperatorLibraryModel::newOperator(const OperatorData* data) const
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

QFileInfoList OperatorLibraryModel::findInstalledPackages()
{
    QStringList packages;
    
#ifdef UNIX
    QDir dir(STROMX_PACKAGE_DIR);
#endif // UNIX
    
#ifdef WIN32
    QDir dir(QCoreApplication::applicationDirPath());
#endif // WIN32
    
    QStringList filters;
#ifdef UNIX
    filters << "libstromx_*.so";
#endif // UNIX
    
#ifdef WIN32
    filters << "stromx_*.dll";
#endif // WIN32
    
    return dir.entryInfoList(filters);
}






