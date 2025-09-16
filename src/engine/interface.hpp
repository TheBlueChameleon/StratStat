#ifndef INTERFACE_HPP
#define INTERFACE_HPP

#include <filesystem>
#include <string>
#include <utility>
#include <vector>

#include <csv2/reader.hpp>
#include <spdlog/spdlog.h>

#include <jsonxx.h>
#include <json-schema/jsonvalidation.hpp>

#include "types.hpp"
#include "basepkmndatabase.hpp"
#include "basemovedatabase.hpp"

constexpr auto SUCCESS = 0;
constexpr auto EXPECTED_SIGNATURE = 0x504B4D4E;  // "PKMN"

extern "C" {
    int getSignature();

    // oldStyle: pass mutable ref to avoid incomplete type error of std::vector<T> in extern C
    void getPkmnDefHeaders(std::vector<VariantContentInfo>& buffer);
    void getMoveDefHeaders(std::vector<VariantContentInfo>& buffer);
    void getTeamDefStructure();

    void connectLogger(const std::shared_ptr<spdlog::logger>& logger);
    void init(const std::filesystem::path& pkmnDefs, const std::filesystem::path& moveDefs);
    bool isReady();
    void shutdown();
}

void initPkmnDb(const std::filesystem::path& pkmnDefs);
void initMoveDb(const std::filesystem::path& pkmnDefs);

std::vector<CsvMappingInfo> analyzeHeader(const DefaultCsvReader::Row& header, const std::vector<VariantContentInfo>& headerRequirements, const std::string& filename);
std::unordered_map<std::string, VariantContentType> parseCsvRow(const DefaultCsvReader::Row& row, const std::vector<CsvMappingInfo>& columnData);

VariantContentType variantFromString(const std::string& input, const VariantContentID contentID);

#endif // INTERFACE_HPP
