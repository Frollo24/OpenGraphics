#pragma once

#include "OpenEngine/Core/Build.h"
#include <cstdint>
#include <string>
#include <iostream>
#include <format>

namespace OpenGraphics
{
    enum class LogLevel : uint8_t
    {
        Trace,
        Debug,
        Info,
        Warn,
        Error,
        Fatal,
        Off
    };

    class OPEN_API Logger
    {
    public:
        static void Init(LogLevel level = LogLevel::Info);
        static void Shutdown();

        template <typename... Args>
        static void Log(LogLevel level, const std::string& message, Args&&... args)
        {
            std::cout << ParseLogLevel(level);
            std::cout << std::vformat(message, std::make_format_args(args...)) << "\u001b[0m" << std::endl;
        }

        template <typename T>
        static void Log(LogLevel level, const T& message)
        {
            std::cout << ParseLogLevel(level);
            std::cout << message << "\u001b[0m" << std::endl;
        }

        template <typename... Args>
        static void Trace(const std::string& message, Args&&... args)
        {
            Log(LogLevel::Trace, message, std::forward<Args>(args)...);
        }

        template <typename T>
        static void Trace(const T& message)
        {
            Log(LogLevel::Trace, message);
        }

        template <typename... Args>
        static void Debug(const std::string& message, Args&&... args)
        {
            Log(LogLevel::Debug, message, std::forward<Args>(args)...);
        }

        template <typename T>
        static void Debug(const T& message)
        {
            Log(LogLevel::Debug, message);
        }

        template <typename... Args>
        static void Info(const std::string& message, Args&&... args)
        {
            Log(LogLevel::Info, message, std::forward<Args>(args)...);
        }

        template <typename T>
        static void Info(const T& message)
        {
            Log(LogLevel::Info, message);
        }

        template <typename... Args>
        static void Warn(const std::string& message, Args&&... args)
        {
            Log(LogLevel::Warn, message, std::forward<Args>(args)...);
        }

        template <typename T>
        static void Warn(const T& message)
        {
            Log(LogLevel::Warn, message);
        }

        template <typename... Args>
        static void Error(const std::string& message, Args&&... args)
        {
            Log(LogLevel::Error, message, std::forward<Args>(args)...);
        }

        template <typename T>
        static void Error(const T& message)
        {
            Log(LogLevel::Error, message);
        }

        template <typename... Args>
        static void Fatal(const std::string& message, Args&&... args)
        {
            Log(LogLevel::Fatal, message, std::forward<Args>(args)...);
        }

        template <typename T>
        static void Fatal(const T& message)
        {
            Log(LogLevel::Fatal, message);
        }

    private:
        inline static const std::string_view ParseLogLevel(LogLevel level)
        {
            switch (level)
            {
                case LogLevel::Trace: return "\u001b[0m";
                case LogLevel::Debug: return "\u001b[32;1m";
                case LogLevel::Info:  return "\u001b[36;1m";
                case LogLevel::Warn:  return "\u001b[33;1m";
                case LogLevel::Error: return "\u001b[31;1m";
                case LogLevel::Fatal: return "\u001b[41;1m\u001b[30m";
                default:
                    return "";
            }
        }

        inline static LogLevel s_Level = LogLevel::Info;
    };
}
