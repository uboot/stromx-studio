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

#ifndef THREADMODEL_H
#define THREADMODEL_H

#include <QColor>
#include <QObject>

namespace stromx
{
    namespace core
    {
        class Thread;
    }
}

class StreamModel;

class ThreadModel : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QColor color READ color WRITE setColor)
    
    friend class RenameThreadCmd;
    friend class SetThreadColorCmd;
    friend QDataStream & operator<< (QDataStream & stream, const ThreadModel * thread);
    friend QDataStream & operator>> (QDataStream & stream, ThreadModel * thread);
    
public:
    explicit ThreadModel(stromx::core::Thread* thread, StreamModel *stream = 0);
    
    stromx::core::Thread* thread() const { return m_thread; }
    void setThread(stromx::core::Thread* thread);
    
    const QString & name() const { return m_name; }
    void setName(const QString & name);
    
    const QColor & color() const { return m_color; }
    void setColor(const QColor & color);
    
signals:
    /** The color of the thread changed. */
    void colorChanged(const QColor & color);
    
    /** The name of the thread changed. */
    void nameChanged(const QString & name);
    
    /** A property of the thread changed. */
    void changed(ThreadModel* thread);
    
private:
    void doSetColor(const QColor & color);
    void doSetName(const QString & name);
    
    stromx::core::Thread* m_thread;
    StreamModel* m_stream;
    QString m_name;
    QColor m_color;
};

QDataStream & operator<< (QDataStream & stream, const ThreadModel * thread);
QDataStream & operator>> (QDataStream & stream, ThreadModel * thread);

#endif // THREADMODEL_H
