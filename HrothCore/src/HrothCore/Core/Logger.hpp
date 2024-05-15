#pragma once

#include <spdlog/spdlog.h>

#if DEBUG
    #define HC_LOG_DEBUG(format, ...) HrothCore::Logger::GetSpdLogger()->debug(format, __VA_ARGS__)
    #define HC_LOG_INFO(format, ...) HrothCore::Logger::GetSpdLogger()->info(format, __VA_ARGS__)
    #define HC_LOG_WARNING(format, ...) HrothCore::Logger::GetSpdLogger()->warn(format, __VA_ARGS__)
    #define HC_LOG_ERROR(format, ...) HrothCore::Logger::GetSpdLogger()->error(format, __VA_ARGS__)
    #define HC_LOG_CRITICAL(format, ...) HrothCore::Logger::GetSpdLogger()->critical(format, __VA_ARGS__)
#else
    #define HC_LOG_DEBUG(format, ...) void(0)
    #define HC_LOG_INFO(format, ...) void(0)
    #define HC_LOG_WARNING(format, ...) HrothCore::Logger::GetSpdLogger()->warn(format, __VA_ARGS__)
    #define HC_LOG_ERROR(format, ...) HrothCore::Logger::GetSpdLogger()->error(format, __VA_ARGS__)
    #define HC_LOG_CRITICAL(format, ...) HrothCore::Logger::GetSpdLogger()->critical(format, __VA_ARGS__)
#endif

namespace HrothCore
{
    class Logger
    {
        public:
            static void Init(const std::string &logFile);

            static std::shared_ptr<spdlog::logger> &GetSpdLogger() { return s_SpdLogger; }
        private:
            static std::shared_ptr<spdlog::logger> s_SpdLogger;
    };
}