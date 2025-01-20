#pragma once

#include "OpenEngine/Core/Includes.h"
#include "OpenEngine/Core/Logger.h"

#ifdef OG_DEBUG
#define OG_ENABLE_ASSERTS
#define OG_DEBUGBREAK() __debugbreak()
#else
#define OG_DEBUGBREAK()
#endif

#ifdef OG_ENABLE_ASSERTS
#define OG_VERIFY(x)      ::OpenGraphics::Assertions::Verify((x), __FILE__, __LINE__)
#define OG_ASSERT(x, ...) ::OpenGraphics::Assertions::Assert((x), __VA_ARGS__)
#else
#define OG_VERIFY(x)
#define OG_ASSERT(x, ...)
#endif

namespace OpenGraphics
{
    class Assertions
    {
    public:
        static void Verify(bool condition, const char* file, int32_t line)
        {
            if (!condition)
            {
                Logger::Error("Verification Failed at File: {0}; Line: {1}", file, line);
                OG_DEBUGBREAK();
            }
        }

        template<typename... Args>
        static void Assert(bool condition, Args&&... args)
        {
            if (!condition)
            {
                Logger::Error("Assertion Failed: {0}", std::forward<Args>(args)...);
                OG_DEBUGBREAK();
            }
        }
    };
}
