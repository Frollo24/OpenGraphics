#include "Logger.h"

namespace OpenGraphics
{
    void Logger::Init(LogLevel level)
    {
        s_Level = level;
    }

    void Logger::Shutdown()
    {
        s_Level = LogLevel::Off;
    }
}
