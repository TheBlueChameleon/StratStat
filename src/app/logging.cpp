#include <iostream>

#include <spdlog/spdlog.h>
#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>

#include "logging.hpp"

void configureLogger(const Config& cfg)
{
    const auto pattern = "[%Y-%m-%d %H:%M:%S] [%l] %v";
    const auto logLevel = static_cast<spdlog::level::level_enum>(cfg.getLogLevel());

    auto defLogger = std::make_shared<spdlog::logger>(DEFALUT_LOGGER_NAME);
    spdlog::set_default_logger(defLogger);

    auto console_sink = std::make_shared<spdlog::sinks::stderr_color_sink_mt>();
    console_sink->set_level(logLevel);
    console_sink->set_pattern(pattern);

    defLogger->sinks().push_back(console_sink);
    defLogger->set_level(logLevel);

    if (cfg.getLogFile().has_value())
    {
        try
        {
            auto file_sink = std::make_shared<spdlog::sinks::basic_file_sink_mt>(
                                 cfg.getLogFile().value().c_str(),
                                 /*truncate = */true
                             );
            file_sink->set_level(logLevel);
            file_sink->set_pattern(pattern);

            defLogger->sinks().push_back(file_sink);
        }
        catch (const spdlog::spdlog_ex& ex)
        {
            std::cerr << "Log init failed: " << ex.what() << std::endl;
        }
    }
}

void logCfg(const Config& cfg)
{
    const std::string logFile = cfg.getLogFile().has_value() ?
                                cfg.getLogFile().value().c_str() :
                                "<not set>";

    spdlog::debug("### CONFIGURATION:");
    spdlog::debug("ENGINE: {}", cfg.getEngine().c_str());
    spdlog::debug("HUMAN_TEAM       : {}", cfg.getHumanTeam().c_str());
    spdlog::debug("HUMAN_STRATEGY   : {}", cfg.getHumanStrat().c_str());
    spdlog::debug("ENEMY_TEAM       : {}", cfg.getEnemyTeam().c_str());
    spdlog::debug("ENEMY_STRATEGY   : {}", cfg.getEnemyStrat().c_str());
    spdlog::debug("PKMN_DEFS        : {}", cfg.getPkmnDefs().c_str());
    spdlog::debug("MOVE_DEFS        : {}", cfg.getMoveDefs().c_str());
    spdlog::debug("REPETITIONS      : {}", cfg.getRepetitions());
    spdlog::debug("MAX_TURNS        : {}", cfg.getMaxTurns());
    spdlog::debug("LOGFILE          : {}", logFile);
    spdlog::debug("LOGLEVEL         : {}", cfg.getLogLevel());
    spdlog::debug("### END OF CONFIGURATION");
}
