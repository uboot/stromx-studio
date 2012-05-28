#include "OperatorModel.h"

#include <QEvent>
#include <QUndoStack>
#include <stromx/core/Operator.h>
#include <stromx/core/Parameter.h>
#include <stromx/core/Trigger.h>
#include "Common.h"
#include "ConnectorObserver.h"
#include "ConnectorDataEvent.h"
#include "ConnectorOccupyEvent.h"
#include "DataConverter.h"
#include "MoveOperatorCmd.h"
#include "RenameOperatorCmd.h"
#include "StreamModel.h"
#include "SetParameterCmd.h"
#include "Image.h"


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
    
    const stromx::core::Parameter* param = reinterpret_cast<stromx::core::Parameter*>(index.internalPointer());
  
    switch(index.row())
    {
        case TYPE:
        case STATUS:
            break;
        
        case NAME:
            return flags | Qt::ItemIsEditable;
            
        default:
            if(parameterIsWriteAccessible(param))
                return flags | Qt::ItemIsEditable;
    }    
    return flags;
}

QModelIndex OperatorModel::index(int row, int column, const QModelIndex& parent) const
{
    if(row >= PARAMETER_OFFSET)
        return createIndex(row, column, (void*)(parameterAtRow(row)));
    else
        return createIndex(row, column);
}

QVariant OperatorModel::data(const QModelIndex& index, int role) const
{
    // return if any of the conditions below is not satisfied
    if(!index.isValid() || index.column() > 1 || m_op == 0)
        return QVariant();
    
    // possible rows
    enum
    {
        TYPE_ROW, // 0
        STATUS_ROW, // 1
        NAME_ROW, // 2
        PARAMETER_ROW // 3
    };
    
    // possible columns
    enum
    {
        DESCRIPTION,
        VALUE
    };
    
    // possible roles
    enum
    {
        DISPLAY, // 0
        EDIT, // 1
        USER, // 2
        OTHER // 3
    };
    
    // possible return values
    enum ReturnValue
    {
        NOTHING, // 0
        STRING_TYPE, // 1
        TYPE, // 2
        STRING_STATUS, // 3
        STATUS, // 4
        STRING_NAME, // 5
        NAME, // 6
        PARAMETER_NAME, // 7
        PARAMETER_VALUE // 8
    };
    
    // the decision table
    int table[4][2][5] =
    /* Row          0  0  0  0    0  0  0  0      1  1  1  1    1  1  1  1 
     * Column       0  0  0  0    1  1  1  1      0  0  0  0    1  1  1  1
     * Role         0  1  2  3    0  1  2  3      0  1  2  3    0  1  2  3 */
    /* Action */ {{{1, 0, 0, 0}, {2, 0, 0, 0}}, {{3, 0, 0, 0}, {4, 0, 0, 0}},
                 
    /* Row          2  2  2  2    2  2  2  2      3  3  3  3    3  3  3  3 
     * Column       0  0  0  0    1  1  1  1      0  0  0  0    1  1  1  1
     * Role         0  1  2  3    0  1  2  3      0  1  2  3    0  1  2  3 */
    /* Action */  {{5, 0, 0, 0}, {6, 6, 0, 0}}, {{7, 0, 0, 0}, {8, 8, 8, 0}}};
     
    // extract row, column and role type
    int row = index.row() > 2 ? PARAMETER_ROW : index.row();
    int column = index.column() == 0 ? 0 : 1;
    int roleType = 0;
    switch(role)
    {
    case Qt::DisplayRole:
        roleType = DISPLAY;
        break;
    case Qt::EditRole:
        roleType = EDIT;
        break;
    case ChoicesRole:
    case TriggerRole:
    case ImageRole:
        roleType = USER;
        break;
    default:
        roleType = OTHER;
        break;
    }
    
    // ask the decision table
    ReturnValue action = ReturnValue(table[row][column][roleType]);
    
    // get a pointer to the parameter (is 0 if this index does not point to a parameter)
    const stromx::core::Parameter* param = reinterpret_cast<stromx::core::Parameter*>(index.internalPointer());
        
    // act accordingly
    switch(action)
    {
    case STRING_TYPE:
        return tr("Type");
    case TYPE:
        return QString::fromStdString(m_op->info().type());
    case STRING_STATUS:
        return tr("Status");
    case STATUS:
        return statusToString(m_op->status());
    case STRING_NAME:
        return tr("Name");
    case NAME:
        return QString::fromStdString(m_op->name());
    case PARAMETER_NAME:
    {
        return QVariant(QString::fromStdString(param->name()));
    }
    case PARAMETER_VALUE:
        if(parameterIsReadAccessible(param))
        {
            unsigned int paramId = param->id();
            try
            {
                return DataConverter::toQVariant(m_op->getParameter(paramId), *param, role);
            }
            catch(stromx::core::Exception&)
            {
                QVariant();
            }
        }
        else
        {
            return QVariant();
        }
    case NOTHING:
    default:
        return QVariant();
    }
}

