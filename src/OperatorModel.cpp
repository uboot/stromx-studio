#include "OperatorModel.h"

#include <QUndoStack>
#include <stromx/core/Operator.h>
#include <stromx/core/Parameter.h>
#include "MoveOperatorCmd.h"
#include "StreamModel.h"

OperatorModel::OperatorModel(stromx::core::Operator* op, StreamModel* stream)
  : QAbstractTableModel(stream),
    m_op(op),
    m_stream(stream),
    m_package(QString::fromStdString(m_op->info().package())),
    m_type(QString::fromStdString(m_op->info().type())),
    m_name(QString::fromStdString(m_op->name())),
    offsetPosParam(3)
{

}

int OperatorModel::rowCount(const QModelIndex& index) const
{
    return accessibleParametersCount() + offsetPosParam;
//     m_op->info().parameters().size() + offsetPosParam;
//     return 2;
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

QVariant OperatorModel::data(const QModelIndex& index, int role) const
{
    if(role != Qt::DisplayRole)
        return QVariant();
    
    if(!index.isValid())
        return QVariant();
    
    if(m_op)
    {
        switch(index.row())
        {
            case 0:
            {
                if(index.column() == 0)
                    return tr("Type");
                else
                    return QVariant(QString::fromStdString(m_op->info().type()));
            }
            
            case 1:
            {
                if(index.column() == 0)
                    return tr("Status");
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
            
            case 2:
            {
                if(index.column() == 0)
                    return tr("Name");
                else
                    if (m_op->name().empty())
                        return QVariant(tr("<empty>"));
                    else
                        return QVariant(QString::fromStdString(m_op->name()));
            }
        
        
            default:
            {
                for(std::vector<const stromx::core::Parameter*>::const_iterator iter_par = m_op->info().parameters().begin();
                    iter_par != m_op->info().parameters().end();
                    ++iter_par)
                {
                    if(accessibleParameter(*iter_par))
                    {
                        if(index.column() == 0)
                            return QVariant(QString::fromStdString(m_op->info().parameters()[index.row()-offsetPosParam]->name()));
                        else
                            return QVariant(convertDataToQString(m_op->getParameter(m_op->info().parameters()[index.row()-offsetPosParam]->id())));
                    }
                    else
                        QVariant(tr("<not accesible>"));                  
                }
            }
        }
    }
    
    return QVariant();
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
    m_name = name;
    m_op->setName(name.toStdString());
    emit nameChanged(m_name);
}

void OperatorModel::doSetPos(const QPointF& pos)
{
    m_pos = pos;
    emit posChanged(m_pos);
}

QString OperatorModel::convertDataToQString(const stromx::core::Data& data) const
{
    QString dataString;
//     if (data.isVariant(stromx::core::DataVariant::BOOL))
//     {
//          const stromx::core::Bool & boolData = stromx::core::data_cast<const stromx::core::Bool&>(data);
//          dataString.setNum((unsigned int)(boolData));
//          return dataString;
//     }
    
    if (data.isVariant(stromx::core::DataVariant::INT_8))
    {
         const stromx::core::Int8 & int8Data = stromx::core::data_cast<const stromx::core::Int8&>(data);
         dataString.setNum((int)(int8Data));
         return dataString;
    }
    
    if (data.isVariant(stromx::core::DataVariant::UINT_8))
    {
         const stromx::core::UInt8 & uintData = stromx::core::data_cast<const stromx::core::UInt8&>(data);
         dataString.setNum((unsigned int)(uintData));
         return dataString;
    }
    
    if (data.isVariant(stromx::core::DataVariant::INT_16))
    {
         const stromx::core::Int16 & int16Data = stromx::core::data_cast<const stromx::core::Int16&>(data);
         dataString.setNum((int)(int16Data));
         return dataString;
    }
    
    if (data.isVariant(stromx::core::DataVariant::UINT_16))
    {
         const stromx::core::UInt16 & uint16Data = stromx::core::data_cast<const stromx::core::UInt16&>(data);
         dataString.setNum((unsigned int)(uint16Data));
         return dataString;
    }
    
    if (data.isVariant(stromx::core::DataVariant::INT_32))
    {
         const stromx::core::Int32 & int32Data = stromx::core::data_cast<const stromx::core::Int32&>(data);
         dataString.setNum((int)(int32Data));
         return dataString;
    }
    
    if (data.isVariant(stromx::core::DataVariant::UINT_32))
    {
         const stromx::core::UInt32 & uint32Data = stromx::core::data_cast<const stromx::core::UInt32&>(data);
         dataString.setNum((unsigned int)(uint32Data));
         return dataString;
    }
    
    if (data.isVariant(stromx::core::DataVariant::FLOAT))
    {
         const stromx::core::Float & floatData = stromx::core::data_cast<const stromx::core::Float&>(data);
         dataString.setNum((double)(floatData));
         return dataString;
    }
    
    if (data.isVariant(stromx::core::DataVariant::DOUBLE))
    {
         const stromx::core::Double & doubleData = stromx::core::data_cast<const stromx::core::Double&>(data);
         dataString.setNum((double)(doubleData));
         return dataString;
    }
    
    else
        return QString("<Unknown>");
}

int OperatorModel::accessibleParametersCount() const
{
    int count = 0;
    for(std::vector<const stromx::core::Parameter*>::const_iterator iter_par = m_op->info().parameters().begin();
        iter_par != m_op->info().parameters().end();
        ++iter_par)
        {
            if(accessibleParameter(*iter_par))
            {
                count++;
            }
        }
    return count;    
}

bool OperatorModel::accessibleParameter(const stromx::core::Parameter* const par) const
{
    if(m_op->status() != stromx::core::Operator::NONE)
    {
        if (par->accessMode() == stromx::core::Parameter::NO_ACCESS)
        {
            return false;
        }
        else
        {
            return true;
        }
    }
    else
    {
        if(par->accessMode() == stromx::core::Parameter::NONE_READ || par->accessMode() == stromx::core::Parameter::NONE_WRITE)
        {
            return true;
        }
        else
        {
            return false;
        }
    }
}

bool OperatorModel::isInitialized() const
{
    return m_op->status() != stromx::core::Operator::NONE;
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

QDataStream& operator<<(QDataStream& stream, const OperatorModel* op)
{
    return stream << op->m_pos;
}

QDataStream& operator>>(QDataStream& stream, OperatorModel* op)
{
    return stream >> op->m_pos;
}

