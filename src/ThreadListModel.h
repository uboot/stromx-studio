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

class ThreadListModel : public QAbstractTableModel
{
    Q_OBJECT
    
    friend class StreamModel;
    
public:
    explicit ThreadListModel(QObject *parent = 0);
    
    virtual int rowCount(const QModelIndex & parent) const;
    virtual int columnCount(const QModelIndex & parent) const;
    virtual QVariant data(const QModelIndex & index, int role = Qt::DisplayRole) const;
    virtual bool setData(const QModelIndex & index, const QVariant & value, int role);
    Qt::ItemFlags flags(const QModelIndex & index) const;
    virtual QVariant headerData(int section, Qt::Orientation orientation, int role) const;
    virtual QModelIndex index(int row, int column, const QModelIndex & parent) const;
    
    const QList<ThreadModel*> threads() const { return m_threads; }
    ThreadModel* thread(const QModelIndex & index) const;
    
    friend QDataStream & operator<< (QDataStream & stream, const ThreadListModel * threadList);
    friend QDataStream & operator>> (QDataStream & stream, ThreadListModel * threadList);
    
private:
    void addThread(ThreadModel* thread);
    void removeThread(ThreadModel* thread);
    void removeAllThreads();
    
    QList<ThreadModel*> m_threads;
};

QDataStream & operator<< (QDataStream & stream, const ThreadListModel * threadList);
QDataStream & operator>> (QDataStream & stream, ThreadListModel * threadList);
    
#endif // THREADLISTMODEL_H
