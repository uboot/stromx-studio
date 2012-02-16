#include "JoinStreamTask.h"

#include <stromx/core/Stream.h>
#include "StreamModel.h"

void JoinStreamTask::run()
{
    m_stream->join();
}
