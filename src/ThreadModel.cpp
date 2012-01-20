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
