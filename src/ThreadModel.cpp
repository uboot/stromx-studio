#include "ThreadModel.h"

#include "StreamModel.h"

ThreadModel::ThreadModel(stromx::core::Thread* thread, StreamModel* stream)
  : QObject(stream),
    m_thread(thread),
    m_stream(stream)
{

}

void ThreadModel::setThread(stromx::core::Thread* thread)
{
    m_thread = thread;
}

void ThreadModel::setColor(const QColor& color)
{
    if(color != m_color)
    {
        m_color = color;
        emit colorChanged(color);
    }
}

QDataStream& operator<<(QDataStream& stream, const ThreadModel* thread)
{
    return stream << thread->m_color;
}

QDataStream& operator>>(QDataStream& stream, ThreadModel* thread)
{
    return stream >> thread->m_color;
}
