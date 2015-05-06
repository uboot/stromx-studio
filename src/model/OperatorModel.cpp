#include "model/OperatorModel.h"

#include <QEvent>
#include <QUndoStack>
#include <stromx/runtime/Operator.h>
#include <stromx/runtime/OperatorException.h>
#include <stromx/runtime/Parameter.h>
#include <stromx/runtime/TriggerData.h>
#include "Common.h"
#include "ConnectorObserver.h"
#include "DataConverter.h"
#include "ParameterServer.h"
#include "StreamModel.h"
#include "cmd/MoveOperatorCmd.h"
#include "cmd/RenameOperatorCmd.h"
#include "event/ConnectorDataEvent.h"
#include "event/ConnectorOccupyEvent.h"

const unsigned int OperatorModel::TIMEOUT = 1000;

OperatorModel::OperatorModel(stromx::runtime::Operator* op, StreamModel* stream)
  : PropertyModel(stream),
    m_op(op),
    m_stream(stream),
    m_pos(m_op->position().x(), m_op->position().y()),
    m_package(QString::fromStdString(m_op->info().package())),
    m_type(QString::fromStdString(m_op->info().type())),
    m_name(QString::fromStdString(m_op->name())),
    m_observer(this),
    m_server(new ParameterServer(op, stream->undoStack(), this))
{
    Q_ASSERT(m_op);
    
    // install the observer
    m_op->addObserver(&m_observer);
    
    // refresh the cache of the parameter server
    m_server->refresh();
    
    // propagate the current access time out of the stream
    m_server->setAccessTimeout(m_stream->accessTimeout());
    
    // Activate and deactivate when the stream stream starts/stop
    connect(m_stream, SIGNAL(streamStarted()), this, SLOT(setActiveTrue()));
    connect(m_stream, SIGNAL(streamJoined()), this, SLOT(setActiveFalse()));
    
    // forward the access time out of the stream to the parameter server
    connect(m_stream, SIGNAL(accessTimeoutChanged(int)), m_server, SLOT(setAccessTimeout(int)));
    
    // forward the parameter server signals
    connect(m_server, SIGNAL(parameterAccessTimedOut()), this, SIGNAL(operatorAccessTimedOut()));
    connect(m_server, SIGNAL(parameterErrorOccurred(ErrorData)),
            this, SIGNAL(parameterErrorOccurred(ErrorData)));
    
    // handle parameter changes by the server
    connect(m_server, SIGNAL(parameterChanged(uint)), this, SLOT(handleParameterChanged(uint)));
}

int OperatorModel::rowCount(const QModelIndex& index) const
{
    if(index.isValid())
    {
        
        if(index.internalPointer())
        {
            const stromx::runtime::Parameter* group = 0;
        
            group = reinterpret_cast<const stromx::runtime::Parameter*>(index.internalPointer());
            return numDisplayedParameters(group);
        }
        else
        {
            return 0;
        }
    }
    else
    {
        return numDisplayedParameters(0) + PARAMETER_OFFSET;
    }
}

Qt::ItemFlags OperatorModel::flags(const QModelIndex& index) const
{   
    Qt::ItemFlags flags = QAbstractItemModel::flags(index);
    if(index.column() == 0 || m_op == 0 || !index.isValid())
        return flags;
    
    const stromx::runtime::Parameter* param = reinterpret_cast<stromx::runtime::Parameter*>(index.internalPointer());
  
    int row = index.parent().isValid() ? PARAMETER_OFFSET : index.row();
    
    switch(row)
    {
        case TYPE:
        case STATUS:
            break;
        
        case NAME:
            return flags | Qt::ItemIsEditable;
            
        default:
            return flags | m_server->parameterFlags(param->id());
    }    
    return flags;
}

QModelIndex OperatorModel::parent(const QModelIndex& child) const
{
    const stromx::runtime::Parameter* param = reinterpret_cast<const stromx::runtime::Parameter*>(child.internalPointer());
    
    // this is no parameter
    if(! param)
        return QModelIndex();
    
    // this is a top-level parameter
    if(! param->group())
        return QModelIndex();
    
    int row = rowOfDisplayedParameter(param->group());
    
    return createIndex(row, 0, (void*)(param->group()));
}

QModelIndex OperatorModel::index(int row, int column, const QModelIndex& parent) const
{
    const stromx::runtime::Parameter* group = 0;
    
    if(parent.isValid() && parent.internalPointer())
        group = reinterpret_cast<const stromx::runtime::Parameter*>(parent.internalPointer());
    
    const stromx::runtime::Parameter* param = 0;
    
    if(parent.isValid())
        param = parameterAtRow(group, row);
    else if(row >= PARAMETER_OFFSET)
        param = parameterAtRow(group, row - PARAMETER_OFFSET);
    
    return createIndex(row, column, (void*)(param));
}

