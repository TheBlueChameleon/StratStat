#include <iostream>
#include <string>
using namespace std::string_literals;

#include <lua.hpp>

#include "config.hpp"
#include "cliparser.hpp"
#include "logging.hpp"
#include "enginewrapper.hpp"

#include "tests.hpp"

void doLuaStuff()
{
    lua_State* L = luaL_newstate();
    luaL_openlibs(L);
    luaL_dofile(L, "./strategies/hello.lua");
    lua_close(L);
}

int main(const int argc, const char* argv[])
{
    testJsonValidation();
    std::exit(0);

    auto parser = CliParser();
    auto cfg = parser.run(argc, argv);

    configureLogger(cfg);
    logCfg(cfg);

    auto ew = EngineWrapper(cfg.getEngine());

    ew.connectLogger(spdlog::get(DEFALUT_LOGGER_NAME));
    ew.init(cfg.getPkmnDefs(), cfg.getMoveDefs());
    ew.loadTeams(cfg.getHumanTeam(), cfg.getEnemyTeam());
    // doLuaStuff();

    return 0;
}
