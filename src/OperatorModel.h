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

namespace stromx
{
    namespace core
    { 
        class Operator;
    }
}

class ConnectionModel;
class StreamModel;

class OperatorModel : public QAbstractTableModel
{
    Q_OBJECT
    Q_PROPERTY(QPointF pos READ pos WRITE setPos)
    Q_PROPERTY(bool initialized READ isInitialized WRITE setInitialized)
    
public:
    explicit OperatorModel(stromx::core::Operator* op, StreamModel *stream = 0);
    
    stromx::core::Operator* const op() const { return m_op; }
    
    int rowCount(const QModelIndex & index) const;
    int columnCount(const QModelIndex & index) const;
    QVariant data(const QModelIndex & index, int role) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;
    
    const QPointF & pos() const { return m_pos; }
    void setPos(const QPointF & pos);
    
    bool isInitialized() const;
    void setInitialized(bool initialized);
    
    void addConnection(ConnectionModel* connection);
    void removeConnection(ConnectionModel* connection);
    
signals:
    void posChanged(const QPointF & pos);
    void initializedChanged(bool status);
    
private:
    stromx::core::Operator* m_op;
    StreamModel* m_stream;
    QPointF m_pos;
    QSet<ConnectionModel*> m_connections;
};

#endif // OPERATORMODEL_H
