#include "OperatorModel.h"

#include <QEvent>
#include <QUndoStack>
#include <stromx/core/Operator.h>
#include <stromx/core/Parameter.h>
#include "Common.h"
#include "DataConverter.h"
#include "RenameOperatorCmd.h"
#include "MoveOperatorCmd.h"
#include "ConnectorObserver.h"
#include "ConnectorDataEvent.h"
#include "ConnectorOccupyEvent.h"
#include "StreamModel.h"

OperatorModel::OperatorModel(stromx::core::Operator* op, StreamModel* stream)
  : QAbstractTableModel(stream),
    m_op(op),
    m_stream(stream),
    m_package(QString::fromStdString(m_op->info().package())),
    m_type(QString::fromStdString(m_op->info().type())),
    m_name(QString::fromStdString(m_op->name())),
    m_observer(new ConnectorObserver(this))
{
    Q_ASSERT(m_op);
    
    m_op->addObserver(m_observer);
    connect(m_stream, SIGNAL(streamStarted()), this, SLOT(setActiveTrue()));
    connect(m_stream, SIGNAL(streamStopped()), this, SLOT(setActiveFalse()));
}

OperatorModel::~OperatorModel()
{
    m_op->removeObserver(m_observer);
    delete m_observer;
}

int OperatorModel::rowCount(const QModelIndex& index) const
{
    return accessibleParametersCount() + PARAMETER_OFFSET;
}

int OperatorModel::columnCount(const QModelIndex& index) const
{
    return 2;
}

QVariant OperatorModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if(orientation == Qt::Horizontal && role == Qt::DisplayRole)
    {
        if(section == 0)
            return tr("Property");
        else
            return tr("Value");
    }
    
    return QVariant();
}

Qt::ItemFlags OperatorModel::flags(const QModelIndex& index) const
{   
    Qt::ItemFlags flags = QAbstractItemModel::flags(index);
    if(index.column() == 0 || m_op == 0 || !index.isValid())
        return flags;
    
    switch(index.row())
    {
        case TYPE:
        case STATUS:
            break;
        
        case NAME:
            return flags | Qt::ItemIsEditable;
            
        default:
        {
            if(parameterIsWriteAccessible(m_op->info().parameters()[index.row()-PARAMETER_OFFSET]))
                return flags | Qt::ItemIsEditable;
        }
    }    
    return flags;
}

QVariant OperatorModel::data(const QModelIndex& index, int role) const
{
    if(role != Qt::DisplayRole && role != Qt::EditRole && role != ChoicesRole)
        return QVariant();

    if(!index.isValid() || m_op == 0)
        return QVariant();
    
    switch(index.row())
    {
        case TYPE:
        {
            if(index.column() == 0)
                return tr("Type");
            else
                return QVariant(QString::fromStdString(m_op->info().type()));
        }
        
        case STATUS:
        {
            if(index.column() == 0)
            {
                return tr("Status");
            }
            else
            {
                switch(m_op->status())
                {
                case stromx::core::Operator::NONE:
                    return QVariant(tr("None"));
                default:
                    return QVariant(tr("Initialized"));
                }
            }
        }
        
        case NAME:
        {
            if(index.column() == 0)
                return tr("Name");
            else
                return QVariant(QString::fromStdString(m_op->name()));
        }
    
    
        default:
        {
            if(index.column() == 0)
                return QVariant(QString::fromStdString(m_op->info().parameters()[index.row()-PARAMETER_OFFSET]->name()));
            
            if(parameterIsReadAccessible(m_op->info().parameters()[index.row()-PARAMETER_OFFSET]))
            {
                const stromx::core::Parameter* param = m_op->info().parameters()[index.row()-PARAMETER_OFFSET];
                unsigned int paramId = param->id();
                try
                {
                    return DataConverter::toQVariant(m_op->getParameter(paramId), *param, role);
                }
                catch(stromx::core::Exception&)
                {
                    return QVariant(tr("<X:not accessible>"));
                }
            }
            else
            {
                return QVariant(tr("<not accessible>"));
            }
        }
    }
    
    return QVariant();
}

bool OperatorModel::setData(const QModelIndex& index, const QVariant& value, int role)
{   
    if(role != Qt::EditRole)
        return false;
    
    if(!index.isValid() || index.column() == 0)
        return false;
    
    switch(index.row())
    {
        case NAME:
        { 
            QString newName = value.toString();
            if(newName.isEmpty())
                return false;
        
            setName(newName);
            return true;
        }

        case TYPE:
        case STATUS:
            return false;
            
        default:
        {
            if(parameterIsWriteAccessible(m_op->info().parameters()[index.row()-PARAMETER_OFFSET]))
            {
                const stromx::core::Parameter* param = m_op->info().parameters()[index.row()-PARAMETER_OFFSET];
                unsigned int paramId = param->id();
                try
                {
                    std::auto_ptr<stromx::core::Data> stromxData;
                    stromxData = DataConverter::toStromxData(value, *param);
                    
                    if(stromxData.get() == 0)
                        return false;
                    
                    m_op->setParameter(paramId, *stromxData);
                    return true;
                }
                catch(stromx::core::Exception&)
                {
                    return false;
                }
            }
            else
            {
                return false;
            }
        }
    }
}

