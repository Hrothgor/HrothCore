#include "HrothCore_pch.hpp"

#include "HrothCore/Core/Logger.hpp"

#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/basic_file_sink.h>

namespace HrothCore
{
    std::shared_ptr<spdlog::logger> Logger::s_SpdLogger;

    void Logger::Init(const std::string &logFile)
    {
        auto console_sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();

        auto file_sink = std::make_shared<spdlog::sinks::basic_file_sink_mt>(logFile, true);

        std::vector<spdlog::sink_ptr> sinks = {console_sink, file_sink};

        s_SpdLogger = std::make_shared<spdlog::logger>("HrothCore", sinks.begin(), sinks.end());
        spdlog::register_logger(s_SpdLogger);
        s_SpdLogger->set_pattern("%^[%T] [%l] %n: %v%$");
        s_SpdLogger->set_level(spdlog::level::debug);
        s_SpdLogger->flush_on(spdlog::level::debug);
    }
}