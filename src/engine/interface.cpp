#include <algorithm>
#include <ranges>
#include <string>
using namespace std::string_literals;

#include "interface.hpp"
#include "shared.hpp"

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
        initPkmnDb(pkmnDefs);
        initMoveDb(moveDefs);
        spdlog::debug("... DONE");
    }
}
