#include <algorithm>
#include <ranges>
#include <string>
using namespace std::string_literals;

#include "interface.hpp"
#include "shared.hpp"

using namespace StratStat;

extern "C" {
    int getSignature()
    {
        return EXPECTED_SIGNATURE;
    }

    void connectLogger(const std::shared_ptr<spdlog::logger>& logger)
    {
        spdlog::set_default_logger(logger);
    }

    void init(const std::filesystem::path& pkmnDefs, const std::filesystem::path& moveDefs)
    {
        spdlog::debug("INITIALIZING ENGINE");
        loadPkmnDb(pkmnDefs);
        loadMoveDb(moveDefs);
        spdlog::debug("... DONE");
    }

    void loadTeams(const std::filesystem::path& player1TeamDef, const std::filesystem::path& player2TeamDef)
    {
        spdlog::debug("INITIALIZING TEAMS");
        loadTeam1(player1TeamDef);
        loadTeam2(player2TeamDef);
        spdlog::debug("... DONE");
    }

    bool isReady()
    {
        return readyFlag;
    }
}
