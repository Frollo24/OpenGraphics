#include "ogpch.h"
#include "Logger.h"

namespace OpenGraphics
{
    void Logger::Init(const LogLevel level)
    {
        s_Level = level;
    }

    void Logger::Shutdown()
    {
        s_Level = LogLevel::Off;
    }
}
