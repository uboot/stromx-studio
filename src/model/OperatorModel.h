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

#include <QPointF>
#include <QSet>
#include <stromx/runtime/DataContainer.h>
#include <stromx/runtime/ReadAccess.h>
#include "ConnectorObserver.h"
#include "PropertyModel.h"

namespace stromx
{
    namespace runtime
    { 
        class Data;
        class Operator;
        class Parameter;
        class ParameterError;
    }
}

class QUndoStack;
class ConnectionModel;
class ErrorData;
class ParameterServer;
class StreamModel;

/** \brief Model of a stromx operator. */
class OperatorModel : public PropertyModel
{
    Q_OBJECT
    Q_PROPERTY(QString package READ package)
    Q_PROPERTY(QString type READ type)
    Q_PROPERTY(QString name READ name WRITE setName)
    Q_PROPERTY(QPointF pos READ pos WRITE setPos)
    Q_PROPERTY(bool initialized READ isInitialized)
    
    friend class MoveOperatorCmd;
    friend class RenameOperatorCmd;
    friend class StreamModel;
    friend QDataStream & operator<< (QDataStream & stream, const OperatorModel * op);
    friend QDataStream & operator>> (QDataStream & stream, OperatorModel * op);
    
public:
    enum ConnectorType
    {
        INPUT,
        OUTPUT
    };
    
    explicit OperatorModel(stromx::runtime::Operator* op, StreamModel *stream);
    
    /** Returns the stromx operator of this model. */
    stromx::runtime::Operator* op() const { return m_op; }
    
    /** Returns the package the operator belongs to. */
    const QString & package() const;
    
    /** Returns the type of the operator. */
    const QString & type() const;
    
    /** Returns the name of the operator. */
    const QString & name() const { return m_name; }
    
    /** Pushes a set name command on the undo stack. */
    void setName(const QString & name);
    
    /** Returns the position of the operator in the stream scene. */
    const QPointF & pos() const { return m_pos; }
    
    /** Pushes a set position command on the undo stack. */
    void setPos(const QPointF & pos);
    
    /** 
     * Returns true if the operator is initialized, i.e. the status of the
     * stromx operator different from stromx::runtime::Operator::NONE.
     */
    bool isInitialized() const;
    
    /** 
     * Returns true if the operator is activated, i.e. the status of the
     * stromx operator is stromx::runtime::Operator::ACTIVE or 
     * stromx::runtime::Operator::EXECUTING.
     */
    bool isActive() const;
    
    /** 
     * Returns the connections which end or start at this operator.
     */
    const QSet<ConnectionModel*> connections() const { return m_connections; }
    
    /** Adds the connection to the list of current connections of this operator. */
    void addConnection(ConnectionModel* connection);
    
    /** Removes a connection from the list of current connections of this operator */
    void removeConnection(ConnectionModel* connection);
    
    /** Returns the undo stack. */
    QUndoStack* undoStack() const;
    
    virtual int rowCount(const QModelIndex & index) const;
    virtual QVariant data(const QModelIndex & index, int role) const;
    virtual bool setData(const QModelIndex & index, const QVariant & value, int role);
    virtual Qt::ItemFlags flags(const QModelIndex & index) const;
    virtual QModelIndex index(int row, int column, const QModelIndex& parent) const;
    virtual QModelIndex parent(const QModelIndex& child) const;
    
signals:
    /** The position of the operator on the stream scene has changed. */
    void posChanged(const QPointF & pos);
    
    /** The name of the operator has changed. */
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
    void connectorDataChanged(OperatorModel::ConnectorType type, unsigned int id,
                              stromx::runtime::ReadAccess access);
      
    /** 
     * An operation accessing a parameter of the operator or data at an operator
     * connector timed out. 
     */
    void operatorAccessTimedOut() const;
    
    /** A parameter occurred while setting a parameter. */
    void parameterErrorOccurred(const ErrorData &) const;
    
protected:
    virtual void customEvent(QEvent* event);
    
#ifdef STROMX_STUDIO_QT4
    virtual void connectNotify(const char * signal);
    virtual void disconnectNotify(const char * signal);
#else
    virtual void connectNotify(const QMetaMethod & signal);
    virtual void disconnectNotify(const QMetaMethod & signal);
#endif // STROMX_STUDIO_QT4

private slots:
    /** Resets the model and emits <tt>activeChanged(true)</tt>. */
    void setActiveTrue();
    
    /** Resets the model and emits <tt>activeChanged(false)</tt>. */
    void setActiveFalse();
    
    /** Emits a data changed event for the cell of the parameter \c id. */
    void handleParameterChanged(unsigned int id);
    
private:
    enum Row
    {
        TYPE,
        STATUS,
        NAME,
        PARAMETER_OFFSET
    };
    
    static const unsigned int TIMEOUT;
    
    static QString statusToString(int status);
    
    /** Returns the min, max or step (depending on \c role) value of \c param. */
    static QVariant getParameterSetting(const stromx::runtime::Parameter & param, int role);
    
    /** Sets the name of the operator. */
    void doSetName(const QString & name);
    
    /** Sets the position of the operator in the stream scene. */
    void doSetPos(const QPointF & pos);
    
    /** 
     * Starts a change of the initialzed state of the operator. This functions is called by 
     * the class StreamModel.
     */
    void beginChangeInitialized();
    
    /** 
     * Ends a change of the initialzed state of the operator. This functions is called by 
     * the class StreamModel. This triggers an update of the operator model.
     */
    void endChangeInitialized();
    
    /**
     * Returns the number of members \c group which are currently displayed.
     * If \c group is 0 the number of displayed top-level parameters is returned.
     */
    int numDisplayedParameters(const stromx::runtime::Parameter* group) const;
    
    /**
     * Returns the parameter at the position \c row of the displayed parameters in \c group. 
     * The row index \c row is the position of the parameter in the list of the currently
     * displayed parameters of \c group. If \c group is 0 the parameter at the
     * position \c row of all displayed top-level parameters is returned.
     */
    const stromx::runtime::Parameter* parameterAtRow(const stromx::runtime::Parameter* group, int row) const;
    
    /** 
     * Returns the row at which the parameter \c param is displayed. Returns -1 of the parameter
     * is not currently displayed.
     */
    int rowOfDisplayedParameter(const stromx::runtime::Parameter* param) const;
    
    /** 
     * Returns all parameters which are members of the parameter \c group. Returns all top-level
     * parameters if \c group is 0.
     */
    QList<const stromx::runtime::Parameter*> members(const stromx::runtime::Parameter* group) const;
    
    /** 
     * Returns the row type of the row \c index refers to.
     */
    Row rowType(const QModelIndex & index) const;
    
    stromx::runtime::Operator* m_op;
    StreamModel* m_stream;
    QPointF m_pos;
    QSet<ConnectionModel*> m_connections;
    QString m_package;
    QString m_type;
    QString m_name;
    unsigned int m_offsetPosParam;
    ConnectorObserver m_observer;
    ParameterServer* m_server;
};

QDataStream & operator<< (QDataStream & stream, const OperatorModel * op);
QDataStream & operator>> (QDataStream & stream, OperatorModel * op);

#endif // OPERATORMODEL_H
