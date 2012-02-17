/* 
*  Copyright 2011 xxx yyy
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

#ifndef THREADEDITOR_H
#define THREADEDITOR_H

#include <QWidget>

class QAbstractTableModel;
class QModelIndex;
class QTableView;
class ThreadModel;
class StreamModel;

class ThreadEditor : public QWidget
{
    Q_OBJECT

public:
    explicit ThreadEditor(QWidget *parent = 0);
    
    QAction* createAddThreadAction(QObject* parent);
    QAction* createRemoveThreadAction(QObject* parent);
    
    void setModel(StreamModel* model);
    
public slots:
    void addThread() const;
    void removeThread() const;
    
signals:
    void removeThreadActiveChanged(bool selected);
    void addThreadActiveChanged(bool selected);
    
private slots:
    void updateThreadSelected( const QModelIndex & current, const QModelIndex & previous );
    void updateStreamActive();
    
private:
    ThreadModel* selectedThread() const;
    
    QTableView* m_table;
    StreamModel* m_model;
};

#endif // THREADEDITOR_H
