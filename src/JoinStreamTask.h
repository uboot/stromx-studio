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

#ifndef JOINSTREAMTASK_H
#define JOINSTREAMTASK_H

#include <QThread>

namespace stromx
{
    namespace core
    {
        class Stream;
    }
}

class StreamModel;

/** A thread which waits for a given stromx stream to join. */
class JoinStreamTask : public QThread
{
public:
    /** Constructs a tas but does not start it. */
    explicit JoinStreamTask(QObject* parent = 0) : QThread(parent), m_stream(0) {}
    
    /** Returns the current stromx stream handled by this task. */
    const stromx::core::Stream* stream() const { return m_stream; }
    
    /** Sets the stromx stream handled by this task. */
    void setStream(stromx::core::Stream* stream);
    
protected:
    /** Calls stromx::core::Stream::join() of the current stream. */
    virtual void run();
    
private:
    stromx::core::Stream* m_stream;
};

#endif // ADDOPERATORCMD_H