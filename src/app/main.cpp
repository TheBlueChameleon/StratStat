#include <iostream>
#include <dlfcn.h>

#include <lua.hpp>

#include <plog/Log.h>
#include <plog/Initializers/ConsoleInitializer.h>
#include <plog/Initializers/RollingFileInitializer.h>

#include "plogcustomformatter.hpp"

#include "config.hpp"
#include "cliparser.hpp"
#include "enginewrapper.hpp"

void configureLogger(const Config& cfg)
{
    static plog::ColorConsoleAppender<PlogCustomFormatter> consoleAppender;

    const auto severity = static_cast<plog::Severity>(cfg.getLogLevel());

    if (cfg.getLogFile().has_value())
    {
        static plog::RollingFileAppender<plog::TxtFormatter> fileAppender(cfg.getLogFile().value().c_str());

        plog::init(severity, &consoleAppender)
        .addAppender(&fileAppender);
    }
    else
    {
        plog::init(severity, &consoleAppender);
    }
}

void logCfg(const Config& cfg)
{
    PLOG_VERBOSE << "### CONFIGURATION:";
    PLOG_VERBOSE << ENGINE << "\t" << cfg.getEngine();
    PLOG_VERBOSE << HUMAN_TEAM << "\t" << cfg.getHumanTeam();
    PLOG_VERBOSE << HUMAN_STRATEGY << "\t" << cfg.getHumanStrat();
    PLOG_VERBOSE << ENEMY_TEAM << "\t"  << cfg.getEnemyTeam();
    PLOG_VERBOSE << ENEMY_STRATEGY << "\t" << cfg.getEnemyStrat();
    PLOG_VERBOSE << PKMN_DEFS << "\t" << cfg.getPkmnDefs();
    PLOG_VERBOSE << MOVE_DEFS << "\t" << cfg.getMoveDefs();
    PLOG_VERBOSE << REPETITIONS << "\t" << cfg.getRepetitions();
    PLOG_VERBOSE << MAX_TURNS << "\t" << cfg.getMaxTurns();
    PLOG_VERBOSE << LOGFILE << "\t" << cfg.getLogFile().value_or("<not set>");
    PLOG_VERBOSE << LOGLEVEL << "\t" << cfg.getLogLevel();
    PLOG_VERBOSE << "### END OF CONFIGURATION";
}

void doLuaStuff()
{
    lua_State* L = luaL_newstate();
    luaL_openlibs(L);
    luaL_dofile(L, "./strategies/hello.lua");
    lua_close(L);
}

int main(const int argc, const char* argv[])
{
    auto parser = CliParser();
    auto cfg = parser.run(argc, argv);

    configureLogger(cfg);
    logCfg(cfg);

    auto ew = EngineWrapper(cfg.getEngine());
    std::vector<TextFileContentInfo> hdr;
    ew.init(cfg.getPkmnDefs(), cfg.getMoveDefs());
    PLOG_VERBOSE << "post";

    // doLuaStuff();

    return 0;
}