QVariant OperatorModel::data(const QModelIndex& index, int role) const
{
    // return if any of the conditions below is not satisfied
    if(!index.isValid() || index.column() > 1 || m_op == 0)
        return QVariant();
    
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
        OTHER, // 3
        SETTING // 4
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
        PARAMETER_VALUE, // 8
        PARAMETER_SETTING // 9
    };
    
    // the decision table
    int table[4][2][5] =
    /* Row          0  0  0  0  0    0  0  0  0  0      1  1  1  1  1    1  1  1  1  1
     * Column       0  0  0  0  0    1  1  1  1  1      0  0  0  0  0    1  1  1  1  1
     * Role         0  1  2  3  4    0  1  2  3  4      0  1  2  3  4    0  1  2  3  4 */
    /* Action */ {{{1, 0, 0, 0, 0}, {2, 0, 0, 0, 0}}, {{3, 0, 0, 0, 0}, {4, 0, 0, 0, 0}},
                 
    /* Row          2  2  2  2  2    2  2  2  2  2      3  3  3  3  3    3  3  3  3  0 
     * Column       0  0  0  0  0    1  1  1  1  1      0  0  0  0  0    1  1  1  1  1
     * Role         0  1  2  3  4    0  1  2  3  4      0  1  2  3  4    0  1  2  3  4 */
    /* Action */  {{5, 0, 0, 0, 0}, {6, 6, 0, 0, 0}}, {{7, 0, 0, 0, 0}, {8, 8, 8, 0, 9}}};
     
    // extract row, column and role type
    int row = rowType(index);
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
    case MatrixRole:
        roleType = USER;
        break;
    case MinRole:
    case MaxRole:
    case StepRole:
    case NumRowsRole:
    case NumColsRole:
        roleType = SETTING;
        break;
    default:
        roleType = OTHER;
        break;
    }
    
    // ask the decision table
    ReturnValue action = ReturnValue(table[row][column][roleType]);
    
    // get a pointer to the parameter (is 0 if this index does not point to a parameter)
    const stromx::runtime::Parameter* param = reinterpret_cast<stromx::runtime::Parameter*>(index.internalPointer());
        
    // act accordingly
    switch(action)
    {
    case STRING_TYPE:
        return tr("Type");
    case TYPE:
        return tr("%1::%2").arg(QString::fromStdString(m_op->info().package()))
                          .arg(QString::fromStdString(m_op->info().type()));
    case STRING_STATUS:
        return tr("Status");
    case STATUS:
        return statusToString(m_op->status());
    case STRING_NAME:
        return tr("Name");
    case NAME:
        return QString::fromStdString(m_op->name());
    case PARAMETER_NAME:
        return QVariant(fromStromxTitle(param->title()));
    case PARAMETER_VALUE:
        return m_server->getParameter(param->id(), role);
    case PARAMETER_SETTING:
        return getParameterSetting(*param, role);
    case NOTHING:
    default:
        return QVariant();
    }
}

QVariant OperatorModel::getParameterSetting(const stromx::runtime::Parameter & param, int role)
{
    switch(role)
    {
        case MinRole:
            if(param.min().isVariant(stromx::runtime::Variant::NONE))
                return QVariant();
            return DataConverter::toQVariant(param.min(), param, Qt::EditRole);
            
        case MaxRole:
            if(param.max().isVariant(stromx::runtime::Variant::NONE))
                return QVariant();
            return DataConverter::toQVariant(param.max(), param, Qt::EditRole);
            
        case StepRole:
            if(param.step().isVariant(stromx::runtime::Variant::NONE))
                return QVariant();
            return DataConverter::toQVariant(param.step(), param, Qt::EditRole);
            
        case NumRowsRole:
            return param.rows();
            
        case NumColsRole:
            return param.cols();
            
        default:
            Q_ASSERT(false);
    }
    
    return QVariant();
}

bool OperatorModel::setData(const QModelIndex& index, const QVariant& value, int role)
{   
    if(role != Qt::EditRole)
        return false;
    
    if(!index.isValid() || index.column() == 0)
        return false;
    
    const stromx::runtime::Parameter* param = reinterpret_cast<stromx::runtime::Parameter*>(index.internalPointer());

    Row row = rowType(index);
    switch(row)
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
            return m_server->setParameter(param->id(), value);
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
    stromx::runtime::Position stromxPos(pos.x(), pos.y());
    m_op->setPosition(stromxPos);
    emit posChanged(m_pos);
}

int OperatorModel::rowOfDisplayedParameter(const stromx::runtime::Parameter* param) const
{
    const QList<const stromx::runtime::Parameter*> parameters = members(param->group());
    
    int row = 0;
    
    if(! param->group())
        row += PARAMETER_OFFSET;
    
    foreach(const stromx::runtime::Parameter* p, parameters)
    {
        if(m_server->parameterIsDisplayed(param->id()))
        {
            if(param == p)
                return row;
            
            row++;
        }
    }
    
    return -1;
}

const stromx::runtime::Parameter* OperatorModel::parameterAtRow(const stromx::runtime::Parameter* group, int row) const
{
    int currentRow = 0;
    const QList<const stromx::runtime::Parameter*> parameters = members(group);
    
    foreach(const stromx::runtime::Parameter* param, parameters)
    {
        if(m_server->parameterIsDisplayed(param->id()))
        {
            if(currentRow == row)
                return param;
            
            currentRow++;
        }
    }
    
    return 0;
}

