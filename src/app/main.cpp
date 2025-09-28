#include <iostream>
#include <string>
using namespace std::string_literals;

#include "cliparser.hpp"
#include "config.hpp"
#include "enginewrapper.hpp"
#include "errors.hpp"
#include "logging.hpp"

int main(const int argc, const char* argv[])
{
    try
    {
        auto parser = CliParser();
        auto cfg = parser.run(argc, argv);

        configureLogger(cfg);
        logCfg(cfg);

        auto ew = EngineWrapper(cfg.getEngine());

        ew.connectLogger(spdlog::get(DEFALUT_LOGGER_NAME));
        ew.init(cfg.getPkmnDefs(), cfg.getMoveDefs());
        ew.loadTeams(cfg.getHumanTeam(), cfg.getEnemyTeam());
    }
    catch (const CriticalAbort& e)
    {
        std::cerr << e.what() << std::endl;
        return -1;
    }

    return 0;
}
