#include "JoinStreamTask.h"

#include <stromx/core/Stream.h>
#include "StreamModel.h"

void JoinStreamTask::run()
{
    if(m_stream)
        m_stream->join();
}

void JoinStreamTask::setStream(stromx::core::Stream* stream)
{
    m_stream = stream;
}
