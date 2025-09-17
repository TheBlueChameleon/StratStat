#ifndef INTERFACE_HPP
#define INTERFACE_HPP

#include <filesystem>
#include <utility>
#include <unordered_set>
#include <vector>

#include <spdlog/spdlog.h>

#include <json-schema/jsonvalidation.hpp>

#include "types.hpp"


constexpr auto SUCCESS = 0;
constexpr auto EXPECTED_SIGNATURE = 0x504B4D4E;  // "PKMN"

extern "C" {
    int getSignature();

    // oldStyle: pass mutable ref to avoid incomplete type error of std::vector<T> in extern C
    void getPkmnDefHeaders(std::vector<VariantContentInfo>& buffer);
    void getMoveDefHeaders(std::vector<VariantContentInfo>& buffer);
    void getTeamDefStructure(std::unordered_set<JsonValidation::Node>& specs);

    void connectLogger(const std::shared_ptr<spdlog::logger>& logger);
    void init(const std::filesystem::path& pkmnDefs, const std::filesystem::path& moveDefs);
    bool isReady();
    void shutdown();
}

#endif // INTERFACE_HPP