void OperatorModel::addConnection(ConnectionModel* connection)
{
    m_connections.insert(connection);
}

void OperatorModel::removeConnection(ConnectionModel* connection)
{
    m_connections.remove(connection);
}

void OperatorModel::setPos(const QPointF& pos)
{
    if(m_pos != pos)
    {
        MoveOperatorCmd* cmd = new MoveOperatorCmd(this, pos);
        m_stream->undoStack()->push(cmd);
    }
}

void OperatorModel::setName(const QString& name)
{
    if(m_name != name)
    {
        RenameOperatorCmd* cmd = new RenameOperatorCmd(this, name);
        m_stream->undoStack()->push(cmd);
    }
}

void OperatorModel::doSetName(const QString& name)
{
    m_name = name;
    m_op->setName(name.toStdString());
    emit nameChanged(m_name);
    emit dataChanged(createIndex(NAME, 1), createIndex(NAME, 1));
}

void OperatorModel::doSetPos(const QPointF& pos)
{
    m_pos = pos;
    emit posChanged(m_pos);
}

int OperatorModel::accessibleParametersCount() const
{
    return m_op->info().parameters().size();    
}

bool OperatorModel::parameterIsReadAccessible(const stromx::core::Parameter* const par) const
{
    if(m_op->status() != stromx::core::Operator::NONE)
    {
        if (par->accessMode() == stromx::core::Parameter::NO_ACCESS)
            return false;
        else
            return true;
    }
    else
    {
        if(par->accessMode() == stromx::core::Parameter::NONE_READ || par->accessMode() == stromx::core::Parameter::NONE_WRITE)
            return true;
        else
            return false;
    }
}

bool OperatorModel::parameterIsWriteAccessible(const stromx::core::Parameter*const par) const
{
    switch(m_op->status())
    {
        case stromx::core::Operator::NONE:
            if(par->accessMode() == stromx::core::Parameter::NONE_WRITE)
                return true;
            else
                return false;
            
        case stromx::core::Operator::INITIALIZED:
            if(par->accessMode() == stromx::core::Parameter::INITIALIZED_WRITE || par->accessMode() == stromx::core::Parameter::ACTIVATED_WRITE)
                return true;
            else
                return false;
            
        default:
            if(par->accessMode() == stromx::core::Parameter::ACTIVATED_WRITE)
                return true;
            else
                return false;
    }
}


bool OperatorModel::isInitialized() const
{
    return m_op->status() != stromx::core::Operator::NONE;
}

bool OperatorModel::isActive() const
{
    return m_stream->isActive();
}

void OperatorModel::setInitialized(bool status)
{
    if(isInitialized() == status)
        return;
   
    beginResetModel();
    
    if(status == true)
        m_op->initialize();
    else
        m_op->deinitialize();
        
    endResetModel();
    
    emit initializedChanged(isInitialized());
}

const QString& OperatorModel::type() const
{
    return m_type;
}

const QString& OperatorModel::package() const
{
    return m_package;
}

QUndoStack* OperatorModel::undoStack() const
{
    return m_stream->undoStack(); 
}

void OperatorModel::customEvent(QEvent* event)
{
    if(ConnectorOccupyEvent* occupyEvent = dynamic_cast<ConnectorOccupyEvent*>(event))
        emit connectorOccupiedChanged(occupyEvent->type(), occupyEvent->id(), occupyEvent->occupied());
    else if(ConnectorDataEvent* dataEvent = dynamic_cast<ConnectorDataEvent*>(event))
        emit connectorDataChanged(dataEvent->type(), dataEvent->id(), dataEvent->data());
}

void OperatorModel::setActiveFalse()
{
    emit activeChanged(false);
}

void OperatorModel::setActiveTrue()
{
    emit activeChanged(true);
}

void OperatorModel::connectNotify(const char* signal)
{
    // if there are receivers for data change signals the according events must be sent
    if(receivers(SIGNAL(connectorDataChanged(OperatorModel::ConnectorType,uint,stromx::core::DataContainer))))
        m_observer->setObserveData(true);  
}

void OperatorModel::disconnectNotify(const char* signal)
{
    // if there are no receivers for data change signals do not send the events
    if(! receivers(SIGNAL(connectorDataChanged(OperatorModel::ConnectorType,uint,stromx::core::DataContainer))))
        m_observer->setObserveData(false); 
}

QDataStream& operator<<(QDataStream& stream, const OperatorModel* op)
{
    return stream << op->m_pos;
}

QDataStream& operator>>(QDataStream& stream, OperatorModel* op)
{
    return stream >> op->m_pos;
}