QList<const stromx::runtime::Parameter*> OperatorModel::members(const stromx::runtime::Parameter* group) const
{
    QList<const stromx::runtime::Parameter*> params;
    
    if(! group)
    {    
        const std::vector<const stromx::runtime::Parameter*> & parameters = m_op->info().parameters();
        
        for(std::vector<const stromx::runtime::Parameter*>::const_iterator iter = parameters.begin();
            iter != parameters.end();
            ++iter)
        {
            if(! (*iter)->group())
                params.push_back(*iter);
        }
    }
    else
    { 
        const std::vector<const stromx::runtime::Parameter*> & parameters = group->members();
        
        for(std::vector<const stromx::runtime::Parameter*>::const_iterator iter = parameters.begin();
            iter != parameters.end();
            ++iter)
        {
            params.push_back(*iter);
        }
    }
    
    return params;
}

int OperatorModel::numDisplayedParameters(const stromx::runtime::Parameter* group) const
{
    int count = 0;
    const QList<const stromx::runtime::Parameter*> parameters = members(group);
    
    foreach(const stromx::runtime::Parameter* param, parameters)
    {
        if(m_server->parameterIsDisplayed(param->id()))
            count++;
    }
    return count;    
}

bool OperatorModel::isInitialized() const
{
    return m_op->status() != stromx::runtime::Operator::NONE;
}

bool OperatorModel::isActive() const
{
    return m_stream->isActive();
}

void OperatorModel::beginChangeInitialized()
{
    beginResetModel();
}

void OperatorModel::endChangeInitialized()
{
    // update the cache of the parameter server
    m_server->refresh();
        
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
    if(event->type() == ConnectorOccupyEvent::TYPE)
    {
        ConnectorOccupyEvent* occupyEvent = reinterpret_cast<ConnectorOccupyEvent*>(event);
        emit connectorOccupiedChanged(occupyEvent->type(), occupyEvent->id(), occupyEvent->occupied());
    }
    else if(event->type() == ConnectorDataEvent::TYPE)
    {
        ConnectorDataEvent* dataEvent = reinterpret_cast<ConnectorDataEvent*>(event);
        emit connectorDataChanged(dataEvent->type(), dataEvent->id(), dataEvent->access());
    }
}

void OperatorModel::handleParameterChanged(unsigned int id)
{
    // get the row of the parameter
    const stromx::runtime::Parameter & param = m_op->info().parameter(id);
    int row = rowOfDisplayedParameter(&param);
    Q_ASSERT(row >= 0);
    
    // emit the signal
    emit dataChanged(createIndex(row, 1, (void*)(&param)), createIndex(row, 1, (void*)(&param)));
}

void OperatorModel::setActiveFalse()
{
    // trigger the reset model signal (reset() is deprecated in Qt5)
    beginResetModel();
    endResetModel();
    emit activeChanged(false);
}

void OperatorModel::setActiveTrue()
{
    // trigger the reset model signal (reset() is deprecated in Qt5)
    beginResetModel();
    endResetModel();
    emit activeChanged(true);
}

#ifdef STROMX_STUDIO_QT4
void OperatorModel::connectNotify(const char* /*signal*/)
#else
void OperatorModel::connectNotify(const QMetaMethod & /*signal*/)
#endif // STROMX_STUDIO_QT4
{
    // if there are receivers for data change signals the according events must be sent
    if(receivers(SIGNAL(connectorDataChanged(OperatorModel::ConnectorType,uint,stromx::runtime::ReadAccess))))
        m_observer.setObserveData(true);  
}

#ifdef STROMX_STUDIO_QT4
void OperatorModel::disconnectNotify(const char* /*signal*/)
#else
void OperatorModel::disconnectNotify(const QMetaMethod & /*signal*/)
#endif // STROMX_STUDIO_QT4
{
    // if there are no receivers for data change signals do not send the events
    if(! receivers(SIGNAL(connectorDataChanged(OperatorModel::ConnectorType,uint,stromx::runtime::ReadAccess))))
        m_observer.setObserveData(false); 
}

QString OperatorModel::statusToString(int status)
{
    switch(status)
    {
    case stromx::runtime::Operator::NONE:
        return tr("None");
    case stromx::runtime::Operator::INITIALIZED:
        return tr("Initialized");
    case stromx::runtime::Operator::ACTIVE:
    case stromx::runtime::Operator::EXECUTING:
        return tr("Active");
    default:
        Q_ASSERT(false);
    }
    
    return QString();
}

OperatorModel::Row OperatorModel::rowType(const QModelIndex& index) const
{
    return (index.parent().isValid() || index.row() >= PARAMETER_OFFSET) ? PARAMETER_OFFSET : Row(index.row());
}

QDataStream& operator<<(QDataStream& stream, const OperatorModel* op)
{
    return stream << op->m_pos;
}

QDataStream& operator>>(QDataStream& stream, OperatorModel* op)
{
    stream >> op->m_pos;
    stromx::runtime::Position stromxPos(op->m_pos.x(), op->m_pos.y());
    op->op()->setPosition(stromxPos);
    return stream;
}

