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

#ifndef THREADLISTMODEL_H
#define THREADLISTMODEL_H

#include <QAbstractTableModel>
#include <QList>

class ThreadModel;

/**
 * \brief List of thread models
 * 
 * This class contains a list of ThreadModel objects and provides all necessary
 * functions of QAbstractTableModel to display and edit the thread list via a table view.
 * This class is usually owned and used by a StreamModel object.
 */
class ThreadListModel : public QAbstractTableModel
{
    Q_OBJECT
    
    friend class StreamModel;
    
public:
    /** Constructs a thread list model. */
    explicit ThreadListModel(QObject *parent = 0);
    
    virtual int rowCount(const QModelIndex & parent) const;
    virtual int columnCount(const QModelIndex & parent) const;
    virtual QVariant data(const QModelIndex & index, int role = Qt::DisplayRole) const;
    virtual bool setData(const QModelIndex & index, const QVariant & value, int role);
    virtual Qt::ItemFlags flags(const QModelIndex & index) const;
    virtual QVariant headerData(int section, Qt::Orientation orientation, int role) const;
    virtual QModelIndex index(int row, int column, const QModelIndex & parent) const;
    
    /** Returns the threads in the list. */
    const QList<ThreadModel*> threads() const { return m_threads; }
    
    /** 
     * Returns the thread displayed at the \c index. If \c index does not point to
     * a thread 0 is returned.
     */
    ThreadModel* thread(const QModelIndex & index) const;
    
    friend QDataStream & operator<< (QDataStream & stream, const ThreadListModel * threadList);
    friend QDataStream & operator>> (QDataStream & stream, ThreadListModel * threadList);
     
private slots:
    /** Updates the displayed data of \c thread. */
    void updateThread(ThreadModel* thread);
    
private:
    /** The number of columns of the model. */
    const static int NUM_COLUMNS = 2;
    
    void addThread(ThreadModel* thread);
    void removeThread(ThreadModel* thread);
    void removeAllThreads();
    
    QList<ThreadModel*> m_threads;
};
    
#endif // THREADLISTMODEL_H
