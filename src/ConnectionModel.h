/* 
*  Copyright 2012 Matthias Fuchs
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

#ifndef CONNECTIONMODEL_H
#define CONNECTIONMODEL_H

#include <QAbstractTableModel>

class OperatorModel;
class StreamModel;
class ThreadModel;

class ConnectionModel : public QAbstractTableModel
{
    Q_OBJECT
    
public:
    explicit ConnectionModel(OperatorModel* sourceOp, unsigned int outputId,
                             OperatorModel* targetOp, unsigned int inputId, StreamModel * stream = 0);
    
    int rowCount(const QModelIndex & index) const;
    int columnCount(const QModelIndex & index) const;
    QVariant data(const QModelIndex & index, int role) const;
    
    ThreadModel* thread() const { return m_thread; }
    void setThread(ThreadModel* thread);
    
    OperatorModel* sourceOp() const { return m_sourceOp; }
    OperatorModel* targetOp() const { return m_targetOp; }
    
    unsigned int inputId() const { return m_inputId; }
    unsigned int outputId() const { return m_outputId; }
    
signals:
    void threadChanged(ThreadModel* thread);
    
private:
    OperatorModel* m_sourceOp;
    unsigned int m_outputId;
    OperatorModel* m_targetOp;
    unsigned int m_inputId;
    StreamModel* m_stream;
    ThreadModel* m_thread;
};

#endif // CONNECTIONMODEL_H