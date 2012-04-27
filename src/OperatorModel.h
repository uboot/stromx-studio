/* 
*  Copyright 2012 Thomas Fidler
*
*  This file is part of stromx-studio.
*
*  Stromx-studio is free software: you can redistribute it and/or modify
*  it under the terms of the GNU General Public License as published by
*  the Free Software Foundation, either version 3 of the License, or
*  (at your option) any later version.
*
*  Stromx-studio is distributed in the hope that it will be useful,
*  but WITHOUT ANY WARRANTY; without even the implied warranty of
*  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
*  GNU General Public License for more details.
*
*  You should have received a copy of the GNU General Public License
*  along with stromx-studio.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef OPERATORMODEL_H
#define OPERATORMODEL_H

#include <QAbstractTableModel>
#include <QPointF>
#include <QSet>
#include <stromx/core/DataContainer.h>

namespace stromx
{
    namespace core
    { 
        class Data;
        class Operator;
        class Parameter;
    }
}

class QUndoStack;
class ConnectionModel;
class ConnectorObserver;
class StreamModel;

class OperatorModel : public QAbstractTableModel
{
    Q_OBJECT
    Q_PROPERTY(QString package READ package)
    Q_PROPERTY(QString type READ type)
    Q_PROPERTY(QString name READ name WRITE setName)
    Q_PROPERTY(QPointF pos READ pos WRITE setPos)
    Q_PROPERTY(bool initialized READ isInitialized WRITE setInitialized)
    
    friend class MoveOperatorCmd;
    friend class RenameOperatorCmd;
    friend class SetParameterCmd;
    friend class StreamModel;
    friend QDataStream & operator<< (QDataStream & stream, const OperatorModel * op);
    friend QDataStream & operator>> (QDataStream & stream, OperatorModel * op);
    
public:
    enum ConnectorType
    {
        INPUT,
        OUTPUT
    };
    
    explicit OperatorModel(stromx::core::Operator* op, StreamModel *stream);
    virtual ~OperatorModel();
    
    stromx::core::Operator* const op() const { return m_op; }
    
    virtual int rowCount(const QModelIndex & index) const;
    virtual int columnCount(const QModelIndex & index) const;
    virtual QVariant data(const QModelIndex & index, int role) const;
    virtual bool setData(const QModelIndex & index, const QVariant & value, int role);
    virtual QVariant headerData(int section, Qt::Orientation orientation, int role) const;
    virtual Qt::ItemFlags flags(const QModelIndex & index) const;
    virtual void customEvent(QEvent* event);
    
    const QString & package() const;
    const QString & type() const;
    
    const QString & name() const { return m_name; }
    void setName(const QString & name);
    
    const QPointF & pos() const { return m_pos; }
    void setPos(const QPointF & pos);
    
    bool isInitialized() const;
    bool isActive() const;
    
    const QSet<ConnectionModel*> connections() const { return m_connections; }
    void addConnection(ConnectionModel* connection);
    void removeConnection(ConnectionModel* connection);
    
    QUndoStack* undoStack() const;
    
signals:
    void posChanged(const QPointF & pos);
    void nameChanged(const QString & name);
    
    /** The stromx operator was initialized or deinitialized. */
    void initializedChanged(bool status);
    
    /** The stromx operator was activated or deactivated. */
    void activeChanged(bool status);
    
    /**
     * The connector specified by \c type and \c id was set to a data pointer which
     * was either zero (<tt>occupied == false</tt>) or non-zero (<tt>occupied == true</tt>).
     */
    void connectorOccupiedChanged(OperatorModel::ConnectorType type, unsigned int id, bool occupied);
    
    /** The connector specified by \c type and \c id was set to \c data. */
    void connectorDataChanged(OperatorModel::ConnectorType type, unsigned int id, stromx::core::DataContainer data);
    
protected:
    virtual void connectNotify(const char * signal);
    virtual void disconnectNotify(const char * signal);

private slots:
    /** Emits <tt>activeChanged(true)</tt>. */
    void setActiveTrue();
    
    /** Emits <tt>activeChanged(false)</tt>. */
    void setActiveFalse();
    
private:
    enum Row
    {
        TYPE,
        STATUS,
        NAME,
        PARAMETER_OFFSET
    };
    
    static QString statusToString(int status);
    
    void doSetName(const QString & name);
    void doSetParameter(unsigned int paramId, const stromx::core::Data& newValue);
    void doSetPos(const QPointF & pos);
    void setInitialized(bool initialized);
    int accessibleParametersCount() const;
    bool parameterIsReadAccessible(const stromx::core::Parameter* const par) const;
    bool parameterIsWriteAccessible(const stromx::core::Parameter* const par) const;
    QString convertDataToQString(const stromx::core::Data& data, const stromx::core::Parameter* param) const;
    
    stromx::core::Operator* m_op;
    StreamModel* m_stream;
    QPointF m_pos;
    QSet<ConnectionModel*> m_connections;
    QString m_package;
    QString m_type;
    QString m_name;
    unsigned int m_offsetPosParam;
    ConnectorObserver* m_observer;
};

QDataStream & operator<< (QDataStream & stream, const OperatorModel * op);
QDataStream & operator>> (QDataStream & stream, OperatorModel * op);

#endif // OPERATORMODEL_H
