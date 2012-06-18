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
#include <stromx/core/DataContainer.h>
#include "ConnectorObserver.h"
#include "PropertyModel.h"

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
class StreamModel;

/** \brief Model of a stromx operator. */
class OperatorModel : public PropertyModel
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
    
    /** Returns the stromx operator of this model. */
    stromx::core::Operator* const op() const { return m_op; }
    
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
     * stromx operator different from stromx::core::Operator::NONE.
     */
    bool isInitialized() const;
    
    /** 
     * Returns true if the operator is activated, i.e. the status of the
     * stromx operator is stromx::core::Operator::ACTIVE or 
     * stromx::core::Operator::EXECUTING.
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
    void connectorDataChanged(OperatorModel::ConnectorType type, unsigned int id, stromx::core::DataContainer data);
      
    /** An operation accessing a parameter of the operator timed out. */
    void parameterAccessTimedOut() const;
    
protected:
    virtual void customEvent(QEvent* event);
    virtual void connectNotify(const char * signal);
    virtual void disconnectNotify(const char * signal);

private slots:
    /** Resets the model and emits <tt>activeChanged(true)</tt>. */
    void setActiveTrue();
    
    /** Resets the model and emits <tt>activeChanged(false)</tt>. */
    void setActiveFalse();
    
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
    
    /** Sets the name of the operator. */
    void doSetName(const QString & name);
    
    /** Sets the parameter \c paramId to \c newValue. */
    void doSetParameter(unsigned int paramId, const stromx::core::Data& newValue);
    
    /** Sets the position of the operator in the stream scene. */
    void doSetPos(const QPointF & pos);
    
    /** 
     * Initializes or deinitializes the operator. This functions is called by 
     * the class StreamModel.
     */
    void setInitialized(bool initialized);
    
    /**
     * Returns the number of members \c group which are currently displayed.
     * If \c group is 0 the number of displayed top-level parameters is returned.
     */
    int numDisplayedParameters(const stromx::core::Parameter* group) const;
    
    /**
     * Returns the parameter at the position \c row of the displayed parameters in \c group. 
     * The row index \c row is the position of the parameter in the list of the currently
     * displayed parameters of \c group. If \c group is 0 the parameter at the
     * position \c row of all displayed top-level parameters is returned.
     */
    const stromx::core::Parameter* parameterAtRow(const stromx::core::Parameter* group, int row) const;
    
    /** 
     * Returns the row at which the parameter \c param is displayed. Returns -1 of the parameter
     * is not currently displayed.
     */
    int rowOfDisplayedParameter(const stromx::core::Parameter* param) const;
    
    /** 
     * Returns true if the parameter \c par is read-accessible or it is a non-empty parameter
     * group, i.e. it has at least one member. 
     */
    bool parameterIsDisplayed(const stromx::core::Parameter* par) const;
    
    /** Returns whether the parameter \c par can currently be read. */
    bool parameterIsReadAccessible(const stromx::core::Parameter* par) const;
    
    /** Returns whether the parameter \c par can currently be written. */
    bool parameterIsWriteAccessible(const stromx::core::Parameter* par) const;
    
    /** 
     * Returns all parameters which are members of the parameter \c group. Returns all top-level
     * parameters if \c group is 0.
     */
    QList<const stromx::core::Parameter*> members(const stromx::core::Parameter* group) const;
    
    
    /** 
     * Returns the row type of the row \c index refers to.
     */
    Row rowType(const QModelIndex & index) const;
    
    stromx::core::Operator* m_op;
    StreamModel* m_stream;
    QPointF m_pos;
    QSet<ConnectionModel*> m_connections;
    QString m_package;
    QString m_type;
    QString m_name;
    unsigned int m_offsetPosParam;
    ConnectorObserver m_observer;
};

QDataStream & operator<< (QDataStream & stream, const OperatorModel * op);
QDataStream & operator>> (QDataStream & stream, OperatorModel * op);

#endif // OPERATORMODEL_H
