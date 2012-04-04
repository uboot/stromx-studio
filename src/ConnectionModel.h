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
    
    friend class SetThreadCmd;
    
public:
    explicit ConnectionModel(OperatorModel* sourceOp, unsigned int outputId,
                             OperatorModel* targetOp, unsigned int inputId, StreamModel * stream = 0);
    
    virtual int rowCount(const QModelIndex & index) const;
    virtual int columnCount(const QModelIndex & index) const;
    virtual QVariant data(const QModelIndex & index, int role) const;
    virtual bool setData(const QModelIndex & index, const QVariant & value, int role);
    virtual Qt::ItemFlags flags(const QModelIndex& index) const;
    
    ThreadModel* thread() const { return m_thread; }
    
    /** 
     * Sets the thread of this connection and the adds the input of 
     * this connection to the stromx stream.
     */
    
    /** Pushes a set thread command on the undo stack. */
    void setThread(ThreadModel* thread);
    
    OperatorModel* sourceOp() const { return m_sourceOp; }
    OperatorModel* targetOp() const { return m_targetOp; }
    
    unsigned int inputId() const { return m_inputId; }
    unsigned int outputId() const { return m_outputId; }
    
    /** Adds itself to the source and target operators. */
    void connectToOperators();
    
    /** Removes itself from the source and target operators. */
    void disconnectFromOperators();
    
signals:
    void threadChanged(ThreadModel* thread);
    
private:
    enum Row
    {
        THREAD = 0
    };
    
    /** 
     * Sets the thread of this connection and the adds the input of 
     * this connection to the stromx stream.
     */
    void doSetThread(ThreadModel* thread);
    
    OperatorModel* m_sourceOp;
    unsigned int m_outputId;
    OperatorModel* m_targetOp;
    unsigned int m_inputId;
    StreamModel* m_stream;
    ThreadModel* m_thread;
};

#endif // CONNECTIONMODEL_H