bool OperatorModel::setData(const QModelIndex& index, const QVariant& value, int role)
{   
    if(role != Qt::EditRole)
        return false;
    
    if(!index.isValid() || index.column() == 0)
        return false;
    
    const stromx::core::Parameter* param = reinterpret_cast<stromx::core::Parameter*>(index.internalPointer());

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
            if(parameterIsWriteAccessible(param))
            {
                unsigned int paramId = param->id();
                try
                {
                    std::auto_ptr<stromx::core::Data> stromxData;
                    stromxData = DataConverter::toStromxData(value, *param);
                    
                    if(stromxData.get() == 0)
                        return false;
                    
                    // test if this data is trigger data
                    stromx::core::Trigger* trigger =
                        stromx::core::data_cast<stromx::core::Trigger*>(stromxData.get());
                        
                    if(trigger)
                    {
                        // triggers are set without informing the undo stack (they can not 
                        // be undone)
                        doSetParameter(paramId, *stromxData);
                    }
                    else
                    {
                        // any other parameters are set via an undo stack command
                        SetParameterCmd* cmd = new SetParameterCmd(this, paramId,*stromxData);
                        m_stream->undoStack()->push(cmd);
                    }
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

void OperatorModel::doSetParameter(unsigned int paramId, const stromx::core::Data& newValue)
{
    try
    {
        m_op->setParameter(paramId, newValue);   
    } 
    catch(stromx::core::Exception&)
    {
    }
    
    unsigned int index = 0;
    
    //Translate paramId to index of the row in table which is assigned to the parameter
    for(std::vector<const stromx::core::Parameter*>::const_iterator iter_param = m_op->info().parameters().begin();
        iter_param != m_op->info().parameters().end();
        ++iter_param)
    {   
        if((*iter_param)->id() == paramId)
        {
            emit dataChanged(createIndex(index + PARAMETER_OFFSET,1), createIndex(index + PARAMETER_OFFSET,1));
            break;
        }
        ++index;    
    }     
}

void OperatorModel::doSetPos(const QPointF& pos)
{
    m_pos = pos;
    emit posChanged(m_pos);
}

const stromx::core::Parameter* OperatorModel::parameterAtRow(int row) const
{
    Q_ASSERT(row - PARAMETER_OFFSET >= 0);
    
    const std::vector<const stromx::core::Parameter*> & parameters = m_op->info().parameters();
    int currentRow = PARAMETER_OFFSET;
    
    for(std::vector<const stromx::core::Parameter*>::const_iterator iter = parameters.begin();
        iter != parameters.end();
        ++iter)
    {
        if(parameterIsReadAccessible(*iter))
        {
            if(currentRow == row)
                return *iter;
            
            currentRow++;
        }
    }
    
    Q_ASSERT(false);
}

int OperatorModel::accessibleParametersCount() const
{
    int count = 0;
    const std::vector<const stromx::core::Parameter*> & parameters = m_op->info().parameters();
    
    for(std::vector<const stromx::core::Parameter*>::const_iterator iter = parameters.begin();
        iter != parameters.end();
        ++iter)
    {
        if(parameterIsReadAccessible(*iter))
            count++;
    }
    return count;    
}

bool OperatorModel::parameterIsReadAccessible(const stromx::core::Parameter* par) const
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

bool OperatorModel::parameterIsWriteAccessible(const stromx::core::Parameter* par) const
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
    reset();
    emit activeChanged(false);
}

void OperatorModel::setActiveTrue()
{
    reset();
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

QString OperatorModel::statusToString(int status)
{
    switch(status)
    {
    case stromx::core::Operator::NONE:
        return tr("None");
    case stromx::core::Operator::INITIALIZED:
        return tr("Initialized");
    case stromx::core::Operator::ACTIVE:
    case stromx::core::Operator::EXECUTING:
        return tr("Active");
    default:
        Q_ASSERT(false);
    }
    
    return QString();
}

QDataStream& operator<<(QDataStream& stream, const OperatorModel* op)
{
    return stream << op->m_pos;
}

QDataStream& operator>>(QDataStream& stream, OperatorModel* op)
{
    return stream >> op->m_pos;
}

