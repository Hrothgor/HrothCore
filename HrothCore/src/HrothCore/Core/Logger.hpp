#pragma once

#include <spdlog/spdlog.h>

#if DEBUG
    #define HC_LOG_DEBUG(...) HrothCore::Logger::GetSpdLogger()->debug(__VA_ARGS__)
    #define HC_LOG_INFO(...) HrothCore::Logger::GetSpdLogger()->info(__VA_ARGS__)
    #define HC_LOG_WARNING(...) HrothCore::Logger::GetSpdLogger()->warn(__VA_ARGS__)
    #define HC_LOG_ERROR(...) HrothCore::Logger::GetSpdLogger()->error(__VA_ARGS__)
    #define HC_LOG_CRITICAL(...) HrothCore::Logger::GetSpdLogger()->critical(__VA_ARGS__)
#else
    #define HC_LOG_DEBUG(...) void(0)
    #define HC_LOG_INFO(...) HrothCore::Logger::GetSpdLogger()->info(__VA_ARGS__)
    #define HC_LOG_WARNING(...) HrothCore::Logger::GetSpdLogger()->warn(__VA_ARGS__)
    #define HC_LOG_ERROR(...) HrothCore::Logger::GetSpdLogger()->error(__VA_ARGS__)
    #define HC_LOG_CRITICAL(...) HrothCore::Logger::GetSpdLogger()->critical(__VA_ARGS__)
#endif

namespace HrothCore
{
    namespace Logger
    {
        void Init(const std::string &logFile);
        std::shared_ptr<spdlog::logger> &GetSpdLogger();
    }
}