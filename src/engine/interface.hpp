#ifndef INTERFACE_HPP
#define INTERFACE_HPP

#include <filesystem>
#include <utility>
#include <unordered_set>
#include <vector>

#include <spdlog/spdlog.h>

#include <json-schema/jsonvalidation.hpp>

#include "sharedtypes.hpp"


constexpr auto SUCCESS = 0;
constexpr auto EXPECTED_SIGNATURE = 0x504B4D4E;  // "PKMN"

extern "C" {
    int getSignature();

    // oldStyle: pass mutable ref to avoid incomplete type error of std::vector<T> in extern C
    void getTypeNames(JsonValidation::AllowedValues& buffer);
    void getPkmnDefHeaders(std::vector<StratStat::VariantContentInfo>& buffer);
    void getMoveDefHeaders(std::vector<StratStat::VariantContentInfo>& buffer);
    void getTeamDefStructure(JsonValidation::SpecificationSet& specs);

    void connectLogger(const std::shared_ptr<spdlog::logger>& logger);
    void init(const std::filesystem::path& pkmnDefs, const std::filesystem::path& moveDefs);
    void loadTeams(const std::filesystem::path& player1TeamDef, const std::filesystem::path& player2TeamDef);
    bool isReady();
    void shutdown();
}

#endif // INTERFACE_HPP
